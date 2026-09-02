#pragma once

#include "Piece.hpp"

#include <concepts>
#include <cstdint>
#include <iostream>
#include <type_traits>

using Bitboard = uint64_t;
using Square = uint8_t;

namespace chess {

enum class SSquare: uint8_t {
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

constexpr SSquare operator++(const SSquare sq) {
  return static_cast<SSquare>(std::to_underlying(sq) + 1);
}

constexpr Bitboard kNotAFile = 0xFEFEFEFEFEFEFEFEULL;
constexpr Bitboard kNotHFile = 0x7F7F7F7F7F7F7F7FULL;
constexpr Bitboard kNotABFile = 0xFCFCFCFCFCFCFCFCULL;
constexpr Bitboard kNotGHFile = 0x3F3F3F3F3F3F3F3FULL;
constexpr Bitboard kNot1Rank = 0xFFFFFFFFFFFFFF00ULL;
constexpr Bitboard kNot12Rank = 0xFFFFFFFFFFFF0000ULL;
constexpr Bitboard kNot8Rank = 0x00FFFFFFFFFFFFFFULL;
constexpr Bitboard kNot78Rank = 0x0000FFFFFFFFFFFFULL;

inline constexpr int coord(const int rank, const int file) {
  return 8 * rank + file;
}

inline constexpr Bitboard ToBB(const Square sq) noexcept {
  [[assume(sq < kBoardSize)]];
  return 1ULL << sq;
}

inline void PrintBitboard(Bitboard b) {
  for (int rank = 7; rank >= 0; --rank) {
    std::cout << rank << " | ";
    for (int file = 0; file < 8; ++file) {
      std::cout << ((b >> coord(rank, file)) & 1) << ' ';
    }
    std::cout << "| " << rank << '\n';
  }
}

inline std::string get_notation(const uint8_t move) {
  return std::string{static_cast<char>((move & 0x07) + 'a'),
                     static_cast<char>(((move >> 3) & 0x07) + '1')};
}

inline void BitLooping(Bitboard bb, std::invocable<uint8_t> auto&& f) {
  while (bb > 0) {
    uint8_t ind = std::countr_zero(bb);
    [[assume(ind < 64)]];
    f(ind);
    bb &= (bb - 1); //Brian Kernighan's algorithm
  }
}

constexpr std::array<int, 8> directions = {7, 8, 9, 1, -7, -8, -9, -1};

inline constexpr Bitboard ShiftDir(Bitboard bb, const int dir) {
  switch (dir) {
    case 7:  return (bb & kNotAFile & kNot8Rank) << 7;
    case 8:  return (bb & kNot8Rank) << 8;
    case 9:  return (bb & kNotHFile & kNot8Rank) << 9;
    case 1:  return (bb & kNotHFile) << 1;
    case -7: return (bb & kNotHFile & kNot1Rank) >> 7;
    case -8: return (bb & kNot1Rank) >> 8;
    case -9: return (bb & kNotAFile & kNot1Rank) >> 9;
    case -1: return (bb & kNotAFile) >> 1;
  }
  std::unreachable();
}

inline constexpr Bitboard GenerateSlide(const uint8_t sq, const int dir, const Bitboard occupied) {
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
  std::array<std::array<Bitboard, kBoardSize>, kBoardSize> rays{};
  for (int sq1 = 0; sq1 < kBoardSize; ++sq1) {
    for (int dir: directions) {
      Bitboard slide1 = GenerateSlide(sq1, dir, 0);
      for (int sq2 = sq1; sq2 < kBoardSize; ++sq2) {
        if (slide1 & ToBB(sq2)) {
          rays[sq1][sq2] = oper(slide1, GenerateSlide(sq2, -dir, 0));
          rays[sq2][sq1] = rays[sq1][sq2];
        }
      }
    }
  }

  return rays;
}

}// namespace internal

inline constexpr auto kBetween = internal::GenerateRays([](auto a, auto b) { return a & b; });;
inline constexpr auto kLines = internal::GenerateRays([](auto a, auto b) { return a | b; });

}