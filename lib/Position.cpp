#include "Position.hpp"
#include "MoveGenerator.hpp"

namespace chess {

bool Position::is_white_move() const noexcept {
  return side_to_move_ == ColorType::kWhite;
}

bool Position::is_en_passant() const noexcept {
  return en_passant_ != Square::kNone;
}

std::size_t Position::get_no_capture_moves() const noexcept {
  return no_capture_moves_;
}

std::size_t Position::get_move_number() const noexcept {
  return move_;
}

Bitboard Position::get_all_pieces() const noexcept {
  return white_pieces_ | black_pieces_;
}

Bitboard Position::get_white_pieces() const noexcept {
  return white_pieces_;
}

Bitboard Position::get_black_pieces() const noexcept {
  return black_pieces_;
}

Bitboard Position::get_piece_metric(const PieceType piece) const {
  return pieces_[std::to_underlying(piece) - 1];
}

uint8_t Position::get_castles() const noexcept {
  return castles_;
}

Square Position::get_en_passant() const noexcept {
  return en_passant_;
}

std::string Position::get_castling_notation() const noexcept {
  std::string notation;
  for (int i = chess::kMxCastles - 1; i >= 0; --i) {
    if ((castles_ >> i) & 1) {
      notation += chess::kCastleChars[chess::kMxCastles - i - 1];
    }
  }
  if (notation.empty()) {
    return "no castling rights";
  }

  return notation;
}


template<MovesType Type>
MoveList Position::GenerateMoves() {
  CalculatePinnedPieces();
  info_.king_attackers_ = GetSquareAttackers(GetLSB(get_piece_metric(PieceBase::kKing & side_to_move_)),
                                                                                         Square::kNone);
  
  return move_generator::GenerateMoves<Type>(*this);
}

bool Position::is_single_check() const noexcept {
  return std::popcount(info_.king_attackers_) == 1;
}

bool Position::is_double_check() const noexcept {
  return std::popcount(info_.king_attackers_) > 1;
}

bool Position::is_check() const noexcept {
  return std::popcount(info_.king_attackers_) != 0;
}

bool Position::is_pawn(const Square sq) const noexcept {
  return (board_[sq] == PieceType::kWhitePawn || board_[sq] == PieceType::kBlackPawn);
}

bool Position::is_knight(const Square sq) const noexcept {
  return (board_[sq] == PieceType::kWhiteKnight || board_[sq] == PieceType::kBlackKnight);
}

bool Position::is_bishop(const Square sq) const noexcept {
  return (board_[sq] == PieceType::kWhiteBishop || board_[sq] == PieceType::kBlackBishop);
}

bool Position::is_rook(const Square sq) const noexcept {
  return (board_[sq] == PieceType::kWhiteRook || board_[sq] == PieceType::kBlackRook);
}

bool Position::is_queen(const Square sq) const noexcept {
  return (board_[sq] == PieceType::kWhiteQueen || board_[sq] == PieceType::kBlackQueen);
}

bool Position::is_king(const Square sq) const noexcept {
  return (board_[sq] == PieceType::kWhiteKing || board_[sq] == PieceType::kBlackKing);
}

namespace internal {
FlippedPosition::FlippedPosition(const Position& pos)
: pos_(pos) {
}

}// namespace chess::internal

Bitboard Position::get_pinned_pieces() const {
  return info_.pinned_pieces_;
}

Bitboard Position::get_king_attackers() const {
  return info_.king_attackers_;
}

Bitboard Position::GetSquareAttackers(const Square sq, const Square cleared) const {
  [[assume(sq != Square::kNone)]];
  Bitboard attackers = 0;
  const Bitboard blockers = (cleared == Square::kNone) ? get_all_pieces() : get_all_pieces() & ~ToBB(cleared);
  const Bitboard own_pieces = (side_to_move_ == ColorType::kWhite) ? white_pieces_ : black_pieces_;
  Bitboard king_rook_attacks = ~own_pieces & attacks::SlidingAttacks<PieceBase::kRook>(sq, blockers);
  Bitboard king_bishop_attacks = ~own_pieces & attacks::SlidingAttacks<PieceBase::kBishop>(sq, blockers);
  attackers |= (attacks::kAttacks<PieceBase::kPawn>[!side_to_move_, sq] &
                get_piece_metric(PieceBase::kPawn & !side_to_move_));// pawn attacks
  attackers |= (attacks::kAttacks<PieceBase::kKnight>[sq] &
                get_piece_metric(PieceBase::kKnight & !side_to_move_));// knight attacks
  attackers |= (king_rook_attacks & get_piece_metric(PieceBase::kRook & !side_to_move_));// rook attacks
  attackers |= (king_bishop_attacks & get_piece_metric(PieceBase::kBishop & !side_to_move_));// bishop attacks
  attackers |= (king_rook_attacks & get_piece_metric(PieceBase::kQueen & !side_to_move_));// queen attacks
  attackers |= (king_bishop_attacks & get_piece_metric(PieceBase::kQueen & !side_to_move_));

  return attackers;
}

template<PieceBase Base>
Bitboard Position::GetPinsBySlidingPiece(const Square king_sq, const Bitboard own_pieces,
                                                                   const Bitboard pieces) const {
  Bitboard pinned_pieces = 0;
  BitLooping(pieces, [&pinned_pieces, own_pieces, king_sq](const Square sq) {
    Bitboard line = own_pieces & kBetween[king_sq, sq];
    if (std::popcount(line) == 1) {
      pinned_pieces |= line;
    }
  });

  return pinned_pieces;
}

void Position::CalculatePinnedPieces() {
  info_.pinned_pieces_ = 0;
  Square king_sq = GetLSB(get_piece_metric(PieceBase::kKing & side_to_move_));
  const Bitboard own_pieces = (side_to_move_ == ColorType::kWhite) ? white_pieces_ : black_pieces_;
  info_.pinned_pieces_ |= GetPinsBySlidingPiece<PieceBase::kRook>(king_sq, own_pieces,
                                  get_piece_metric(PieceBase::kRook & !side_to_move_));
  info_.pinned_pieces_ |= GetPinsBySlidingPiece<PieceBase::kBishop>(king_sq, own_pieces,
                                  get_piece_metric(PieceBase::kBishop & !side_to_move_));
  info_.pinned_pieces_ |= GetPinsBySlidingPiece<PieceBase::kRook>(king_sq, own_pieces,
                                  get_piece_metric(PieceBase::kQueen & !side_to_move_));
  info_.pinned_pieces_ |= GetPinsBySlidingPiece<PieceBase::kBishop>(king_sq, own_pieces,
                                  get_piece_metric(PieceBase::kQueen & !side_to_move_));
}

// explicit template instantiation
template MoveList Position::GenerateMoves<MovesType::kPseudo>();
template MoveList Position::GenerateMoves<MovesType::kLegal>();
template MoveList Position::GenerateMoves<MovesType::kCaptures>();
template MoveList Position::GenerateMoves<MovesType::kChecks>();
template MoveList Position::GenerateMoves<MovesType::kEvasions>();

}// namespace chess


namespace {

void PrintPositionDetails(std::ostream& os, const chess::Position& pos) {
  os << (pos.is_white_move() ? "White's move, " : "Black's move, ") << pos.get_castling_notation() << ", ";
  if (pos.is_en_passant() > 0) {
    os << chess::get_notation(std::to_underlying(pos.get_en_passant()));
  } else {
    os << "no en-passant";
  }
  os << ", no capture moves: " << pos.get_no_capture_moves() << ", move number: " << pos.get_move_number() << '\n';
}
}

std::ostream& operator<<(std::ostream& os, const chess::Move& move) {
  const uint8_t from_shift = std::to_underlying(move.get_from());
  const uint8_t to_shift = std::to_underlying(move.get_to());
  os << static_cast<char>('a' + (from_shift & 7))
     << static_cast<char>('1' + (from_shift >> 3 & 7)) << '-' << static_cast<char>('a' + (to_shift & 7))
     << static_cast<char>('1' + (to_shift >> 3 & 7));
  if (move.has_promoted_piece()) {
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

std::ostream& operator<<(std::ostream& os, const chess::Position& pos) {
  for (int i = chess::kMaxInd - 1; i >= 0; --i) {
    for (int j = 0; j < chess::kMaxInd; ++j) {
      os << chess::GetPieceIcon(pos.get_piece(i, j)) << ' ';
    }
    os << '\n';
  }
  PrintPositionDetails(os, pos);

  return os;
}

std::ostream& operator<<(std::ostream& os, const chess::internal::FlippedPosition& flipped_pos) {
  for (std::size_t i = 0; i < chess::kMaxInd; ++i) {
    for (int j = chess::kMaxInd - 1; j >= 0; --j) {
      os << chess::GetPieceIcon(flipped_pos.pos_.get_piece(i, j)) << ' ';
    }
    os << '\n';
  }
  PrintPositionDetails(os, flipped_pos.pos_);

  return os;
}

chess::internal::FlippedPosition flipped(const chess::Position& pos) {
  return chess::internal::FlippedPosition(pos);
}