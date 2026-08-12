#include "Position.hpp"

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
  for (int i = kMxCastles - 1; i >= 0; --i) {
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

template<ColorType Color>
PieceType Position::GetPawnsType() const {
  if (Color == ColorType::kWhite) {
    return PieceType::kWhitePawn;
  }
  return PieceType::kBlackPawn;
}

template<ColorType Color>
Bitboard Position::GetSinglePawnPush(const Bitboard pawns) const {
  if (Color == ColorType::kWhite) {
    return (pawns << 8);
  }

  return (pawns >> 8);
}

template<ColorType Color>
void Position::GeneratePawnPushes(MoveList& list, Bitboard pawns, const int shift) const {
  while (pawns > 0) {
    uint8_t ind = std::countr_zero(pawns);
    list.push(Move(GetPawnsType<Color>(), ind - shift, ind));
    pawns &= (pawns - 1); //Brian Kernighan's algorithm
  }
}

template<ColorType Color>
void Position::GeneratePawnCaptures(MoveList& list, Bitboard pawns, const int shift) const {
  while (pawns > 0) {
    uint8_t ind = std::countr_zero(pawns);
    list.push(Move(GetPawnsType<Color>(), ind - shift, ind));
    pawns &= (pawns - 1); //Brian Kernighan's algorithm
  }
}

template<ColorType Color>
void Position::GeneratePawnPromotions(MoveList& list, Bitboard pawns, const int shift) const {
  while (pawns > 0) {
    uint8_t ind = std::countr_zero(pawns);
    PieceType knight = (Color == ColorType::kWhite) ? PieceType::kWhiteKnight : PieceType::kBlackKnight;
    PieceType bishop = (Color == ColorType::kWhite) ? PieceType::kWhiteBishop : PieceType::kBlackBishop;
    PieceType rook = (Color == ColorType::kWhite) ? PieceType::kWhiteRook : PieceType::kBlackRook;
    PieceType queen = (Color == ColorType::kWhite) ? PieceType::kWhiteQueen : PieceType::kBlackQueen;
    list.push(Move(GetPawnsType<Color>(), ind - shift, ind, knight));
    list.push(Move(GetPawnsType<Color>(), ind - shift, ind, bishop));
    list.push(Move(GetPawnsType<Color>(), ind - shift, ind, rook));
    list.push(Move(GetPawnsType<Color>(), ind - shift, ind, queen));
    pawns &= (pawns - 1); //Brian Kernighan's algorithm
  }
}

template<ColorType Color>
void Position::GeneratePawnMoves(MoveList& list) const {
  Bitboard empty_squares = ~(all_white_pieces_ | all_black_pieces_);
  Bitboard pawns = (Color == ColorType::kWhite) ? pieces_[static_cast<int>(PieceType::kWhitePawn) - 1]
                                                :  pieces_[static_cast<int>(PieceType::kBlackPawn) - 1];
  Bitboard single_shift = GetSinglePawnPush<Color>(pawns);
  Bitboard left_shift = (Color == ColorType::kWhite) ? (pawns << 7) : (pawns >> 7);
  Bitboard right_shift = (Color == ColorType::kWhite) ? (pawns << 9) : (pawns >> 9);
  constexpr Bitboard double_rank_mask = (Color == ColorType::kWhite) ? 0x00000000FF000000ULL
                                                                     : 0x000000FF00000000ULL;
  constexpr Bitboard last_rank_mask = (Color == ColorType::kWhite) ? 0xFF00000000000000ULL
                                                                   : 0x00000000000000FFULL;
  constexpr int board_single_shift = (Color == ColorType::kWhite) ? 8 : -8;
  constexpr Bitboard not_a_file = 0xFEFEFEFEFEFEFEFEULL;
  constexpr Bitboard not_h_file = 0x7F7F7F7F7F7F7F7FULL;
  Bitboard able_to_push = single_shift & empty_squares & ~last_rank_mask;
  Bitboard able_to_push_twice = double_rank_mask & GetSinglePawnPush<Color>(able_to_push) & empty_squares;
  Bitboard able_to_left_capture = (Color == ColorType::kWhite) ? (left_shift & not_a_file & all_black_pieces_)
                                                               : (left_shift & not_h_file & all_white_pieces_);
  Bitboard able_to_right_capture = (Color == ColorType::kWhite) ? (right_shift & not_h_file & all_black_pieces_)
                                                                : (right_shift & not_a_file & all_white_pieces_);
  GeneratePawnPushes<Color>(list, able_to_push, board_single_shift);
  GeneratePawnPushes<Color>(list, able_to_push_twice, 2 * board_single_shift);
  GeneratePawnPromotions<Color>(list, single_shift & empty_squares & last_rank_mask, board_single_shift);
  GeneratePawnCaptures<Color>(list, able_to_left_capture, board_single_shift - 1);
  GeneratePawnCaptures<Color>(list, able_to_right_capture, board_single_shift + 1);
}

MoveList Position::GeneratePseudoMoves() const {
  MoveList list;
  GeneratePawnMoves<ColorType::kWhite>(list);
  GeneratePawnMoves<ColorType::kBlack>(list);

  return list;
}

MoveList Position::GenerateLegalMoves() const {
  MoveList list = GeneratePseudoMoves();

  return list;
}

template<MovesType type>
MoveList Position::GenerateMoves() const {
  if constexpr (type == MovesType::kPseudo) {
    return GeneratePseudoMoves();
  } else {
    return GenerateLegalMoves();
  }
}

// explicit template instantiation
template MoveList Position::GenerateMoves<MovesType::kPseudo>() const;
template MoveList Position::GenerateMoves<MovesType::kLegal>() const;
template MoveList Position::GenerateMoves<MovesType::kCaptures>() const;
template MoveList Position::GenerateMoves<MovesType::kChecks>() const;

}// namespace chess

std::ostream& operator<<(std::ostream& os, const chess::Move& move) {
  os << GetPieceCode(move.piece_) << ':' << static_cast<char>('a' + (move.from_ & 7))
     << static_cast<char>('1' + (move.from_ >> 3 & 7)) << '-' << static_cast<char>('a' + (move.to_ & 7))
     << static_cast<char>('1' + (move.to_ >> 3 & 7));
  
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

