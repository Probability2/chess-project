#pragma once

#include <array>
#include <cassert>
#include <cstdint>
#include <iostream>
#include <span>

namespace chess {

inline constexpr uint8_t kMaxInd = 8;

inline constexpr uint8_t kSquaresCount = 64;

inline constexpr std::size_t kPieceCount = 12;

inline constexpr std::size_t kMaxMoves = 256;

enum class PieceType: uint8_t {
  kNone,
  kWhitePawn, kWhiteKnight, kWhiteBishop, kWhiteRook, kWhiteQueen, kWhiteKing,
  kBlackPawn, kBlackKnight, kBlackBishop, kBlackRook, kBlackQueen, kBlackKing
};

enum class ColorType: uint8_t {
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

enum class MovesType: uint8_t {
  kPseudo, kLegal, kCaptures, kChecks
};

struct Move {
  Move() = default;

  Move(const PieceType piece, const uint8_t from, const uint8_t to);

  Move(const PieceType piece, const uint8_t from, const uint8_t to, const PieceType promoted_piece);

  bool operator==(const Move& other) const = default;

  bool has_promoted_piece() const;

  bool is_pawn() const;

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

}// end of chess namespace