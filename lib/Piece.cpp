#include "Piece.hpp"

namespace chess {

Move::Move(const PieceType piece, const uint8_t from, const uint8_t to)
: piece_(piece)
, from_(from)
, to_(to) {
}

Move::Move(const PieceType piece, const uint8_t from, const uint8_t to,
           const PieceType promoted_piece): Move(piece, from, to) {
  promoted_piece_ = promoted_piece;            
}

bool Move::has_promoted_piece() const {
  return promoted_piece_ != PieceType::kNone;
}

bool Move::is_pawn() const {
  return (piece_ == PieceType::kWhitePawn || piece_ == PieceType::kBlackPawn);
}

std::size_t MoveList::size() const {
  return size_;
}

std::span<const Move> MoveList::AsSpan() const {
  return std::span<const Move>{moves_}.first(size_);
}

bool MoveList::contains(const Move& m) const {
  for (auto move: this->AsSpan()) {
    if (move == m) {
      return true;
    }
  }

  return false;
}

void MoveList::push(const Move& move) {
  assert(size_ < kMaxMoves && "Movelist overflow");
  moves_[size_++] = move;
}

}// namespace chess