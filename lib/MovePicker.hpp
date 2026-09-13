#pragma once

#include "types/Piece.hpp"

#include "Evaluation.hpp"
#include "Move.hpp"
#include "Position.hpp"

namespace chess {

class MovePicker {
public:
  MovePicker(const Position& pos);
  MovePicker(const Position& pos, const bool only_captures);
  bool has_next();
  Move YieldMove();
  std::size_t size() const;
  bool empty() const;

  decltype(auto) operator[](this auto& self, const std::size_t ind) {
    return self.list_[ind];
  }

private:
  const Position& pos_;
  MoveList list_;
  std::size_t ind_ = 0;
  bool only_captures_ = false;
  bool is_captures_ = false;
  bool is_quiets_ = false;

  void SortOutCapture();
  int GetDiff(const Move& move);
};

}// namespace chess