#pragma once

// #include "../core/utils.hpp"
#include "types/Attacks.hpp"
#include "types/Bitboard.hpp"
#include "Move.hpp"

#include <bit>
#include <iostream>
#include <expected>
#include <type_traits>
#include <ranges>
#include <span>

namespace chess {

inline constexpr uint8_t kMxCastles = 4;

inline constexpr std::array<char, kMxCastles> kCastleChars = {'K', 'Q', 'k', 'q'};

constexpr int kMaxHalfMoves = 512;

struct InternalInfo {
  bool operator==(const InternalInfo& other) const = default;

  Square en_passant_ = Square::kNone;
  PieceType captured_piece_ = PieceType::kNone;
  uint8_t castling_rights_ = 0;
  std::size_t no_capture_moves_ = 0;
  Bitboard pinned_pieces_ = 0;
  Bitboard king_attackers_ = 0;
};

class StateStack {
public:
  bool operator==(const StateStack& other) const;
  void push(const InternalInfo& info);
  void pop();
  InternalInfo top() const;
  std::size_t size() const;
  bool empty() const;
  std::span<const InternalInfo> AsSpan() const;

private:
  std::array<InternalInfo, kMaxHalfMoves> stack_{};
  std::size_t size_ = 0;
};

class Position {
public:
  constexpr Position() = default;

  bool operator==(const Position& other) const = default;

  constexpr void SetSquares(PieceType p, const std::size_t x, const std::size_t y, const std::size_t n) {
    for (std::size_t i = 0; i < n; ++i) {
      SetSquare(p, x, y + i);
    }
  }

  //A1 = 0, H8 = 63
  constexpr void SetSquare(const PieceType piece, const std::size_t x, const std::size_t y) {
    Square sq = coord(x, y);
    if (piece == PieceType::kNone) {
      board_[sq] = piece;
      return;
    }
    PutPiece(piece, sq);
  }

  inline constexpr void ClearSquare(const Square sq) {
    if (board_[sq] == PieceType::kNone) {
      return;
    }
    const Bitboard mask = ToBB(sq);
    PieceOccupied(board_[sq]) &= ~mask;
    all_pieces_[std::to_underlying(Color(board_[sq]))] &= ~mask;
    board_[sq] = PieceType::kNone;
  }

  inline constexpr PieceType PieceOn(const int x, const int y) const noexcept {
    return board_[coord(x, y)];
  }

  inline constexpr PieceType PieceOn(const Square sq) const noexcept {
    return board_[sq];
  }

  constexpr void set_castling(const uint8_t position) {
    info_.castling_rights_ |= (1 << position);
  }

  constexpr void set_white_move(const bool move) {
    side_to_move_ = (move) ? ColorType::kWhite : ColorType::kBlack;
  }

  constexpr void set_en_passant(const Square sq) {
    info_.en_passant_ = sq;
  }

  constexpr std::expected<void, std::string_view> set_no_captures(const int moves) {
    if (moves < 0) {
      return std::unexpected("No capture moves number is not valid");
    }
    info_.no_capture_moves_ = moves;
    return {};
  }

  constexpr std::expected<void, std::string_view> set_move_number(const int moves) {
    if (moves < 0) {
      return std::unexpected("Move number is not valid");
    }
    halfmoves_ = 2 * moves + std::to_underlying(side_to_move_);
    return {};
  }
  
  template<MovesType Type>
  void GenerateMoves(MoveList& list);

  bool is_white_move() const noexcept;
  bool is_en_passant() const noexcept;
  std::size_t get_no_capture_moves() const noexcept;
  std::size_t get_move_number() const noexcept;
  Bitboard get_all_pieces() const noexcept;
  Bitboard get_white_pieces() const noexcept;
  Bitboard get_black_pieces() const noexcept;
  Bitboard get_piece_metric(const PieceType piece) const;
  uint8_t get_castles() const noexcept;
  Square get_en_passant() const noexcept;
  std::string get_castling_notation() const noexcept;
  InternalInfo GetInfo() const;
  StateStack GetStateStack() const;

  bool is_pawn(const Square sq) const noexcept;// for tests only
  bool is_knight(const Square sq) const noexcept;// for tests only
  bool is_bishop(const Square sq) const noexcept;// for tests only
  bool is_rook(const Square sq) const noexcept; // for tests only
  bool is_queen(const Square sq) const noexcept;// for tests only
  bool is_king(const Square) const noexcept;// for tests only

  Bitboard get_king_attackers() const;
  Bitboard get_pinned_pieces() const;

  Bitboard GetSquareAttackers(const Square sq, const Bitboard occupied) const;
  bool is_single_check() const noexcept;
  bool is_double_check() const noexcept;
  bool is_check() const noexcept;

  void MakeMove(const Move& move);
  void UnmakeMove(const Move& move);

  bool IsPinned(const Square sq) const noexcept;

private:
  LookupTable<PieceType> board_{};
  std::array<Bitboard, kPieceCount> pieces_{};
  std::array<Bitboard, 2> all_pieces_{};
  ColorType side_to_move_;
  std::size_t halfmoves_ = 1;
  InternalInfo info_{};
  StateStack state_stack_{};

  template<PieceBase Piece> requires attacks::SlidingPiece<Piece>
  void GetPinnedBySlidingPiece(const Square king_sq, const Bitboard occupied, const Bitboard pieces) noexcept;

  constexpr Bitboard& PieceOccupied(const PieceType piece) {
    [[assume(piece != PieceType::kNone)]];
    return pieces_[std::to_underlying(piece) - 1];
  }
  
  void CalculatePinnedPieces() noexcept;
  // inline void ClearCastling(const uint8_t ind) noexcept;
  // inline void ClearCastling(const Square sq, const ColorType side) noexcept;
  inline void UpdateMoveClocks(const Move& move) noexcept;
  inline void UpdateCastleFlags(const MoveFlag flag) noexcept;
  inline void UndoRookCastle(const MoveFlag flag) noexcept;

  inline constexpr void PutPiece(const PieceType piece, const Square sq) noexcept {
    [[assume(piece != PieceType::kNone && sq != Square::kNone)]];
    board_[sq] = piece;
    AddSquareMask(piece, ToBB(sq));
  }

  inline constexpr void AddSquareMask(const PieceType piece, const Bitboard mask) {
    [[assume(piece != PieceType::kNone)]];
    all_pieces_[std::to_underlying(Color(piece))] |= mask;
    PieceOccupied(piece) |= mask;
  }
};

namespace internal {

struct FlippedPosition {
  FlippedPosition(const chess::Position& pos);

  const chess::Position& pos_;
};

}// namespace chess::internal

std::ostream& operator<<(std::ostream& os, const Position& pos);

std::ostream& operator<<(std::ostream& os, const internal::FlippedPosition& flipped_pos);

}// namespace chess


chess::internal::FlippedPosition flipped(const chess::Position& pos);