#pragma once

#include <gtest/gtest_prod.h>

#include "../core/utils.hpp"
#include "Piece.hpp"

#include <bit>
#include <cassert>
#include <iostream>
#include <optional>
#include <span>
#include <type_traits>


namespace chess {

inline constexpr int kBoardSize = 64;

inline constexpr int kMxCastles = 4;

inline constexpr std::size_t kMaxMoves = 256;

inline constexpr std::array<char, kMxCastles> kCastles = {'K', 'Q', 'k', 'q'};

enum class MovesType: uint8_t {
  kPseudo, kLegal, kCaptures, kChecks
};

struct Move {
  Move() = default;

  Move(const PieceType piece, const uint8_t from, const uint8_t to);

  Move(const PieceType piece, const uint8_t from, const uint8_t to, const PieceType promoted_piece);

  bool operator==(const Move& other) const = default;

  PieceType piece_; // the piece that has been moved
  uint8_t from_;// a-h files, 1-8 ranks
  uint8_t to_;// the same thing
  PieceType promoted_piece_ = PieceType::kNone;
};

class MoveList {
public:
  MoveList() = default;

  std::size_t size() const;

  void push(const Move& move);
  
  std::span<const Move> AsSpan() const;

  bool contains(const Move& move) const;

private:
  std::array<Move, kMaxMoves> moves_;
  std::size_t size_ = 0;
};

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

  bool is_white_move() const;

  bool is_en_passant() const;
  
  std::size_t get_no_capture_moves() const;

  std::size_t get_move_number() const;

  std::optional<std::string> get_castle() const;

  std::optional<std::string> get_en_passant() const;

  constexpr void set_castling(const int position) {
    castle_ |= (1 << position);
  }

  constexpr void set_white_move(const bool move) {
    is_white_move_ = move;
  }

  constexpr void set_en_passant(const int ind) {
    target_ = ind;
  }

  constexpr void set_no_captures(const std::size_t moves) {
    no_capture_moves_ = moves;
  }

  constexpr void set_move_number(const std::size_t moves) {
    move_ = moves;
  }
  
  
  
  template<MovesType type>
  MoveList GenerateMoves() const;
  
private:
  std::array<PieceType, kBoardSize> board_{};
  std::array<Bitboard, kPieceCount> pieces_{};
  Bitboard all_white_pieces_ = 0;
  Bitboard all_black_pieces_ = 0;
  bool is_white_move_ = true;
  uint8_t castle_ = 0;
  uint8_t target_ = 0;
  std::size_t no_capture_moves_ = 0;
  std::size_t move_ = 0;

  MoveList GeneratePseudoMoves() const;

  MoveList GenerateLegalMoves() const;

  template<ColorType Color>
  PieceType GetPawnsType() const;

  template<ColorType Color>
  Bitboard GetSinglePawnPush(const Bitboard pawns) const;

  template<ColorType Color>
  void GeneratePawnPushes(MoveList& list, Bitboard pawns, const int shift) const;

  template<ColorType Color>
  void GeneratePawnMoves(MoveList& list) const;

  template<ColorType Color>
  void GeneratePawnCaptures(MoveList& list, Bitboard pawns, const int shift) const;

  template<ColorType Color>
  void GeneratePawnPromotions(MoveList& list, Bitboard pawns, const int shift) const;
  
  FRIEND_TEST(PseudoMovesSuite, Pawns);
};


}// namespace chess

std::ostream& operator<<(std::ostream& os, const chess::Move& list);

std::ostream& operator<<(std::ostream& os, const chess::MoveList& list);

std::ostream& operator<<(std::ostream& os, const chess::Position& pos);
