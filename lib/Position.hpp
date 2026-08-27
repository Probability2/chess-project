#pragma once

#include "../core/utils.hpp"
#include "types/Bitboard.hpp"
#include "Move.hpp"

#include <bit>
#include <iostream>
#include <optional>
#include <type_traits>
#include <utility>

namespace chess {

inline constexpr uint8_t kMxCastles = 4;

inline constexpr std::array<char, kMxCastles> kCastles = {'K', 'Q', 'k', 'q'};

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
    Bitboard mask = 1ULL << sq;
    if (board_[sq] != PieceType::kNone) {
      pieces_[std::to_underlying(board_[sq]) - 1] &= ~mask;
      all_white_pieces_ &= ~mask;
      all_black_pieces_ &= ~mask;
    }
    board_[sq] = piece;
    if (piece == PieceType::kNone) {
      return;
    }
    if (Color(piece) == ColorType::kWhite) {
      all_white_pieces_ |= mask;
    } else {
      all_black_pieces_ |= mask;
    }
    pieces_[std::to_underlying(piece) - 1] |= mask;
  }

  constexpr PieceType get_piece(const int x, const int y) const {
    return board_[coord(x, y)];
  }

  constexpr PieceType get_piece(const uint8_t sq) const {
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
  MoveList GenerateMoves() const;

  bool is_white_move() const;
  bool is_en_passant() const;
  std::size_t get_no_capture_moves() const;
  std::size_t get_move_number() const;
  Bitboard get_all_pieces() const;
  Bitboard get_all_white_pieces() const;
  Bitboard get_all_black_pieces() const;
  Bitboard get_piece_metric(const PieceType piece) const;
  uint8_t get_castles() const;
  uint8_t get_en_passant() const;
  std::string get_castling_notation() const;

  bool is_pawn(const uint8_t sq) const;// for tests only
  bool is_knight(const uint8_t sq) const;// for tests only
  bool is_bishop(const uint8_t sq) const;// for tests only
  bool is_rook(const uint8_t sq) const; // for tests only
  bool is_queen(const uint8_t sq) const;// for tests only
  bool is_king(const uint8_t) const;// for tests only
  
  private:
  std::array<PieceType, kBoardSize> board_{};
  std::array<Bitboard, kPieceCount> pieces_{};
  Bitboard all_white_pieces_ = 0;
  Bitboard all_black_pieces_ = 0;
  ColorType side_to_move_;
  uint8_t castles_ = 0;
  uint8_t en_passant_ = kBoardSize;
  std::size_t no_capture_moves_ = 0;
  std::size_t move_ = 1;
  
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