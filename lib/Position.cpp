#include "Position.hpp"

namespace chess {

Move::Move(PieceType piece, uint8_t from, uint8_t to)
: piece_(piece)
, from_(from)
, to_(to) {
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

void MoveList::Push(const Move& move) {
  if (size_ == kMaxMoves) {
    return;
  }
  moves_[size_++] = move;
}

bool Position::is_white_move() const {
  return is_white_move_;
}

bool Position::is_en_passant() const {
  return target_ != 0;
}

std::size_t Position::get_no_capture_moves() const {
  return no_capture_moves_;
}

std::size_t Position::get_move_number() const {
  return move_;
}

std::optional<std::string> Position::get_castle() const {
  if (castle_ == 0) {
    return std::nullopt;
  }
  std::string castling_rights;
  for (std::size_t i = 0; i < kMxCastles; ++i) {
    if ((castle_ >> i) & 1) {
      castling_rights += kCastles[kMxCastles - i - 1];
    }
  }

  return castling_rights;
}

std::optional<std::string> Position::get_en_passant() const {
  if (target_ == 0) {
    return std::nullopt;
  }

  return std::string{static_cast<char>(((target_ >> 3) & 0x07) + 'a'), //file
                     static_cast<char>((target_ & 0x07) + '1')};       //rank
}

MoveList Position::GeneratePseudoMoves() const {
  MoveList list;
  utils::PrintBitboard(pieces_[0]);
  // std::cout << pieces_[0] << " powerful debugger\n";
  Bitboard w_pawns = pieces_[0];
  std::size_t pawns_max_ind = 63;
  while (w_pawns > 0) {
    std::size_t zeros = std::countr_zero(w_pawns);
    pawns_max_ind -= zeros;
    AddPseudoPawnMoves(list, pawns_max_ind);
    w_pawns &= (w_pawns - 1); //Brian Kernighan's algorithm
  }

  return list;
}

void Position::AddPseudoPawnMoves(MoveList& list, std::size_t ind) const {
  if (ind + 8 < kMaxInd && board_[ind + 8] == PieceType::kNone) {// ind = number of the square
    list.Push(Move(PieceType::kWhitePawn, ind, ind + 8));
  }
  if (ind >= 8 && ind <= 15 && board_[ind + 8] == PieceType::kNone && board_[ind + 16] == PieceType::kNone) {
    list.Push(Move(PieceType::kWhitePawn, ind, ind + 16));
  }
  // captures
  if (ind + 7 < kMaxInd && Color(board_[ind + 7]) == ColorType::kBlack && ind % 8 != 0) {
    list.Push(Move(PieceType::kWhitePawn, ind, ind + 7));
  }
  if (ind + 7 && kMaxInd && Color(board_[ind + 9]) == ColorType::kBlack && ind % 8 != 7) {
    list.Push(Move(PieceType::kWhitePawn, ind, ind + 9));
  }
}

MoveList Position::GenerateLegalMoves() const {
  MoveList list;

  return list;
}

}// namespace chess

std::ostream& operator<<(std::ostream& os, const chess::Move& move) {
  os << GetPieceCode(move.piece_) << static_cast<char>('a' + move.from_ & 0x0F)
     << static_cast<char>('1' + (move.from_ >> 4) & 0x0F) << '-' << static_cast<char>('a' + move.to_ & 0x0F)
     << static_cast<char>('1' + (move.to_ >> 4) & 0x0F);
  
  return os;
}

std::ostream& operator<<(std::ostream& os, const chess::MoveList& list) {
  for (auto move: list.AsSpan()) {
    os << move << ", ";
  }

  return os;
}

std::ostream& operator<<(std::ostream& os, const chess::Position& pos) {
  for (int i = chess::kMaxInd - 1; i >= 0; --i) {
    for (int j = 0; j < chess::kMaxInd; ++j) {
      os << GetPieceIcon(pos.get_square(i, j)) << ' ';
    }
    os << '\n';
  }
  os << (pos.is_white_move() ? "White's move, " : "Black's move, ") << pos.get_castle().value_or(" ") << ", ";
  auto en_passant = pos.get_en_passant();
  if (en_passant.has_value()) {
    os << "en-passant: " << *en_passant;
  } else {
    os << "no en-passant";
  }
  os << ", no capture moves: " << pos.get_no_capture_moves() << ", move number: " << pos.get_move_number() << '\n';

  return os;
}

