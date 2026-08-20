#include "Move.hpp"

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

bool Move::is_knight() const {
  return (piece_ == PieceType::kWhiteKnight || piece_ == PieceType::kBlackKnight);
}

bool Move::is_bishop() const {
  return (piece_ == PieceType::kWhiteBishop || piece_ == PieceType::kBlackBishop);
}

bool Move::is_rook() const {
  return (piece_ == PieceType::kWhiteRook || piece_ == PieceType::kBlackRook);
}

bool Move::is_queen() const {
  return (piece_ == PieceType::kWhiteQueen || piece_ == PieceType::kBlackQueen);
}

bool Move::is_king() const {
  return (piece_ == PieceType::kWhiteKing || piece_ == PieceType::kBlackKing);
}

std::size_t MoveList::size() const {
  return size_;
}

std::span<const Move> MoveList::AsSpan() const {
  return std::span<const Move>{moves_}.first(size_);
}

bool MoveList::contains(const Move& m) const {
  return std::ranges::contains(this->AsSpan(), m);
}

void MoveList::push(const Move& move) {
  assert(size_ < kMaxMoves && "Movelist overflow");
  moves_[size_++] = move;
}

}// namespace chess