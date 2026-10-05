#include "MovePicker.hpp"
#include "Search.hpp"

namespace chess {

Searcher::Searcher(Position pos)
: pos_(std::move(pos))
, is_time_out_(false) {
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

inline bool Searcher::IsTimeOut(const TimePoint start_time, const Ms duration) {
  return std::chrono::steady_clock::now() - start_time > duration;
}

int Searcher::QuiescenceSearch(NodeInfo info) {
  int score = eval::Evaluate(pos_);
  if (score >= info.beta_) {
    return info.beta_;
  }
  if (score > info.alpha_) {
    info.alpha_ = score;
  }
  MovePicker picker(pos_, true);
  while (Move move = picker.YieldMove2()) {
    // Move move = picker.YieldMove();
    pos_.MakeMove(move);
    score = -QuiescenceSearch(NodeInfo{-info.beta_, -info.alpha_});
    pos_.UnmakeMove(move);
    if (score >= info.beta_) {
      return info.beta_;
    }
    if (score > info.alpha_) {
      info.alpha_ = score;
    }
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
    // if (score > alpha) {
    //   alpha = score;
    // }
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

// void Searcher::ClearPvArray() {
//   for (int i = 0; i < pv_moves_.size(); ++i) {
//     pv_moves_[i] = Move();
//   }
// }

int Searcher::IterativeSearch(NodeInfo info, TimePoint start_time, const Ms move_time,
                              const int ply, const int pv_index, const bool is_main_line) {
  nodes_++;
  // triangular pv-table
  if ((nodes_ & kCheckTimePeriod) && (is_time_out_ || IsTimeOut(start_time, move_time))) [[unlikely]] {
    is_time_out_ = true;
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
  auto picker = pv_move ? MovePicker(pos_, pv_moves_[ply]) : MovePicker(pos_);
  pv_moves_[pv_index] = kNullMove;
  while (Move move = picker.YieldMove2()) {
    pos_.MakeMove(move);
    int score = -IterativeSearch(NodeInfo{-info.beta_, -info.alpha_, info.depth_ - extension, info.checks_},
                                            start_time, move_time, ply + 1, pv_next_index, move == pv_move);
    pos_.UnmakeMove(move);
    if (is_time_out_) [[unlikely]] {
      return 0;
    }
    if (score >= info.beta_) {
      return info.beta_;
    }
    if (score > info.alpha_) {
      info.alpha_ = score;
      pv_moves_[pv_index] = move;//  && pv_moves_[pv_next_index + m - 1]
      for (std::size_t m = 1; m < ply_moves; ++m) {
        // std::cout << pv_moves_[pv_next_index + m - 1] << " YOUUU\n";
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

Move Searcher::IterativeBestMove(const Ms move_time) {
  auto start_time = std::chrono::steady_clock::now();
  nodes_ = 0;
  // MovePicker picker(pos_);
  is_time_out_ = false;
  std::ranges::fill(pv_moves_, kNullMove);
  Move best_move;
  std::size_t depth = 1;
  for (depth = 1; depth < kMaxDepth; ++depth) {
    int score = IterativeSearch(NodeInfo{-kInfinity, +kInfinity, depth, 0}, start_time, move_time, 0, 0, true);
    // std::cout << "On depth: " << depth << '\n';
    // std::cout << nodes_ << '\n';
    if (is_time_out_) {
      break;
    }
    best_move = pv_moves_[0];
  }
  std::cout << "The move is from depth : " <<  depth - 1 << '\n';

  return best_move;
}

}