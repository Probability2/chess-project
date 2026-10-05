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

int MovePicker::CalculateScore(const Move move) {
  if (move.is_en_passant()) [[unlikely]] {
    return 0;
  }
  // return eval::kPieceValues[GetPieceBase(pos_.PieceOn(move.get_to()))] -
        //  eval::kPieceValues[GetPieceBase(pos_.PieceOn(move.get_from()))];
  return eval::kMVP_LVA[GetPieceBase(pos_.PieceOn(move.get_to()))][GetPieceBase(pos_.PieceOn(move.get_from()))];
  // return eval::kMVP_LVA2[(std::to_underlying(GetPieceBase(pos_.PieceOn(move.get_to()))) << 3)  +
                          // std::to_underlying(GetPieceBase(pos_.PieceOn(move.get_from())))];
}

void MovePicker::ScoreMoves() {
  for (auto& move: captures_.AsSpan()) {
    move.score_ = CalculateScore(move);
  }
}

Move MovePicker::YieldMove2() {
  switch (stage2_) {
  case PickerStage::kPrincipalVariation:
    ++stage2_;
    if (pv_move_.has_value() && (pv_move_->is_capture() || !only_captures_)) {
      return *pv_move_;
    }
    [[fallthrough]];
  case PickerStage::kGenCaptures:
    move_generator::GenerateMoves<MovesType::kCaptures>(pos_, captures_);
    captures_.RemovePvMove(pv_move_);
    ScoreMoves();
    right_ = captures_.size() - 1;
    ++stage2_;
    [[fallthrough]];
  case PickerStage::kGoodCaptures:
    if (left_ <= right_) {
      if (Move capture = SortOutCapture2()) {
        return capture;
      }
    }
    left_ = 0;
    ++stage2_;
    [[fallthrough]];
  case PickerStage::kGenQuiets:
    if (only_captures_) {
      stage2_ = PickerStage::kBadCaptures;
      return YieldMove2();
    }
    move_generator::GenerateMoves<MovesType::kQuiets>(pos_, quiets_);
    quiets_.RemovePvMove(pv_move_);
    ++stage2_;
    [[fallthrough]];
  case PickerStage::kQuiets:
    if (ind2_ < quiets_.size()) {
      return quiets_[ind2_++];
    }
    ++stage2_;
    [[fallthrough]];
  case PickerStage::kBadCaptures:
    if (left_ < bad_captures_.size()) {
      return bad_captures_[left_++];
    }
    return kNullMove;
  }
  std::unreachable();
}

Move MovePicker::SortOutCapture2() {
  //* ordering by MVV-LVA (Most Valuable Victim - Least Valuable Aggressor), selection sort
  while (left_ <= right_) {
    // std::cout << left_ << ' ' << right_ << '\n';
    int mx_diff = captures_[left_].score_;
    int mx_ind = left_;
    for (int i = left_ + 1; i <= right_; ++i) {
      int curr_diff = captures_[i].score_;
      if (curr_diff > mx_diff) {
        mx_diff = curr_diff;
        mx_ind = i;
      }
    }
    // std::cout << std::boolalpha << captures_[mx_ind] << " omg what's happening " << pos_.IsGoodCapture(captures_[mx_ind]) << '\n';
    if (pos_.IsGoodCapture(captures_[mx_ind])) {
      std::swap(captures_[left_], captures_[mx_ind]);
      // std::cout << "yepp\n";
      return captures_[left_++];
    }
    bad_captures_.push(captures_[mx_ind]);
    std::swap(captures_[right_--], captures_[mx_ind]);
  }
  return kNullMove;
}

Move MovePicker::YieldMove() {
  if (stage_ == PickerStage::kNone) {
    stage_ = PickerStage::kPrincipalVariation;
    if (pv_move_.has_value() && (pv_move_->is_capture() || !only_captures_)) {
      return pv_move_.value();
    }
  }
  if (ind_ == 0 && stage_ == PickerStage::kPrincipalVariation) {
    move_generator::GenerateMoves<MovesType::kCaptures>(pos_, list_);
    stage_ = PickerStage::kGoodCaptures;
    // list_.RemovePvMove(pv_move_);
  }
  if (ind_ == list_.size() && stage_ == PickerStage::kGoodCaptures && !only_captures_) {
    move_generator::GenerateMoves<MovesType::kQuiets>(pos_, list_);
    stage_ = PickerStage::kQuiets;
    // list_.RemovePvMove(pv_move_);
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
  //* ordering by MVV-LVA (Most Valuable Victim - Least Valuable Aggressor), selection sort
  std::size_t sz = list_.size();
  [[assume(ind_ <= sz)]];
  int mx_diff = CalculateScore(list_[ind_]);
  std::size_t mx_ind = ind_;
  for (std::size_t i = ind_ + 1; i < sz; ++i) {
    int curr_diff = CalculateScore(list_[i]);
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

bool MovePicker::empty2() const {
  return captures_.empty() && quiets_.empty();
}

}// namespace chess