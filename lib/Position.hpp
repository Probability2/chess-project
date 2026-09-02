#pragma once

// #include "../core/utils.hpp"
#include "types/Attacks.hpp"
#include "types/Bitboard.hpp"
#include "Move.hpp"

#include <bit>
#include <iostream>
#include <optional>
#include <type_traits>

namespace chess {

inline constexpr uint8_t kMxCastles = 4;

inline constexpr std::array<char, kMxCastles> kCastleChars = {'K', 'Q', 'k', 'q'};

class Position {
public:
  constexpr Position() = default;

  constexpr void set_squares(PieceType p, const std::size_t x, const std::size_t y, const std::size_t n) {
    for (std::size_t i = 0; i < n; ++i) {
      set_square(p, x, y + i);
    }
  }

  //A1 = 0, H8 = 63
  constexpr void set_square(const PieceType piece, const std::size_t x, const std::size_t y) {
    uint8_t sq = coord(x, y);
    Bitboard mask = ToBB(sq);
    if (board_[sq] != PieceType::kNone) {
      pieces_[std::to_underlying(board_[sq]) - 1] &= ~mask;
      white_pieces_ &= ~mask;
      black_pieces_ &= ~mask;
    }
    board_[sq] = piece;
    if (piece == PieceType::kNone) {
      return;
    }
    if (Color(piece) == ColorType::kWhite) {
      white_pieces_ |= mask;
    } else {
      black_pieces_ |= mask;
    }
    pieces_[static_cast<int>(piece) - 1] |= mask;
  }

  constexpr PieceType get_piece(const int x, const int y) const noexcept {
    return board_[coord(x, y)];
  }

  constexpr PieceType get_piece(const Square sq) const noexcept {
    return board_[sq];
  }

  constexpr void set_castling(const uint8_t position) {
    castles_ |= (1 << position);
  }

  constexpr void set_white_move(const bool move) {
    side_to_move_ = (move) ? ColorType::kWhite : ColorType::kBlack;
  }

  constexpr void set_en_passant(const int ind) {
    en_passant_ = ind;
  }

  constexpr void set_no_captures(const int moves) {
    assert(moves >= 0 && "No capture moves number is not valid");
    no_capture_moves_ = moves;
  }

  constexpr void set_move_number(const int moves) {
    assert(moves >= 0 && "Move number is not valid");
    move_ = moves;
  }
  
  template<MovesType Type>
  MoveList GenerateMoves();

  bool is_white_move() const noexcept;
  bool is_en_passant() const noexcept;
  std::size_t get_no_capture_moves() const noexcept;
  std::size_t get_move_number() const noexcept;
  Bitboard get_all_pieces() const noexcept;
  Bitboard get_white_pieces() const noexcept;
  Bitboard get_black_pieces() const noexcept;
  Bitboard get_piece_metric(const PieceType piece) const;
  uint8_t get_castles() const noexcept;
  uint8_t get_en_passant() const noexcept;
  std::string get_castling_notation() const noexcept;

  bool is_pawn(const Square sq) const noexcept;// for tests only
  bool is_knight(const Square sq) const noexcept;// for tests only
  bool is_bishop(const Square sq) const noexcept;// for tests only
  bool is_rook(const Square sq) const noexcept; // for tests only
  bool is_queen(const Square sq) const noexcept;// for tests only
  bool is_king(const Square) const noexcept;// for tests only

  Bitboard get_king_attackers() const;
  Bitboard get_pinned_pieces() const;

  Bitboard GetSquareAttackers(const Square sq, const Square cleared) const;
  bool is_single_check() const noexcept;
  bool is_double_check() const noexcept;
  bool is_check() const noexcept;

private:
  std::array<PieceType, kBoardSize> board_{};
  std::array<Bitboard, kPieceCount> pieces_{};
  Bitboard white_pieces_ = 0;
  Bitboard black_pieces_ = 0;
  ColorType side_to_move_;
  uint8_t castles_ = 0;
  uint8_t en_passant_ = kBoardSize;
  std::size_t no_capture_moves_ = 0;
  std::size_t move_ = 1;

  template<PieceBase Base>
  Bitboard GetPinsBySlidingPiece(const uint8_t king_sq, const Bitboard own_pieces, const Bitboard pieces) const;
  
  void CalculatePinnedPieces();

  struct InternalInfo {
    Bitboard pinned_pieces_;
    Bitboard king_attackers_;
  };

  InternalInfo info_;
  
  // FRIEND_TEST(PseudoMovesSuite, Pawns);
};

namespace internal {
struct FlippedPosition {
  FlippedPosition(const chess::Position& pos);

  const chess::Position& pos_;
};
}// namespace chess::internal

}// namespace chess

std::ostream& operator<<(std::ostream& os, const chess::Move& list);

std::ostream& operator<<(std::ostream& os, const chess::MoveList& list);

std::ostream& operator<<(std::ostream& os, const chess::Position& pos);

std::ostream& operator<<(std::ostream& os, const chess::internal::FlippedPosition& flipped_pos);

chess::internal::FlippedPosition flipped(const chess::Position& pos);