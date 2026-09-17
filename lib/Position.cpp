#include "Position.hpp"
#include "MoveGenerator.hpp"

namespace chess {

namespace {

void PrintPositionDetails(std::ostream& os, const chess::Position& pos) {
  os << (pos.is_white_move() ? "White's move, " : "Black's move, ") << pos.get_castling_notation() << ", ";
  if (pos.is_en_passant()) {
    os << pos.get_en_passant();
  } else {
    os << "no en-passant";
  }
  os << ", no capture moves: " << pos.get_no_capture_moves() << ", move number: " << pos.get_move_number() << '\n';
}
}

bool StateStack::operator==(const StateStack& other) const {
  if (this->size_ != other.size()) {
    return false;
  }
  return std::ranges::equal(AsSpan(), other.AsSpan());
}

void StateStack::push(const InternalInfo& info) {
  [[assume(size_ != kMaxHalfMoves)]];
  stack_[size_++] = info;
}

void StateStack::pop() {
  [[assume(size_ != 0)]];
  size_--;
}

InternalInfo StateStack::top() const {
  [[assume(size_ != 0)]];
  return stack_[size_ - 1];
}

std::size_t StateStack::size() const {
  return size_;
}

bool StateStack::empty() const {
  return (size_ == 0);
}

std::span<const InternalInfo> StateStack::AsSpan() const {
  return std::span<const InternalInfo>{stack_}.first(size_);
}

bool Position::is_white_move() const noexcept {
  return side_to_move_ == ColorType::kWhite;
}

bool Position::is_en_passant() const noexcept {
  return info_.en_passant_ != Square::kNone;
}

std::size_t Position::get_no_capture_moves() const noexcept {
  return info_.no_capture_moves_;
}

std::size_t Position::get_move_number() const noexcept {
  return (halfmoves_ + 1) / 2;
}

Bitboard Position::get_all_pieces() const noexcept {
  return all_pieces_[ColorType::kWhite] | all_pieces_[ColorType::kBlack];
}

Bitboard Position::get_white_pieces() const noexcept {
  return all_pieces_[ColorType::kWhite];
}

Bitboard Position::get_black_pieces() const noexcept {
  return all_pieces_[ColorType::kBlack];
}

Bitboard Position::get_pieces(ColorType color) const noexcept {
  return all_pieces_[color];
}

Bitboard Position::get_piece_metric(const PieceType piece) const noexcept {
  [[assume(piece != PieceType::kNone)]];
  return pieces_[piece];
}

uint8_t Position::get_castles() const noexcept {
  return info_.castling_rights_;
}

Square Position::get_en_passant() const noexcept {
  return info_.en_passant_;
}

ColorType Position::side_to_move() const noexcept {
  return side_to_move_;
}

std::string Position::get_castling_notation() const noexcept {
  std::string notation;
  for (std::size_t i = 0; i < chess::kMxCastles; ++i) {
    if ((info_.castling_rights_ >> i) & 1) {
      notation += chess::kCastleChars[i];
    }
  }
  if (notation.empty()) {
    return "no castling rights";
  }

  return notation;
}

InternalInfo Position::GetInfo() const {
  return info_;
}

StateStack Position::GetStateStack() const {
  return state_stack_;
}

Bitboard Position::get_pawns() const noexcept {
  return pieces_[PieceType::kWhitePawn] | pieces_[PieceType::kBlackPawn];
}

Bitboard Position::get_knights() const noexcept {
  return pieces_[PieceType::kWhiteKnight] | pieces_[PieceType::kBlackKnight];
}

Bitboard Position::get_bishops() const noexcept {
  return pieces_[PieceType::kWhiteBishop] | pieces_[PieceType::kBlackBishop];
}

Bitboard Position::get_rooks() const noexcept {
  return pieces_[PieceType::kWhiteRook] | pieces_[PieceType::kBlackRook];
}

Bitboard Position::get_queens() const noexcept {
  return pieces_[PieceType::kWhiteQueen] | pieces_[PieceType::kBlackQueen];
}

Bitboard Position::get_kings() const noexcept {
  return pieces_[PieceType::kWhiteKing] | pieces_[PieceType::kBlackKing];
}

template<MovesType Type>
void Position::GenerateMoves(MoveList& list) const {
  if (!info_.is_calculated_) {
    CalculatePinnedPieces();
    info_.king_attackers_ = GetSquareAttackers(GetLSB(get_piece_metric(PieceBase::kKing & side_to_move_)), 0, 0);
    info_.is_calculated_ = true;
  }
  move_generator::GenerateMoves<Type>(*this, list);
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

Bitboard Position::GetSquareAttackers(const Square sq, const Bitboard occupied, const Bitboard padding) const {
  [[assume(sq != Square::kNone)]];
  Bitboard attackers = 0;
  const Bitboard blockers = (get_all_pieces() | padding) & ~occupied;
  const Bitboard own_pieces = get_pieces(side_to_move_);
  Bitboard king_rook_attacks = ~own_pieces & attacks::SlidingAttacks<PieceBase::kRook>(sq, blockers);
  Bitboard king_bishop_attacks = ~own_pieces & attacks::SlidingAttacks<PieceBase::kBishop>(sq, blockers);
  attackers |= (attacks::kAttacks<PieceBase::kPawn>[side_to_move_, sq] &
                get_piece_metric(PieceBase::kPawn & !side_to_move_) & ~occupied);
  attackers |= (attacks::kAttacks<PieceBase::kKnight>[sq] &
                get_piece_metric(PieceBase::kKnight & !side_to_move_));
  attackers |= (king_rook_attacks & get_piece_metric(PieceBase::kRook & !side_to_move_));
  attackers |= (king_bishop_attacks & get_piece_metric(PieceBase::kBishop & !side_to_move_));
  attackers |= (king_rook_attacks & get_piece_metric(PieceBase::kQueen & !side_to_move_));
  attackers |= (king_bishop_attacks & get_piece_metric(PieceBase::kQueen & !side_to_move_));
  attackers |= (attacks::kAttacks<PieceBase::kKing>[sq] & get_piece_metric(PieceBase::kKing & !side_to_move_));

  return attackers;
}

bool Position::IsPinned(const Square sq) const noexcept {
  return (info_.pinned_pieces_ & ToBB(sq)) != 0;
}

template<PieceBase Piece> requires attacks::SlidingPiece<Piece>
void Position::GetPinnedBySlidingPiece(const Square king_sq, const Bitboard occupied,
                                                             const Bitboard pieces) noexcept {
  [[assume(king_sq != Square::kNone)]];
  BitLooping(pieces & attacks::SlidingAttacks<Piece>(king_sq, 0), [this, occupied, king_sq](const Square sq) {
    Bitboard line = occupied & kBetween[king_sq, sq];
    if (std::popcount(line) == 1) {
      info_.pinned_pieces_ |= line;
    }
  });
}

void Position::CalculatePinnedPieces() const noexcept {
  const Square king_sq = GetLSB(get_piece_metric(PieceBase::kKing & side_to_move_));
  const Bitboard all_pieces = get_all_pieces();
  const Bitboard own_pieces = get_pieces(side_to_move_);
  info_.pinned_pieces_ = 0;
  const Bitboard pinned_candidates = attacks::SlidingAttacks<PieceBase::kQueen>(king_sq, all_pieces) & own_pieces;
  const Bitboard bishop_rays = attacks::SlidingAttacks<PieceBase::kBishop>(king_sq, all_pieces ^ pinned_candidates);
  const Bitboard rook_rays = attacks::SlidingAttacks<PieceBase::kRook>(king_sq, all_pieces ^ pinned_candidates);
  Bitboard bishop_pinners = bishop_rays & (get_piece_metric(PieceBase::kBishop & !side_to_move_) |
                                           get_piece_metric(PieceBase::kQueen & !side_to_move_));
  Bitboard rook_pinners = rook_rays & (get_piece_metric(PieceBase::kRook & !side_to_move_) |
                                       get_piece_metric(PieceBase::kQueen & !side_to_move_));
  BitLooping(bishop_pinners | rook_pinners, [this, own_pieces, king_sq](const Square sq) {
    info_.pinned_pieces_ |= (kBetween[king_sq, sq] & own_pieces);
  });
}

int Position::GetWhiteScore() const noexcept {
  return info_.score_;
}

inline void Position::UndoRookCastle(const MoveFlag flag) noexcept {
  const int castle_type = (flag == MoveFlag::kKingCastle) ? 0 : 1;
  ClearSquare(kCastleInterSq[castle_type][side_to_move_]);
  PutPiece(PieceBase::kRook & side_to_move_, kRookCastleSquares[castle_type][side_to_move_]);
}

inline void Position::DoRookCastle(const MoveFlag flag) noexcept {
  const PieceType rook = PieceBase::kRook & side_to_move_;
  const int castle_type = (flag == MoveFlag::kKingCastle) ? 0 : 1;
  RemoveScore(rook, kRookCastleSquares[castle_type][side_to_move_]);
  ClearSquare(kRookCastleSquares[castle_type][side_to_move_]);
  PutPiece(rook, kCastleInterSq[castle_type][side_to_move_]);
  AddScore(rook, kCastleInterSq[castle_type][side_to_move_]);
}

inline void Position::UpdateMoveClocks(const Move& move) noexcept {
  halfmoves_++;
  if (move.is_50_moves_eligible() && (board_[move.get_from()] != (PieceBase::kPawn & side_to_move_))) {
    info_.no_capture_moves_++;
  } else {
    info_.no_capture_moves_ = 0;
  }
}

void Position::MakeMove(const Move& move) {
  const Square from = move.get_from();
  const Square to = move.get_to();
  [[assume(from != Square::kNone && to != Square::kNone)]];
  InternalInfo prev_info = info_;
  const PieceType piece = board_[from];
  RemoveScore(piece, from);
  ClearSquare(from);
  if (move.is_castle()) [[unlikely]] {
    DoRookCastle(move.get_flag());
  } else if (move.is_en_passant()) [[unlikely]] {
    const Square inter_sq = (side_to_move_ == ColorType::kWhite) ? to + Direction::kSouth : to + Direction::kNorth;
    RemoveScore(PieceBase::kPawn & !side_to_move_, inter_sq);
    ClearSquare(inter_sq);
  } else if (move.is_capture()) {
    prev_info.captured_piece_ = board_[to];
    RemoveScore(board_[to], to);
    ClearSquare(to);
  }
  state_stack_.push(prev_info);
  UpdateMoveClocks(move);
  info_.en_passant_ = Square::kNone;
  if (Bitboard bb_to = ToBB(to); move.is_double_pawn_push() &&
  (get_piece_metric(PieceBase::kPawn & !side_to_move_) & (ShiftDir(bb_to, Direction::kEast) |
  ShiftDir(bb_to, Direction::kWest)))) {
    info_.en_passant_ = (side_to_move_ == ColorType::kWhite) ? from + Direction::kNorth
                                                             : from + Direction::kSouth;
  }
  info_.castling_rights_ &= (kCastlingRights[from] & kCastlingRights[to]);
  info_.is_calculated_ = false;
  const PieceType new_piece = (move.is_promotion()) ? (move.get_promoted_piece() & side_to_move_) : piece;
  AddScore(new_piece, to);
  PutPiece(new_piece, to);
  side_to_move_ = !side_to_move_;
}

void Position::UnmakeMove(const Move& move) {
  [[assume(halfmoves_ != 0)]];
  side_to_move_ = !side_to_move_;
  halfmoves_--;
  info_ = state_stack_.top();
  state_stack_.pop();
  const Square to = move.get_to();
  PutPiece((move.is_promotion()) ? (PieceBase::kPawn & side_to_move_) : board_[to], move.get_from());
  ClearSquare(to);
  if (move.is_castle()) [[unlikely]] {
    UndoRookCastle(move.get_flag());
  } else if (move.is_en_passant()) [[unlikely]] {
    const Direction shift = (side_to_move_ == ColorType::kWhite) ? Direction::kSouth : Direction::kNorth;
    PutPiece(PieceBase::kPawn & !side_to_move_, to + shift);
  } else if (move.is_capture()) {
    PutPiece(info_.captured_piece_, to);
  }
}

std::ostream& operator<<(std::ostream& os, const chess::Position& pos) {
  for (int i = chess::kMaxInd - 1; i >= 0; --i) {
    for (int j = 0; j < chess::kMaxInd; ++j) {
      os << chess::GetPieceIcon(pos.PieceOn(i, j)) << ' ';
    }
    os << '\n';
  }
  PrintPositionDetails(os, pos);

  return os;
}

std::ostream& operator<<(std::ostream& os, const chess::internal::FlippedPosition& flipped_pos) {
  for (std::size_t i = 0; i < chess::kMaxInd; ++i) {
    for (int j = chess::kMaxInd - 1; j >= 0; --j) {
      os << chess::GetPieceIcon(flipped_pos.pos_.PieceOn(i, j)) << ' ';
    }
    os << '\n';
  }
  PrintPositionDetails(os, flipped_pos.pos_);

  return os;
}

// explicit template instantiation
template void Position::GenerateMoves<MovesType::kPseudo>(MoveList& list) const;
template void Position::GenerateMoves<MovesType::kLegal>(MoveList& list) const;
template void Position::GenerateMoves<MovesType::kCaptures>(MoveList& list) const;
template void Position::GenerateMoves<MovesType::kChecks>(MoveList& list) const;
template void Position::GenerateMoves<MovesType::kEvasions>(MoveList& list) const;
template void Position::GenerateMoves<MovesType::kQuiets>(MoveList& list) const;

}// namespace chess

chess::internal::FlippedPosition flipped(const chess::Position& pos) {
  return chess::internal::FlippedPosition(pos);
}