#include "Piece.hpp"
#include "../core/utils.hpp"

namespace chess::attacks {

namespace internal {

constexpr Bitboard kNotAFile = 0xFEFEFEFEFEFEFEFEULL;
constexpr Bitboard kNotHFile = 0x7F7F7F7F7F7F7F7FULL;
constexpr Bitboard kNotABFile = 0xFCFCFCFCFCFCFCFCULL;
constexpr Bitboard kNotGHFile = 0x3F3F3F3F3F3F3F3FULL;
constexpr Bitboard kNot1Rank = 0xFFFFFFFFFFFFFF00ULL;
constexpr Bitboard kNot12Rank = 0xFFFFFFFFFFFF0000ULL;
constexpr Bitboard kNot8Rank = 0x00FFFFFFFFFFFFFFULL;
constexpr Bitboard kNot78Rank = 0x0000FFFFFFFFFFFFULL;

}// namespace internal

inline constexpr std::array<std::array<Bitboard, kBoardSize>, 2> kPawnAttacks = []() {
  std::array<std::array<Bitboard, kBoardSize>, 2> attacks{};
  for (std::size_t i = 0; i < kBoardSize; ++i) {
    Bitboard pawn_coord = 1ULL << i;
    attacks[0][i] = ((pawn_coord & internal::kNotAFile) << 7) | ((pawn_coord & internal::kNotHFile) << 9);
    attacks[1][i] = ((pawn_coord & internal::kNotHFile) >> 7) | ((pawn_coord & internal::kNotAFile) >> 9);
  }

  return attacks;
}();

inline constexpr std::array<Bitboard, kBoardSize> kKnightAttacks = []() {
  std::array<Bitboard, kBoardSize> attacks{};
  for (std::size_t i = 0; i < kBoardSize; ++i) {
    Bitboard knight_coord = 1ULL << i;
    attacks[i] = (knight_coord & internal::kNotAFile & internal::kNot78Rank) << 15; //e4->d6
    attacks[i] |= (knight_coord & internal::kNotABFile & internal::kNot8Rank) << 6; //e4->c5
    attacks[i] |= (knight_coord & internal::kNotABFile & internal::kNot1Rank) >> 10;//e4->c3
    attacks[i] |= (knight_coord & internal::kNotAFile & internal::kNot12Rank) >> 17;//e4->d2
    attacks[i] |= (knight_coord & internal::kNotHFile & internal::kNot12Rank) >> 15;//e4->f2
    attacks[i] |= (knight_coord & internal::kNotGHFile & internal::kNot1Rank) >> 6;//e4->g3
    attacks[i] |= (knight_coord & internal::kNotGHFile & internal::kNot8Rank) << 10;//e4->g5
    attacks[i] |= (knight_coord & internal::kNotHFile & internal::kNot78Rank) << 17;//e4->f6
  }

  return attacks;
}();

inline constexpr std::array<Bitboard, kBoardSize> kKingAttacks = []() {
  std::array<Bitboard, kBoardSize> attacks{};
  for (std::size_t i = 0; i < kBoardSize; ++i) {
    Bitboard king_coord = 1ULL << i;
    attacks[i] = (king_coord & internal::kNotAFile) >> 1;                        //e4->d4
    attacks[i] |= (king_coord & internal::kNotAFile && internal::kNot1Rank) >> 9;//e4->d3
    attacks[i] |= (king_coord & internal::kNot1Rank) >> 8;                       //e4->e3
    attacks[i] |= (king_coord & internal::kNotHFile && internal::kNot1Rank) >> 7;//e4->f3
    attacks[i] |= (king_coord & internal::kNotHFile) << 1;                       //e4->f4
    attacks[i] |= (king_coord & internal::kNotHFile && internal::kNot8Rank) << 9;//e4->f5
    attacks[i] |= (king_coord & internal::kNot8Rank) << 8;                       //e4->e5
    attacks[i] |= (king_coord & internal::kNotAFile && internal::kNot8Rank) << 7;//e4->d5
  }

  return attacks;
}();

}// namespace chess::attacks