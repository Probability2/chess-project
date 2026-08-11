#pragma once

#include <algorithm>
#include <array>
#include <iostream>
#include <optional>
#include <string>
#include <utility>
#include <vector>

namespace chess {

inline constexpr int kMaxInd = 8;

constexpr std::size_t kPieceCount = 12;

enum class PieceType {
  kNone,
  kWhitePawn, kWhiteKnight, kWhiteBishop, kWhiteRook, kWhiteQueen, kWhiteKing,
  kBlackPawn, kBlackKnight, kBlackBishop, kBlackRook, kBlackQueen, kBlackKing
};

enum class ColorType {
kNone, kWhite, kBlack
};

constexpr char kEmptySquare = '.';

const std::array<std::string, kPieceCount + 1> kPieceImages = {".", "♙", "♘", "♗", "♖", "♕", "♔",
                                                         "♟", "♞", "♝", "♜", "♛", "♚"};

const std::array<char, kPieceCount + 1> kPieceSymbols = {'.', 'P', 'N', 'B', 'R', 'Q', 'K',
                                                         'p', 'n', 'b', 'r', 'q', 'k'};

struct PieceData {
  PieceType piece;
  char code;
  const char* icon;
};

inline constexpr std::array<PieceData, kPieceCount + 1> kPieceTable {{
    { PieceType::kNone,       '.', "." },
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
    { PieceType::kBlackKing,   'k', "\033[30m♚\033[0m" }
}};

constexpr PieceType GetPieceType(char p) {
  for (auto& data: kPieceTable) {
    if (data.code == p) {
      return data.piece;
    }
  }

  return PieceType::kNone;
}

constexpr char GetPieceCode(PieceType piece) {
  return kPieceTable[static_cast<int>(piece)].code;
}

constexpr std::string GetPieceIcon(PieceType piece) {
  return kPieceTable[static_cast<int>(piece)].icon;
}



inline constexpr ColorType Color(PieceType piece) {
  if (piece == PieceType::kNone) {
    return ColorType::kNone;
  }

  return piece <= PieceType::kWhiteKing ? ColorType::kWhite : ColorType::kBlack;
}

}// end of chess namespace