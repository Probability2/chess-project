#pragma once

#include "types/Piece.hpp"
#include "types/Bitboard.hpp"

#include <array>
#include <iostream>
#include <concepts>
#include <optional>
#include <span>

namespace chess {

inline constexpr std::size_t kMaxMoves = 256;

enum class MovesType: uint8_t {
  kPseudo, kLegal, kCaptures, kChecks, kEvasions, kQuiets
};

enum class MoveFlag: uint8_t {
  kQuiet = 0,
  kDoublePawnPush = 1,
  kKingCastle = 2,
  kQueenCastle = 3,
  kCapture = 4,
  kEpCapture = 5,
  kKnightPromotion = 8,
  kBishopPromotion = 9,
  kRookPromotion = 10,
  kQueenPromotion = 11,
  kKnightPromoCapture = 12,
  kBishopPromoCapture = 13,
  kRookPromoCapture = 14,
  kQueenPromoCapture = 15
};

template<typename T>
concept Scoreable = requires(T t) {
  t.CalculateScore();
};

template<typename T>
concept Displayable = requires(std::ostream& os, T t) {
  os << t;
};

//16 bit representation to preserve memory
class Move {
public:
  Move() = default;
  Move(const Square from, const Square to);
  Move(const Square from, const Square to, const MoveFlag flag);
  Move(std::string_view str);// now for tests only
  bool operator==(const Move& other) const = default;
  bool operator==(std::string_view str) const;
  Square get_from() const;
  Square get_to() const;
  PieceBase get_promoted_base() const;
  MoveFlag get_flag() const;
  bool is_capture() const;;
  bool is_double_pawn_push() const;
  bool is_promotion() const;
  bool is_en_passant() const;
  bool is_castle() const;
  bool is_king_castle() const;
  bool is_queen_castle() const;
  bool is_50_moves_eligible() const;

  bool operator<(const Move& other) const {
    return move_val_ < other.move_val_;
  }

  operator bool() const;

private:
  uint16_t move_val_ = 0;
};

struct MoveEntry: public Move {
  MoveEntry() = default;
  MoveEntry(const Move move);

  int score_ = 0;
};

inline constexpr auto kNullMove = Move();

template<typename T>
concept IsMove = std::derived_from<T, Move>;

template<IsMove T = Move>
class MoveList {
public:
  MoveList() = default;
  std::size_t size() const;
  std::span<const T> AsSpan() const;
  std::span<T> AsSpan();
  bool contains(const T& m) const;
  bool empty() const;
  void push(const T& move);
  void pop_back();
  const T& back() const;
  void remove(std::size_t ind);
  void RemovePvMove(const std::optional<Move> pv_move);
  // void ScoreMoves() requires Scoreable<T>;

  decltype(auto) operator[](this auto& self, const std::size_t ind) {
    return self.moves_[ind];
  }

private:
  std::array<T, kMaxMoves> moves_;
  std::size_t size_ = 0;
};

std::ostream& operator<<(std::ostream& os, const Move& move);

std::ostream& operator<<(std::ostream& os, const MoveEntry& move);

template<Displayable T>
std::ostream& operator<<(std::ostream& os, const MoveList<T>& list) {
  for (const auto& move: list.AsSpan()) {
    os << move << ", ";
  }

  return os;
}

}// namespace chess