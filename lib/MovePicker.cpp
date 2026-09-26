#include "MovePicker.hpp"

namespace chess {

MovePicker::MovePicker(const Position& pos)
: pos_(pos) {
}

MovePicker::MovePicker(const Position& pos, const bool only_captures)
: pos_(pos)
, only_captures_(only_captures) {
}

MovePicker::MovePicker(const Position& pos, const Move pv_move)
: pos_(pos)
, pv_move_(pv_move) {
}

// bool MovePicker::has_next() const {
//   return ind_ < list_.size() || (stage_ == PickerStage::kNone && pv_move_.has_value());
// }

int MovePicker::GetDiff(const Move& move) {
  return eval::kPieceValues[GetPieceBase(pos_.PieceOn(move.get_to()))] -
         eval::kPieceValues[GetPieceBase(pos_.PieceOn(move.get_from()))];
}

Move MovePicker::YieldMove() {
  if (stage_ == PickerStage::kNone) {
    stage_ = PickerStage::kPrincipalVariation;
    if (pv_move_.has_value() && (pv_move_->is_capture() || !only_captures_)) {
      return pv_move_.value();
    }
  }
  if (ind_ == 0 && stage_ == PickerStage::kPrincipalVariation) {
    pos_.GenerateMoves<MovesType::kCaptures>(list_);
    stage_ = PickerStage::kCaptures;
  }
  if (ind_ == list_.size() && stage_ == PickerStage::kCaptures && !only_captures_) {
    pos_.GenerateMoves<MovesType::kQuiets>(list_);
    stage_ = PickerStage::kQuiets;
  }
  if (ind_ >= list_.size()) {
    return Move();
  }
  if (stage_ == PickerStage::kCaptures) {
    SortOutCapture();
  }
  if (pv_move_.has_value() && list_[ind_] == pv_move_.value()) {
    ind_++;
    return YieldMove();
  }
  
  return list_[ind_++];
}

void MovePicker::SortOutCapture() {
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
  std::swap(list_[ind_], list_[mx_ind]);
}

std::size_t MovePicker::size() const {
  return ind_;
}

bool MovePicker::empty() const {
  return list_.empty();
}

}// namespace chess