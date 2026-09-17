#pragma once

#include <algorithm>
#include <array>
#include <cassert>
#include <cstdint>
#include <iostream>
#include <ranges>
#include <span>
#include <utility>

namespace chess {

inline constexpr uint8_t kMaxInd = 8;

inline constexpr uint8_t kBoardSize = 64;

inline constexpr std::size_t kPieceBaseCount = 6;

inline constexpr std::size_t kPieceCount = 12;

enum class ColorType: uint8_t {
  kWhite, kBlack
};

enum class PieceBase: uint8_t {
  kPawn, kKnight, kBishop, kRook, kQueen, kKing
};

enum class PieceType: uint8_t {
  kWhitePawn, kWhiteKnight, kWhiteBishop, kWhiteRook, kWhiteQueen, kWhiteKing,
  kBlackPawn, kBlackKnight, kBlackBishop, kBlackRook, kBlackQueen, kBlackKing,
  kNone
};

constexpr char kEmptySquare = '.';

inline constexpr std::array<std::array<PieceType, kPieceBaseCount>, 2> kPieceMap = {{
  {PieceType::kWhitePawn, PieceType::kWhiteKnight, PieceType::kWhiteBishop,
   PieceType::kWhiteRook, PieceType::kWhiteQueen, PieceType::kWhiteKing},
  {PieceType::kBlackPawn, PieceType::kBlackKnight, PieceType::kBlackBishop,
   PieceType::kBlackRook, PieceType::kBlackQueen, PieceType::kBlackKing}
}};

inline constexpr PieceType operator&(const PieceBase base, const ColorType color) noexcept {
  return kPieceMap[std::to_underlying(color)][std::to_underlying(base)];
}

inline constexpr ColorType operator!(const ColorType color) {
  if (color == ColorType::kWhite) {
    return ColorType::kBlack;
  }
  return ColorType::kWhite;
}

inline constexpr std::array<std::string, kPieceCount + 1> kPieceImages = {".", "♙", "♘", "♗", "♖", "♕", "♔",
                                                                          "♟", "♞", "♝", "♜", "♛", "♚"};

inline constexpr std::array<char, kPieceCount + 1> kPieceSymbols = {'.', 'P', 'N', 'B', 'R', 'Q', 'K',
                                                                    'p', 'n', 'b', 'r', 'q', 'k'};

inline constexpr std::array<std::array<char, 4>, 2> kPromotedPieces = {{{'N', 'B', 'R', 'Q'},
                                                                        {'n', 'b', 'r', 'q'}}};
  
inline constexpr ColorType Color(PieceType piece) {
  return piece <= PieceType::kWhiteKing ? ColorType::kWhite : ColorType::kBlack;
}

struct PieceData {
  PieceType piece;
  char code;
  const char* icon;
};

inline constexpr std::array<PieceData, kPieceCount + 1> kPieceTable {{
  { PieceType::kWhitePawn,   'P', "\033[97m♟\033[0m"},
  { PieceType::kWhiteKnight, 'N', "\033[97m♞\033[0m" },
  { PieceType::kWhiteBishop, 'B', "\033[97m♝\033[0m" },
  { PieceType::kWhiteRook,   'R', "\033[97m♜\033[0m" },
  { PieceType::kWhiteQueen,  'Q', "\033[97m♛\033[0m" },
  { PieceType::kWhiteKing,   'K', "\033[97m♚\033[0m" },
  { PieceType::kBlackPawn,   'p', "\033[30m♟\033[0m" },
  { PieceType::kBlackKnight, 'n', "\033[30m♞\033[0m" },
  { PieceType::kBlackBishop, 'b', "\033[30m♝\033[0m" },
  { PieceType::kBlackRook,   'r', "\033[30m♜\033[0m" },
  { PieceType::kBlackQueen,  'q', "\033[30m♛\033[0m" },
  { PieceType::kBlackKing,   'k', "\033[30m♚\033[0m" },
  { PieceType::kNone,       '.', "." }
}};

constexpr PieceType GetPieceType(char p) {
  for (const auto& data: kPieceTable) {
    if (data.code == p) {
      return data.piece;
    }
  }
  
  return PieceType::kNone;
}

inline constexpr PieceBase GetPieceBase(PieceType piece) {
  [[assume(piece != PieceType::kNone)]];
  const auto& map = (Color(piece) == ColorType::kWhite) ? kPieceMap[0] : kPieceMap[1];
  return static_cast<PieceBase>(std::ranges::find(map, piece) - map.begin());
}

inline bool IsSliding(const PieceType piece) {
  [[assume(piece != PieceType::kNone)]];
  const PieceBase base = GetPieceBase(piece);
  return (base == PieceBase::kBishop || base == PieceBase::kRook || base == PieceBase::kQueen);
}

constexpr char GetPieceCode(PieceBase piece) {
  return kPieceTable[6 + std::to_underlying(piece)].code;
}

constexpr char GetPieceCode(PieceType piece) {
  return kPieceTable[std::to_underlying(piece)].code;
}

constexpr std::string GetPieceIcon(PieceType piece) {
  return kPieceTable[std::to_underlying(piece)].icon;
}


}// end of chess namespace