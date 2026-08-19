#include "Position.hpp"
#include "MoveGenerator.hpp"

namespace chess {

bool Position::is_white_move() const {
  return side_to_move_ == ColorType::kWhite;
}

bool Position::is_en_passant() const {
  return en_passant_ != kBoardSize;
}

std::size_t Position::get_no_capture_moves() const {
  return no_capture_moves_;
}

std::size_t Position::get_move_number() const {
  return move_;
}

Bitboard Position::get_all_pieces() const {
  return all_white_pieces_ | all_black_pieces_;
}

Bitboard Position::get_all_white_pieces() const {
  return all_white_pieces_;
}

Bitboard Position::get_all_black_pieces() const {
  return all_black_pieces_;
}

Bitboard Position::get_piece_metric(const PieceType piece) const {
  return pieces_[std::to_underlying(piece) - 1];
}

uint8_t Position::get_castles() const {
  return castles_;
}

std::string Position::get_castling_notation() const {
  std::string notation;
  for (int i = chess::kMxCastles - 1; i >= 0; --i) {
    if ((castles_ >> i) & 1) {
      notation += chess::kCastles[chess::kMxCastles - i - 1];
    }
  }
  if (notation.empty()) {
    return "no castling rights";
  }

  return notation;
}

uint8_t Position::get_en_passant() const {
  return en_passant_;
}

template<MovesType type>
MoveList Position::GenerateMoves() const {
  return move_generator::GenerateMoves<type>(*this);
}

namespace internal {
FlippedPosition::FlippedPosition(const chess::Position& pos)
: pos_(pos) {
}
}// namespace chess::internal


// explicit template instantiation
template MoveList Position::GenerateMoves<MovesType::kPseudo>() const;
template MoveList Position::GenerateMoves<MovesType::kLegal>() const;
template MoveList Position::GenerateMoves<MovesType::kCaptures>() const;
template MoveList Position::GenerateMoves<MovesType::kChecks>() const;

}// namespace chess


namespace {
void PrintPositionDetails(std::ostream& os, const chess::Position& pos) {
  os << (pos.is_white_move() ? "White's move, " : "Black's move, ") << pos.get_castling_notation() << ", ";
  if (pos.is_en_passant() > 0) {
    os << utils::get_notation(pos.get_en_passant());
  } else {
    os << "no en-passant";
  }
  os << ", no capture moves: " << pos.get_no_capture_moves() << ", move number: " << pos.get_move_number() << '\n';
}
}

std::ostream& operator<<(std::ostream& os, const chess::Move& move) {
  os << GetPieceCode(move.piece_) << ':' << static_cast<char>('a' + (move.from_ & 7))
     << static_cast<char>('1' + (move.from_ >> 3 & 7)) << '-' << static_cast<char>('a' + (move.to_ & 7))
     << static_cast<char>('1' + (move.to_ >> 3 & 7));
  if (move.promoted_piece_ != chess::PieceType::kNone) {
    os << GetPieceCode(move.promoted_piece_);
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
      os << GetPieceIcon(pos.get_square(i, j)) << ' ';
    }
    os << '\n';
  }
  PrintPositionDetails(os, pos);

  return os;
}

std::ostream& operator<<(std::ostream& os, const chess::internal::FlippedPosition& flipped_pos) {
  for (std::size_t i = 0; i < chess::kMaxInd; ++i) {
    for (int j = chess::kMaxInd - 1; j >= 0; --j) {
      os << GetPieceIcon(flipped_pos.pos_.get_square(i, j)) << ' ';
    }
    os << '\n';
  }
  PrintPositionDetails(os, flipped_pos.pos_);

  return os;
}

chess::internal::FlippedPosition flipped(const chess::Position& pos) {
  return chess::internal::FlippedPosition(pos);
}