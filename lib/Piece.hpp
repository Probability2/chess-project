#pragma once

#include <algorithm>
#include <array>
#include <iostream>
#include <optional>
#include <string>
#include <utility>
#include <vector>

constexpr std::size_t kCountPiece = 13;

enum class Pieces {
  kEmpty,
  kWhitePawn, kWhiteKnight, kWhiteBishop, kWhiteRook, kWhiteQueen, kWhiteKing,
  kBlackPawn, kBlackKnight, kBlackBishop, kBlackRook, kBlackQueen, kBlackKing
};

constexpr char kEmptySquare = '.';

const std::array<std::string, kCountPiece> kPieceImages = {".", "♙", "♘", "♗", "♖", "♕", "♔",
                                                         "♟", "♞", "♝", "♜", "♛", "♚"};

const std::array<char, kCountPiece> kPieceSymbols = {'.', 'P', 'N', 'B', 'R', 'Q', 'K',
                                                         'p', 'n', 'b', 'r', 'q', 'k'};

struct PieceData {
  Pieces piece;
  char code;
  const char* icon;
};


// constexpr const char* kReset = "\033[0m";
// constexpr const char* kWhiteText = "\033[97m";
// constexpr const char* kBlackText = "\033[30m";

inline constexpr std::array<PieceData, kCountPiece> kPieceTable {{
    { Pieces::kEmpty,       '.', "." },
    { Pieces::kWhitePawn,   'P', "\033[97m♟\033[0m"},
    { Pieces::kWhiteKnight, 'N', "\033[97m♞\033[0m" },
    { Pieces::kWhiteBishop, 'B', "\033[97m♝\033[0m" },
    { Pieces::kWhiteRook,   'R', "\033[97m♜\033[0m" },
    { Pieces::kWhiteQueen,  'Q', "\033[97m♛\033[0m" },
    { Pieces::kWhiteKing,   'K', "\033[97m♚\033[0m" },
    { Pieces::kBlackPawn,   'p', "\033[30m♟\033[0m" },
    { Pieces::kBlackKnight, 'n', "\033[30m♞\033[0m" },
    { Pieces::kBlackBishop, 'b', "\033[30m♝\033[0m" },
    { Pieces::kBlackRook,   'r', "\033[30m♜\033[0m" },
    { Pieces::kBlackQueen,  'q', "\033[30m♛\033[0m" },
    { Pieces::kBlackKing,   'k', "\033[30m♚\033[0m" }
}};

constexpr Pieces GetPieceCode(char p) {
  for (auto& data: kPieceTable) {
    if (data.code == p) {
      return data.piece;
    }
  }

  return Pieces::kEmpty;
}

constexpr std::string GetPieceIcon(Pieces piece) {
  return kPieceTable[static_cast<int>(piece)].icon;
}



inline void print(char coord, int ind) {
  std::cout << coord << ' ' << ind << '\n';
}

static constexpr int kMaxInd = 8;
static constexpr char kMinCoord = 'a';
static constexpr char kMaxCoord = 'h';

