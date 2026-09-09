#include "Move.hpp"

namespace chess {

Move::Move(const Square from, const Square to)
: move_val_(std::to_underlying(from) | (std::to_underlying(to) << 6)) {
}

Move::Move(const Square from, const Square to, const MoveFlag flag) : Move(from, to) {
  move_val_ |= (std::to_underlying(flag) << 12);
}

Square Move::get_from() const {
  return static_cast<Square>(move_val_ & 0x3F);
}

Square Move::get_to() const {
  return static_cast<Square>((move_val_ >> 6) & 0x3F);
}

MoveFlag Move::get_flag() const {
  return static_cast<MoveFlag>(move_val_ >> 12);
}

PieceBase Move::get_promoted_piece() const {
  return static_cast<PieceBase>(((move_val_ & 0x3000) >> 12) + 1);
}

bool Move::is_capture() const {
  return move_val_ & 0x4000;
}

bool Move::is_double_pawn_push() const {
  return get_flag() == MoveFlag::kDoublePawnPush;
}

bool Move::is_promotion() const {
  return move_val_ & 0x8000;
}

bool Move::is_en_passant() const {
  return get_flag() == MoveFlag::kEpCapture;
}

bool Move::is_castle() const{
  return get_flag() == MoveFlag::kKingCastle || get_flag() == MoveFlag::kQueenCastle;
}

bool Move::is_king_castle() const {
  return get_flag() == MoveFlag::kKingCastle;
}

bool Move::is_queen_castle() const {
  return get_flag() == MoveFlag::kQueenCastle;
}

bool Move::is_50_moves_eligible() const {
  const MoveFlag flag = get_flag();
  return (flag == MoveFlag::kQuiet) || (flag == MoveFlag::kKingCastle) || (flag == MoveFlag::kQueenCastle);
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

bool MoveList::empty() const {
  return size_ == 0;
}

void MoveList::push(const Move& move) {
  assert(size_ < kMaxMoves && "Movelist overflow");
  moves_[size_++] = move;
}

void MoveList::pop_back() {
  [[assume(size_ != 0)]];
  size_--;
}

const Move& MoveList::back() const {
  return moves_[size_ - 1];
}

std::ostream& operator<<(std::ostream& os, const chess::Move& move) {
  const uint8_t from_shift = std::to_underlying(move.get_from());
  const uint8_t to_shift = std::to_underlying(move.get_to());
  os << static_cast<char>('a' + (from_shift & 7))
     << static_cast<char>('1' + (from_shift >> 3 & 7)) << '-' << static_cast<char>('a' + (to_shift & 7))
     << static_cast<char>('1' + (to_shift >> 3 & 7));
  if (move.is_promotion()) {
    os << GetPieceCode(move.get_promoted_piece());
  }
  
  return os;
}

std::ostream& operator<<(std::ostream& os, const chess::MoveList& list) {
  for (auto move: list.AsSpan()) {
    os << move << ", ";
  }

  return os;
}

}// namespace chess