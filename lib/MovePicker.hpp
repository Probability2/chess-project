#pragma once

#include "types/Piece.hpp"

#include "Evaluation.hpp"
#include "Move.hpp"
#include "Position.hpp"

namespace chess {

inline constexpr int kMaxDepth = 64;

enum class PickerStage: uint8_t {
  kPrincipalVariation,
  kGoodCaptures,
  kQuiets,
  kBadCaptures,
  kNone
};

inline PickerStage operator++(const PickerStage stage, int) {
  [[assume(stage != PickerStage::kBadCaptures)]];
  return static_cast<PickerStage>(std::to_underlying(stage) + 1);
}

class MovePicker {
public:
  MovePicker(const Position& pos);
  MovePicker(const Position& pos, const bool only_captures);
  MovePicker(const Position& pos, const Move pv_move);
  Move YieldMove();
  std::size_t size() const;
  bool empty() const;
  Move YieldMove2();

  // decltype(auto) operator[](this auto& self, const std::size_t ind) {
  //   return self.list_[ind];
  // }

private:
  const Position& pos_;
  MoveList list_;
  std::size_t ind_ = 0;
  bool only_captures_ = false;
  bool is_captures_ = false;
  bool is_quiets_ = false;

  std::size_t ind_last_capture_ = 0;
  MoveList quiets_list_;
  MoveList captures_list_;

  std::optional<Move> pv_move_;
  PickerStage stage_ = PickerStage::kNone;

  PickerStage stage2_ = PickerStage::kPrincipalVariation;

  bool is_good_ = true;

  void SortOutCapture();

  void SortOutCapture2(std::invocable<int> auto fun) {
    //* ordering by MVV-LVA (Most Valuable Victim - Least Valuable Aggressor), insertion sort
    std::size_t sz = list_.size();
    [[assume(ind_ <= sz)]];
    int mx_diff = GetDiff(list_[ind_]);
    std::size_t mx_ind = ind_;
    for (std::size_t i = ind_ + 1; i < sz; ++i) {
      int curr_diff = GetDiff(list_[i]);
      if (curr_diff > mx_diff) {
        mx_diff = curr_diff;
        mx_ind = i;
      }
    }
    if (fun(list_[mx_ind])) {
      is_good_ = false;
      return;
    }
    std::swap(list_[ind_], list_[mx_ind]);
  }

  int GetDiff(const Move& move);

  void SkipPvMove();
};

}// namespace chess