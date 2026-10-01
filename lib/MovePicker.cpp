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
  if (move.is_en_passant()) {
    return 0;
  }
  // return eval::kPieceValues[GetPieceBase(pos_.PieceOn(move.get_to()))] -
        //  eval::kPieceValues[GetPieceBase(pos_.PieceOn(move.get_from()))];
  return eval::kMVP_LVA[GetPieceBase(pos_.PieceOn(move.get_to()))][GetPieceBase(pos_.PieceOn(move.get_from()))];
  // return eval::kMVP_LVA2[(std::to_underlying(GetPieceBase(pos_.PieceOn(move.get_to()))) << 3)  +
                          // std::to_underlying(GetPieceBase(pos_.PieceOn(move.get_from())))];
}

void MovePicker::SkipPvMove() {
  // if (pv_move_.has_value() && list_[ind_] == pv_move_.value()) {
  //   ind_++;
  //   return YieldMove2();
  // }
}

Move MovePicker::YieldMove2() {
  switch (stage2_) {
  case PickerStage::kPrincipalVariation:
    stage2_++;
    if (pv_move_.has_value() && (pv_move_->is_capture() || !only_captures_)) {
      return pv_move_.value();
    }
    [[fallthrough]];
  case PickerStage::kGoodCaptures:
    if (ind_ == 0) {
      pos_.GenerateMoves<MovesType::kCaptures>(captures_list_);
    }
    if (ind_ != captures_list_.size()) {
      SortOutCapture2([](int score) { return score < kSeeThreshold; });
    }
    if (pv_move_.has_value() && list_[ind_] == pv_move_.value()) {
      ind_++;
      return YieldMove2();
    }
    if (is_good_) {
      return captures_list_[ind_++];
    }
    ind_last_capture_ = ind_;
    ind_ = 0;
    stage2_++;
    [[fallthrough]];
  case PickerStage::kQuiets:
    if (ind_ == 0) {
      pos_.GenerateMoves<MovesType::kQuiets>(quiets_list_);
    }
    if (pv_move_.has_value() && list_[ind_] == pv_move_.value()) {
      ind_++;
      return YieldMove2();
    }
    if (!quiets_list_.empty()) {
      return quiets_list_[ind_++];
    }
    stage2_++;
    [[fallthrough]];
  case PickerStage::kBadCaptures:
    ind_ = ind_last_capture_;
    if (ind_ == captures_list_.size()) {
      return Move();
    }
    SortOutCapture2([](int score) { return score < kSeeThreshold; });
    return captures_list_[ind_++];
  }
  std::unreachable();
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
    stage_ = PickerStage::kGoodCaptures;
  }
  if (ind_ == list_.size() && stage_ == PickerStage::kGoodCaptures && !only_captures_) {
    pos_.GenerateMoves<MovesType::kQuiets>(list_);
    stage_ = PickerStage::kQuiets;
  }
  if (ind_ >= list_.size()) {
    return kNullMove;
  }
  if (stage_ == PickerStage::kGoodCaptures) {
    // are_captures_sorted = true;
    SortOutCapture();
    // if (list_[])
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

// template<typename F>
// void MovePicker::SortOutCapture2(F fun) {
//   //* ordering by MVV-LVA (Most Valuable Victim - Least Valuable Aggressor), insertion sort
//   std::size_t sz = list_.size();
//   [[assume(ind_ <= sz)]];
//   int mx_diff = GetDiff(list_[ind_]);
//   std::size_t mx_ind = ind_;
//   for (std::size_t i = ind_ + 1; i < sz; ++i) {
//     int curr_diff = GetDiff(list_[i]);
//     if (curr_diff > mx_diff) {
//       mx_diff = curr_diff;
//       mx_ind = i;
//     }
//   }
//   if (fun(mx_diff)) {
//     is_good_ = true;
//     return;
//   }
//   std::swap(list_[ind_], list_[mx_ind]);
// }

std::size_t MovePicker::size() const {
  return ind_;
}

bool MovePicker::empty() const {
  return list_.empty();
}

}// namespace chess