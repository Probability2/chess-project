#include "MovePicker.hpp"

namespace chess {

MovePicker::MovePicker(const Position& pos, const std::array<Move, kMaxKillerMoves>& killers,
             const Move pv_move, const bool only_captures)
: pos_(pos)
, killers_(killers)
, pv_move_(pv_move)
, only_captures_(only_captures) {
}

MovePicker::MovePicker(const Position& pos, const bool only_captures)
: pos_(pos)
, only_captures_(only_captures) {
}

int MovePicker::GetCaptureScore(const Move move) const {
  if (move.is_en_passant()) [[unlikely]] {
    return 0;
  }
  // return eval::kPieceValues[GetPieceBase(pos_.PieceOn(move.get_to()))] -
        //  eval::kPieceValues[GetPieceBase(pos_.PieceOn(move.get_from()))];
  return eval::kMVP_LVA[GetPieceBase(pos_.PieceOn(move.get_to()))][GetPieceBase(pos_.PieceOn(move.get_from()))];
  // return eval::kMVP_LVA2[(std::to_underlying(GetPieceBase(pos_.PieceOn(move.get_to()))) << 3)  +
                          // std::to_underlying(GetPieceBase(pos_.PieceOn(move.get_from())))];
}

int MovePicker::GetQuietScore(const Move move) const {
  if (move == killers_[0]) {
    return 900;
  } else if (move == killers_[1]) {
    return 800;
  }// add here if there are more then 2 killer moves
  if (move.is_promotion()) [[unlikely]] {
    switch (move.promoted_piece()) {
      case PieceBase::kQueen: return 1000;
      case PieceBase::kKnight: return 600;
      default: return 200;// rook and bishop
    }
  } else if (move.is_castle()) [[unlikely]] {
    return 300;
  } else [[likely]] {
    return 100;
  }
}

void MovePicker::ScoreCaptures() {
  for (auto& move: captures_.AsSpan()) {
    move.score_ = GetCaptureScore(move);
  }
}

void MovePicker::ScoreQuiets() {
  for (auto& move: quiets_.AsSpan()) {
    move.score_ = GetQuietScore(move);
  }
}

Move MovePicker::YieldMove2() {
  Move move = YieldMove();
  if (move) {
    sz_++;
  }

  return move;
}

Move MovePicker::YieldMove() {
  switch (stage_) {
  case PickerStage::kPrincipalVariation:
    ++stage_;
    if (pv_move_ && (pv_move_.is_capture() || !only_captures_)) {
      return pv_move_;
    }
    [[fallthrough]];
  case PickerStage::kGenCaptures:
    move_generator::GenerateMoves<MovesType::kCaptures>(pos_, captures_);
    captures_.RemovePvMove(pv_move_);
    // ScoreCaptures();
    // ScoreMoves(captures_, [this](Move m){ return GetCaptureScore(m);});
    ScoreCaptures();
    right_ = captures_.size();
    ++stage_;
    [[fallthrough]];
  case PickerStage::kGoodCaptures:
    while (left_ < right_) {
      Move capture = YieldGoodCapture();
      if (capture && move_generator::IsLegalV(pos_, capture)) {
        return capture;
      }
    }
    left_ = 0;
    ++stage_;
    [[fallthrough]];
  case PickerStage::kGenQuiets:
    if (only_captures_) {
      return kNullMove;
    }
    move_generator::GenerateMoves<MovesType::kQuiets>(pos_, quiets_);
    quiets_.RemovePvMove(pv_move_);
    ScoreQuiets();
    // ScoreMoves(quiets_, [this](Move m){ return GetQuietScore(m);});
    ++stage_;
    [[fallthrough]];
  case PickerStage::kQuiets:
    while (ind_ < quiets_.size()) {
      Move quiet = YieldQuiet();
      if (move_generator::IsLegalV(pos_, quiet)) {
        return quiet;
      }
    }
    ++stage_;
    [[fallthrough]];
  case PickerStage::kBadCaptures:
    while (left_ < bad_captures_.size()) {
      Move capture = bad_captures_[left_++];
      if (move_generator::IsLegalV(pos_, capture)) {
        return capture;
      }
    }
    return kNullMove;
  }
  std::unreachable();
}

std::size_t MovePicker::IndMxScore(const MoveList<MoveEntry>& list, const std::size_t left,
                                                                    const std::size_t right) {
  int mx_score = list[left].score_;
  int mx_ind = left;
  for (std::size_t i = left + 1; i < right; ++i) {
    int score = list[i].score_;
    if (score > mx_score) {
      mx_score = score;
      mx_ind = i;
    }
  }

  return mx_ind;
}

Move MovePicker::YieldQuiet() {
  int mx_ind = IndMxScore(quiets_, ind_, quiets_.size());
  std::swap(quiets_[ind_], quiets_[mx_ind]);

  return quiets_[ind_++];
}

Move MovePicker::YieldGoodCapture() {
  //* ordering by MVV-LVA (Most Valuable Victim - Least Valuable Aggressor), selection sort
  while (left_ < right_) {
    std::size_t mx_ind = IndMxScore(captures_, left_, right_);
    if (pos_.IsGoodCapture(captures_[mx_ind])) {
      std::swap(captures_[left_], captures_[mx_ind]);
      return captures_[left_++];
    }
    bad_captures_.push(captures_[mx_ind]);
    std::swap(captures_[--right_], captures_[mx_ind]);
  }

  return kNullMove;
}

bool MovePicker::empty() const {
  return sz_ == 0;
}

std::size_t MovePicker::size() const {
  return sz_;
}

}// namespace chess