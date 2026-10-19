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
    // Move move = picker.YieldMove();
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
    return Move();
  }
  MovePicker picker(pos_);
  Move best_move = Move();
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

// void Searcher::AddKillerMove(const Move& move, const int ply) {
//   if (move.is_capture() || killer_moves_[ply][0] == move || !move_generator::IsLegal(pos_, move)) {
//     return;
//   }
//   for (std::size_t i = 1; i < kMaxKillerMoves; ++i) {
//     killer_moves_[ply][i] = killer_moves_[ply][i - 1];
//   }
//   killer_moves_[ply][0] = move;
// }

int Searcher::IterativeSearch(NodeInfo info, const int ply, const int pv_index, const bool is_main_line) {
  // triangular pv-table
  if (++nodes_ >= limits_.mx_nodes_ || ((nodes_ & kCheckTimePeriod) == 0 && IsTimeOut())) [[unlikely]] {
    is_cancelled_ = true;
    return 0;
  }
  if (info.depth_ == 0) {
    pv_moves_[pv_index] = kNullMove;
    return QuiescenceSearch(info);
  }
  bool is_check = pos_.is_check();
  info.checks_ = is_check ? info.checks_ + 1 : info.checks_;
  int extension = (is_check && info.checks_ < kMxChecksExtension) ? 0 : 1;
  int ply_moves = kMaxDepth - ply;
  int pv_next_index = ply_moves + pv_index;
  auto pv_move = is_main_line ? pv_moves_[ply] : kNullMove;
  auto picker = pv_move ? MovePicker(pos_, pv_move)
                        : MovePicker(pos_);
  pv_moves_[pv_index] = kNullMove;
  while (Move move = picker.YieldMove2()) {
    pos_.MakeMove(move);
    int score = -IterativeSearch(NodeInfo{-info.beta_, -info.alpha_, info.depth_ - extension, info.checks_},
                                           ply + 1, pv_next_index, move == pv_move);
    pos_.UnmakeMove(move);
    if (is_cancelled_) [[unlikely]] {
      return 0;
    }
    if (score >= info.beta_) {
      // AddKillerMove(move, ply);
      return info.beta_;
    }
    if (score > info.alpha_) {
      info.alpha_ = score;
      pv_moves_[pv_index] = move;
      for (std::size_t m = 1; m < ply_moves; ++m) {
        pv_moves_[pv_index + m] = pv_moves_[pv_next_index + m - 1];
        if (!pv_moves_[pv_index + m]) {
          break;
        }
      }
    }
  }
  if (picker.empty2()) {
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

}