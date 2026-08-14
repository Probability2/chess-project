#pragma once

// #include <gtest/gtest_prod.h>

#include "../core/utils.hpp"
#include "Piece.hpp"

#include <bit>
#include <iostream>
#include <optional>
#include <type_traits>

namespace chess {

inline constexpr uint8_t kBoardSize = 64;

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
    uint8_t coord = utils::coord(x, y);
    Bitboard mask = 1ULL << coord;
    auto color = Color(piece);
    if (board_[coord] != PieceType::kNone) {
      pieces_[static_cast<int>(board_[coord]) - 1] &= ~mask;
      all_white_pieces_ &= ~mask;
      all_black_pieces_ &= ~mask;
    }
    board_[coord] = piece;
    if (color == ColorType::kWhite) {
      all_white_pieces_ |= mask;
    } else if (color == ColorType::kBlack) {
      all_black_pieces_ |= mask;
    } else {
      return;
    }
    pieces_[static_cast<int>(piece) - 1] |= mask;
  }

  constexpr PieceType get_square(const int x, const int y) const {
    return board_[utils::coord(x, y)];
  }

  constexpr void set_castling(const int position) {
    castle_ |= (1 << position);
  }

  constexpr void set_white_move(const bool move) {
    side_to_move_ = (move) ? ColorType::kWhite : ColorType::kBlack;
  }

  constexpr void set_en_passant(const int ind) {
    en_passant_ = ind;
  }

  constexpr void set_no_captures(const std::size_t moves) {
    no_capture_moves_ = moves;
  }

  constexpr void set_move_number(const std::size_t moves) {
    move_ = moves;
  }
  
  template<MovesType type>
  MoveList GenerateMoves() const;

  bool is_white_move() const;
  bool is_en_passant() const;
  std::size_t get_no_capture_moves() const;
  std::size_t get_move_number() const;
  Bitboard get_all_white_pieces() const;
  Bitboard get_all_black_pieces() const;
  Bitboard get_piece_metric(const PieceType piece) const;
  std::optional<std::string> get_castle() const;
  uint8_t get_en_passant() const;

private:
  std::array<PieceType, kBoardSize> board_{};
  std::array<Bitboard, kPieceCount> pieces_{};
  Bitboard all_white_pieces_ = 0;
  Bitboard all_black_pieces_ = 0;
  ColorType side_to_move_;
  uint8_t castle_ = 0;
  uint8_t en_passant_ = kBoardSize;
  std::size_t no_capture_moves_ = 0;
  std::size_t move_ = 0;
  
  // FRIEND_TEST(PseudoMovesSuite, Pawns);
};


}// namespace chess

std::ostream& operator<<(std::ostream& os, const chess::Move& list);

std::ostream& operator<<(std::ostream& os, const chess::MoveList& list);

std::ostream& operator<<(std::ostream& os, const chess::Position& pos);
