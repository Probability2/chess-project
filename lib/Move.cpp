#include "Move.hpp"

namespace chess {

Move::Move(const Square from, const Square to)
: move_val_(std::to_underlying(from) | (std::to_underlying(to) << 6)) {
}

Move::Move(const Square from, const Square to, const MoveFlag flag) : Move(from, to) {
  move_val_ |= (std::to_underlying(flag) << 12);
}

bool Move::operator==(std::string_view str) const {
  if (str.length() < 4) {
    return false;// Incorrect input!!
  }
  int file_from = str[0] - 'a';
  int rank_from = str[1] - '1';
  int file_to = str[2] - 'a';
  int rank_to = str[3] - '1';
  if (file_from < 0 || file_from > 8 || rank_from < 0 || rank_from > 8 ||
      file_to < 0 || file_to > 8 || rank_to < 0 || rank_to > 8) {
    return false;// Incorrect input!!
  }
  Square from = coord(rank_from, file_from);
  Square to = coord(rank_to, file_to);
  std::optional<PieceBase> promoted;
  if (str.length() > 4 && std::ranges::find(kPromotedPieces, str[4]) != kPromotedPieces.end()) {
    promoted = static_cast<PieceBase>(1 +
               std::ranges::distance(kPromotedPieces.begin(), std::ranges::find(kPromotedPieces, str[4])));
  }
  if (get_from() == from && get_to() == to &&
     (!promoted || *promoted == get_promoted_base())) {
    return true;
  }

  return false;
}

Move::Move(std::string_view str) {
  if (str.length() < 4) {
    return;// Incorrect input!!
  }
  Square from = coord(str[1] - '1', str[0] - 'a');
  Square to = coord(str[3] - '1', str[2] - 'a');
  if (str.length() == 4) {
    *this = Move(from, to);
  } else {
    *this = Move(from, to, MoveFlag::kQueenPromotion);
  }
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

PieceBase Move::get_promoted_base() const {
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

Move::operator bool() const {
  return move_val_ != 0;
}

MoveEntry::MoveEntry(const Move move)
: Move(std::move(move)) {
}

template<IsMove T>
std::size_t MoveList<T>::size() const {
  return size_;
}

template<IsMove T>
std::span<const T> MoveList<T>::AsSpan() const {
  [[assume(size_ <= kMaxMoves)]];
  return std::span<const T>{moves_}.first(size_);
}

template<IsMove T>
std::span<T> MoveList<T>::AsSpan() {
  [[assume(size_ <= kMaxMoves)]];
  return std::span<T>{moves_}.first(size_);
}

template<IsMove T>
bool MoveList<T>::contains(const T& m) const {
  return std::ranges::contains(this->AsSpan(), m);
}

template<IsMove T>
bool MoveList<T>::empty() const {
  return size_ == 0;
}

template<IsMove T>
void MoveList<T>::push(const T& move) {
  [[assume(size_ != kMaxMoves)]];
  moves_[size_++] = move;
}

template<IsMove T>
void MoveList<T>::pop_back() {
  [[assume(size_ != 0)]];
  size_--;
}

template<IsMove T>
const T& MoveList<T>::back() const {
  [[assume(size_ != 0)]];
  return moves_[size_ - 1];
}

template<IsMove T>
void MoveList<T>::remove(std::size_t ind) {
  [[assume(ind < size_)]];
  moves_[ind] = this->back();
  this->pop_back();
}

template<IsMove T>
void MoveList<T>::RemovePvMove(const std::optional<Move> pv_move) {
  if (!pv_move) {
    return;
  }
  for (const auto& [i, move]: this->AsSpan() | std::views::enumerate) {
    if (static_cast<Move>(move) == *pv_move) {
      // std::cout << "YESSSSSSSAAAAAAAAAAAAAAAAAREMOVE!!!!!!!!\n";
      // std::cout << move << ' ' << i << '\n';
      remove(i);
      break;
    }
  }
}

std::ostream& operator<<(std::ostream& os, const Move& move) {
  const uint8_t from_shift = std::to_underlying(move.get_from());
  const uint8_t to_shift = std::to_underlying(move.get_to());
  os << move.get_from() << static_cast<char>('a' + (to_shift & 7)) << static_cast<char>('1' + (to_shift >> 3 & 7));
  if (move.is_promotion()) {
    os << GetPieceCode(move.get_promoted_base());
  }
  
  return os;
}

std::ostream& operator<<(std::ostream& os, const MoveEntry& move) {
  os << static_cast<const Move>(move) << ", score: " << move.score_;

  return os;
}

// std::ostream& operator<<(std::ostream& os, const MoveList& list) {
//   for (auto move: list.AsSpan()) {
//     os << move << ", ";
//   }

//   return os;
// }

// explicit template instantiation
template class MoveList<Move>;
template class MoveList<MoveEntry>;


}// namespace chess