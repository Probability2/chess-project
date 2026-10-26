#include "FileManager.hpp"
#include "MovePicker.hpp"
#include "Search.hpp"

namespace chess {

Searcher::Searcher()
: pos_(kStartingPosition)
, is_cancelled_(false) {
}

Searcher::Searcher(Position pos)
: pos_(std::move(pos))
, is_cancelled_(false) {
}

void Searcher::SetPosition(const Position& pos) {
  pos_ = pos;
}

void Searcher::MakeMove(const Move move) {
  pos_.MakeMove(move);
}

void Searcher::UnmakeMove(const Move move) {
  pos_.UnmakeMove(move);
}

Position& Searcher::get_position() {
  return pos_;
}

inline bool Searcher::IsTimeOut() const {
  if (limits_.move_time_.count() == 0) {
    return false;
  }
  return std::chrono::steady_clock::now() - start_time_ > limits_.move_time_;
}

int Searcher::QuiescenceSearch(NodeInfo info) {
  if (++nodes_ >= limits_.mx_nodes_ || ((nodes_ & kCheckTimePeriod) == 0 && IsTimeOut())) [[unlikely]] {
    is_cancelled_ = true;
    return 0;
  }
  int stand_pat = eval::Evaluate(pos_);
  if (stand_pat >= info.beta_) {
    return info.beta_;
  }
  info.alpha_ = std::max(info.alpha_, stand_pat);
  MovePicker picker(pos_, true);
  while (Move move = picker.YieldMove2()) {
    pos_.MakeMove(move);
    int score = -QuiescenceSearch(NodeInfo{-info.beta_, -info.alpha_});
    pos_.UnmakeMove(move);
    if (is_cancelled_) [[unlikely]] {
      return 0;
    }
    if (score >= info.beta_) {
      return info.beta_;
    }
    info.alpha_ = std::max(info.alpha_, score);
  }

  return info.alpha_;
}

int Searcher::Search(NodeInfo info, const int ply) { //negamax
  if (info.depth_ <= 0) {
    return QuiescenceSearch(info); //till' there are no captures (+maybe checks)
    // return eval::Evaluate(pos_);
  }
  info.checks_ = (pos_.is_check()) ? info.checks_ + 1 : info.checks_;
  int extension = (pos_.is_check() && info.checks_ < kMxChecksExtension) ? 0 : 1;
  MovePicker picker(pos_);
  while (Move move = picker.YieldMove()) {
    // Move move = picker.YieldMove();
    pos_.MakeMove(move);
    int score = -Search(NodeInfo{-info.beta_, -info.alpha_, info.depth_ - extension, info.checks_}, ply + 1);
    pos_.UnmakeMove(move);
    if (score >= info.beta_) {
      return info.beta_;
    }
    info.alpha_ = std::max(info.alpha_, score);
  }
  if (picker.empty()) {
    if (pos_.is_check()) {
      return -kInfinity + ply;
    }
    return 0;
  }

  return info.alpha_;
}

Move Searcher::GetBestMove(const std::size_t depth) {
  if (depth == 0) {
    return kNullMove;
  }
  MovePicker picker(pos_);
  Move best_move = kNullMove;
  int alpha = -kInfinity;
  int beta = +kInfinity;
  while (Move move = picker.YieldMove()) {
    pos_.MakeMove(move);
    int score = -Search(NodeInfo{-beta, -alpha, depth - 1, 0}, 0);
    pos_.UnmakeMove(move);
    if (score > alpha) {
      alpha = score;
      best_move = move;
    }
  }

  return best_move;
}

void Searcher::AddKillerMove(const Move move, const int ply) {
  if (move == killers_[ply][0]) {
    return;
  }

  // for (std::size_t i = 1; i < kMaxKillerMoves; ++i) {
  //   killers_[ply][i] = killers_[ply][i - 1];
  // }
  killers_[ply][1] = killers_[ply][0];
  killers_[ply][0] = move;
}

bool Searcher::IsThreeFoldDraw() const {
  std::size_t no_capture_moves = pos_.get_no_capture_moves();
  auto stack = pos_.GetStateStack().AsSpan();
  std::size_t sz = stack.size();
  if (sz < 4 || no_capture_moves < 4) {
    return false;
  }
  Bitboard pos_key = pos_.get_zobrist_key();
  for (std::size_t i = 4; i <= sz && i <= no_capture_moves; i += 2) {
    if (stack[sz - i].zobrist_key_ == pos_key) {
      return true;
    }
  }

  return false;
}

void Searcher::CopyPvMoves(const int index, const int next_index, const int ply_moves) {
  for (std::size_t m = 1; m < ply_moves; ++m) {
    pv_moves_[index + m] = pv_moves_[next_index + m - 1];
    if (!pv_moves_[index + m]) {
      break;
    }
  }
}

void Searcher::UpdateHistory(const ColorType side, const Move move, const int depth) {
  int bonus = std::clamp(depth * depth, -kMaxHistory, kMaxHistory);
  Square from = move.get_from();
  Square to = move.get_to();
  history_[side][from, to] += (bonus - history_[side][from, to] * std::abs(bonus) / kMaxHistory);
}

int Searcher::IterativeSearch(NodeInfo info, const int ply, const int pv_index, const bool is_main_line) {
  // triangular pv-table
  constexpr int kIndLMR = 3;
  if (++nodes_ >= limits_.mx_nodes_ || ((nodes_ & kCheckTimePeriod) == 0 && IsTimeOut())) [[unlikely]] {
    is_cancelled_ = true;
    return 0;
  }
  if (ply > 0 && (IsThreeFoldDraw() || pos_.get_no_capture_moves() > kMxNoCaptureMoves)) [[unlikely]] {
    return 0;
  }
  if (info.depth_ == 0) {
    pv_moves_[pv_index] = kNullMove;
    return QuiescenceSearch(info);
  }
  bool is_check = pos_.is_check();
  ColorType side = pos_.side_to_move();
  info.checks_ = is_check ? info.checks_ + 1 : info.checks_;
  int extension = (is_check && info.checks_ < kMxChecksExtension) ? 0 : 1;
  int ply_moves = kMaxDepth - ply;
  int pv_next_index = ply_moves + pv_index;
  auto pv_move = is_main_line ? pv_moves_[ply] : kNullMove;
  auto picker = MovePicker(pos_, history_[side], killers_[ply], pv_move);
  pv_moves_[pv_index] = kNullMove;
  // std::size_t i = 0;
  while (Move move = picker.YieldMove2()) {
    pos_.MakeMove(move);
    int score = 0;
    bool is_full_search = true;
    if (info.depth_ >= 3 && picker.IsAfterGoodCaptures() && !pos_.is_check()) {
      score = -IterativeSearch(NodeInfo{-info.alpha_ - 1, -info.alpha_, info.depth_ - kReduceDepth - 1,
                                         info.checks_}, ply + 1, pv_next_index, false);
      is_full_search = (score > info.alpha_);
    }
    if (is_full_search && !is_cancelled_) {
      score = -IterativeSearch(NodeInfo{-info.beta_, -info.alpha_, info.depth_ - extension, info.checks_},
                                            ply + 1, pv_next_index, move == pv_move);
    }
    // score = -IterativeSearch(NodeInfo{-info.beta_, -info.alpha_, info.depth_ - extension, info.checks_},
    //                                         ply + 1, pv_next_index, move == pv_move);
    pos_.UnmakeMove(move);
    if (is_cancelled_) [[unlikely]] {
      return 0;
    }
    if (score >= info.beta_) {
      if (move.is_quiet()) {
        AddKillerMove(move, ply);
        UpdateHistory(side, move, info.depth_);
        // history_[side][move.get_from(), move.get_to()] += info.depth_ * info.depth_;
      }
      return info.beta_;
    }
    if (score > info.alpha_) {
      info.alpha_ = score;
      pv_moves_[pv_index] = move;
      CopyPvMoves(pv_index, pv_next_index, ply_moves);
    }
  }
  if (picker.empty()) [[unlikely]] {
    if (is_check) {
      return -kInfinity + ply;
    }
    return 0;
  }

  return info.alpha_;
}

Move Searcher::IterativeBestMove() {
  start_time_ = std::chrono::steady_clock::now();
  nodes_ = 0;
  is_cancelled_ = false;
  std::ranges::fill(pv_moves_, kNullMove);
  history_ = {};
  Move best_move;
  std::size_t depth = 1;
  for (depth = 1; depth <= limits_.depth_; ++depth) {
    int _ = IterativeSearch(NodeInfo{-kInfinity, +kInfinity, depth, 0}, 0, 0, true);
    if (is_cancelled_) {
      break;
    }
    best_move = pv_moves_[0];
  }
  std::cout << "The move is from depth : " <<  depth - 1 << std::endl;
  std::cout << "Total nodes: " << nodes_ << std::endl;
  // std::cout << "Total time: " << std::chrono::duration<double>(std::chrono::steady_clock::now() - start_time_).count() << "s" << std::endl;

  return best_move;
}

void Searcher::clear() {
  pos_ = Position();
  is_cancelled_ = false;
  std::ranges::fill(pv_moves_, kNullMove);
  nodes_ = 0;
}

void Searcher::set_limits(const SearchLimits& limits) {
  limits_ = limits;
}

void Searcher::flip_side() {
  pos_.flip_side();
}

}