#pragma once

#include "Piece.hpp"

#include <concepts>
#include <cstdint>
#include <iostream>
#include <type_traits>

using Bitboard = uint64_t;

namespace chess {

constexpr Bitboard kNotAFile = 0xFEFEFEFEFEFEFEFEULL;
constexpr Bitboard kNotHFile = 0x7F7F7F7F7F7F7F7FULL;
constexpr Bitboard kNotABFile = 0xFCFCFCFCFCFCFCFCULL;
constexpr Bitboard kNotGHFile = 0x3F3F3F3F3F3F3F3FULL;
constexpr Bitboard kNot1Rank = 0xFFFFFFFFFFFFFF00ULL;
constexpr Bitboard kNot12Rank = 0xFFFFFFFFFFFF0000ULL;
constexpr Bitboard kNot8Rank = 0x00FFFFFFFFFFFFFFULL;
constexpr Bitboard kNot78Rank = 0x0000FFFFFFFFFFFFULL;
constexpr Bitboard kAllSquares = 0xFFFFFFFFFFFFFFFFULL;

enum class Square: uint8_t {
  A1, B1, C1, D1, E1, F1, G1, H1,
  A2, B2, C2, D2, E2, F2, G2, H2,
  A3, B3, C3, D3, E3, F3, G3, H3,
  A4, B4, C4, D4, E4, F4, G4, H4,
  A5, B5, C5, D5, E5, F5, G5, H5,
  A6, B6, C6, D6, E6, F6, G6, H6,
  A7, B7, C7, D7, E7, F7, G7, H7,
  A8, B8, C8, D8, E8, F8, G8, H8,
  kNone
};

enum class Direction: int8_t {
  kNorth = 8,
  kNorthEast = 9,
  kEast = 1,
  kSouthEast = -7,
  kSouth = -8,
  kSouthWest = -9,
  kWest = -1,
  kNorthWest = 7,
};

inline constexpr Direction operator-(const Direction dir) {
  return static_cast<Direction>(-std::to_underlying(dir));
}

inline std::string to_string(const Square sq) {
  std::string str;
  if (sq == Square::kNone) {
    str += "kNone";
  } else {
    str += static_cast<char>('a' + std::to_underlying(sq) % 8);
    str += static_cast<char>('1' + std::to_underlying(sq) / 8);
  }

  return str;
}

inline std::ostream& operator<<(std::ostream& os, const Square sq) {
  os << to_string(sq);

  return os;
}

constexpr Square operator++(const Square sq) {
  return static_cast<Square>(std::to_underlying(sq) + 1);
}

inline Square operator-(const Square sq, const int dir) {
  const uint8_t sq_val = std::to_underlying(sq);
  assert(sq_val - dir >= 0 && sq_val - dir < kBoardSize);
  return static_cast<Square>(sq_val - dir);
}

inline Square operator+(const Square sq, const int dir) {
  return sq - (-dir);
}

inline Square operator-(const Square sq, const Direction dir) {
  return sq - std::to_underlying(dir);
}

inline Square operator+(const Square sq, const Direction dir) {
  return sq + std::to_underlying(dir);
}

inline Bitboard operator<<(const Bitboard base, const Square sq) {
  return base << std::to_underlying(sq);
}

template<typename T>
struct LookupTable {
  std::array<T, kBoardSize> table_{};

  constexpr decltype(auto) operator[](this auto& self, const Square sq) noexcept {
    [[assume(sq != Square::kNone)]];
    return self.table_[std::to_underlying(sq)];
  }
};

template<typename T>
concept EnumClass = std::is_scoped_enum_v<T>;

template<std::size_t N>
struct MultiLookupTable {
  std::array<std::array<Bitboard, kBoardSize>, N> table_{};

  constexpr decltype(auto) operator[](this auto& self, const EnumClass auto ind, const Square sq) noexcept {
    [[assume(sq != Square::kNone)]];
    return self.table_[std::to_underlying(ind)][std::to_underlying(sq)];
  }
};

inline constexpr auto Board = []() {
  std::array<Square, kBoardSize> squares{};
  for (int i = 0; i < kBoardSize; ++i) {
    squares[i] = static_cast<Square>(i);
  }
  
  return squares;
}();

inline constexpr std::array<std::array<Square, 2>, 2> kCastleInterSq = {{{Square::F1, Square::F8},
                                                                         {Square::D1, Square::D8}}};

inline constexpr std::array<std::array<Square, 2>, 2> kRookCastleSquares = {{{Square::H1, Square::H8},
                                                                             {Square::A1, Square::A8}}};
inline constexpr std::array<int, 2> kCastleShifts = {2, -2};

inline constexpr auto kCastlingRights = []() {
  LookupTable<uint8_t> rights;
  for (Square sq : Board) {
    switch (sq) {
      case Square::A1: rights[sq] = 0x0D; break;
      case Square::E1: rights[sq] = 0x0C; break;
      case Square::H1: rights[sq] = 0x0E; break;
      case Square::A8: rights[sq] = 0x07; break;
      case Square::E8: rights[sq] = 0x03; break;
      case Square::H8: rights[sq] = 0x0B; break;
      default: rights[sq] = 0xFF;
    }
  }

  return rights;
}();

inline constexpr Square coord(const int rank, const int file) {
  [[assume(8 * rank + file <= kBoardSize)]];
  return static_cast<Square>(8 * rank + file);
}

inline constexpr Bitboard ToBB(const Square sq) noexcept {
  [[assume(sq != Square::kNone)]];
  return 1ULL << std::to_underlying(sq);
}

inline void PrintBitboard(const Bitboard b) {
  for (int rank = 7; rank >= 0; --rank) {
    std::cout << rank << " | ";
    for (int file = 0; file < 8; ++file) {
      if ((b >> std::to_underlying(coord(rank, file))) & 1) {
        std::cout << "\033[1;31m1\033[0m";
      } else {//\033[1;32m
        std::cout << 0;
      }
      std::cout << ' ';
    }
    std::cout << "| " << rank << '\n';
  }
}

// inline std::string get_notation(const uint8_t move) {
//   return std::string{static_cast<char>((move & 0x07) + 'a'),
//                      static_cast<char>(((move >> 3) & 0x07) + '1')};
// }

inline constexpr Square GetLSB(Bitboard bb) {
  const uint8_t ind = std::countr_zero(bb);
  [[assume(ind < 64)]];
  
  return static_cast<Square>(ind);
}

inline constexpr void BitLooping(Bitboard bb, std::invocable<Square> auto&& fun) {
  while (bb > 0) {
    Square sq = GetLSB(bb);
    fun(sq);
    bb &= (bb - 1); //Brian Kernighan's algorithm
  }
}

constexpr std::array<Direction, 8> directions = {Direction::kNorthWest, Direction::kNorth,
                                                 Direction::kNorthEast, Direction::kEast,
                                                 Direction::kSouthEast, Direction::kSouth,
                                                 Direction::kSouthWest, Direction::kWest};

inline constexpr Bitboard ShiftDir(Bitboard bb, const Direction dir) {
  switch (dir) {
    case Direction::kNorthWest:  return (bb & kNotAFile & kNot8Rank) << 7;
    case Direction::kNorth:  return (bb & kNot8Rank) << 8;
    case Direction::kNorthEast:  return (bb & kNotHFile & kNot8Rank) << 9;
    case Direction::kEast:  return (bb & kNotHFile) << 1;
    case Direction::kSouthEast: return (bb & kNotHFile & kNot1Rank) >> 7;
    case Direction::kSouth: return (bb & kNot1Rank) >> 8;
    case Direction::kSouthWest: return (bb & kNotAFile & kNot1Rank) >> 9;
    case Direction::kWest: return (bb & kNotAFile) >> 1;
  }
  std::unreachable();
}

inline constexpr Bitboard GenerateSlide(const Square sq, const Direction dir, const Bitboard occupied) {
  Bitboard slides = ShiftDir(ToBB(sq), dir);
  for (std::size_t i = 0; i < 8; ++i) {
    if (occupied & slides) {
      break;
    }
    slides |= ShiftDir(slides, dir);
  }

  return slides;
}

namespace internal {

constexpr auto GenerateRays(auto oper) {
  MultiLookupTable<kBoardSize> rays{};
  for (Square sq1 : Board) {
    for (Direction dir: directions) {
      Bitboard slide1 = GenerateSlide(sq1, dir, 0);
      BitLooping(slide1, [&rays, &oper, dir, slide1, sq1](const Square sq2) {
        if (sq1 >= sq2) {
          return;
        }
        rays[sq1, sq2] = oper(slide1, GenerateSlide(sq2, -dir, 0));
        rays[sq2, sq1] = rays[sq1, sq2];
      });
    }
  }

  return rays;
}

}// namespace internal

inline constexpr auto kBetween = internal::GenerateRays([](auto a, auto b) { return a & b; });;
inline constexpr auto kLines = internal::GenerateRays([](auto a, auto b) { return a | b; });

}