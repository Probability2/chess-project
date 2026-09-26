#pragma once

#include "types/Piece.hpp"

#include "Evaluation.hpp"
#include "Move.hpp"
#include "Position.hpp"

namespace chess {

inline constexpr int kMaxDepth = 64;

enum class PickerStage: uint8_t {
  kPrincipalVariation,
  kCaptures,
  kQuiets,
  kNone
};

class MovePicker {
public:
  MovePicker(const Position& pos);
  MovePicker(const Position& pos, const bool only_captures);
  MovePicker(const Position& pos, const Move pv_move);
  // bool has_next() const;
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

  std::optional<Move> pv_move_;
  PickerStage stage_ = PickerStage::kNone;

  void SortOutCapture();
  int GetDiff(const Move& move);
};

}// namespace chess