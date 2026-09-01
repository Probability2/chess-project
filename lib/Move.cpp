#include "Move.hpp"

namespace chess {

Move::Move(const uint8_t from, const uint8_t to)
: move_val_(from | (to << 6)) {
}

Move::Move(const uint8_t from, const uint8_t to, const MoveFlag flag) : Move(from, to) {
  move_val_ |= (std::to_underlying(flag) << 12);
}

uint8_t Move::get_from() const {
  return move_val_ & 0x3F;
}

uint8_t Move::get_to() const {
  return (move_val_ >> 6) & 0x3F;
}

MoveFlag Move::get_flag() const {
  return static_cast<MoveFlag>(move_val_ >> 12);
}

PieceBase Move::get_promoted_piece() const {
  return static_cast<PieceBase>((move_val_ & 0x3000) >> 12);
}

bool Move::has_promoted_piece() const {
  return move_val_ & 0x8000;
}

bool Move::is_en_passant() const {
  return get_flag() == MoveFlag::kEpCapture;
}

bool Move::is_castle() const{
  return get_flag() == MoveFlag::kKingCastle || get_flag() == MoveFlag::kQueenCastle;
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

// decltype(auto) MoveList::operator[](this auto& self, const std::size_t ind) {
//   return self.moves_[ind];
// }

Move MoveList::operator[](const std::size_t ind) const {
  return moves_[ind];
}

Move& MoveList::operator[](const std::size_t ind) {
  return moves_[ind];
}

void MoveList::pop_back() {
  [[assume(size_ != 0)]];
  size_--;
}

const Move& MoveList::back() const {
  return moves_[size_ - 1];
}

}// namespace chess