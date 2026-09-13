#include "MovePicker.hpp"

namespace chess {

MovePicker::MovePicker(const Position& pos)
: pos_(pos) {
}

MovePicker::MovePicker(const Position& pos, const bool only_captures)
: pos_(pos)
, only_captures_(only_captures) {
}

bool MovePicker::has_next() {
  if (ind_ == list_.size() && !is_captures_) {
    pos_.GenerateMoves<MovesType::kCaptures>(list_);
    is_captures_ = true;
  }
  if (ind_ == list_.size() && !is_quiets_ && !only_captures_) {
    pos_.GenerateMoves<MovesType::kQuiets>(list_);
    is_quiets_ = true;
  }

  return ind_ < list_.size();
}

int MovePicker::GetDiff(const Move& move) {
  return eval::kPieceValues[GetPieceBase(pos_.PieceOn(move.get_to()))] -
         eval::kPieceValues[GetPieceBase(pos_.PieceOn(move.get_from()))];
}

Move MovePicker::YieldMove() {
  [[assume(ind_ < list_.size())]];
  if (!is_quiets_) {
    SortOutCapture();
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
  return ind_ == 0;
}

}// namespace chess