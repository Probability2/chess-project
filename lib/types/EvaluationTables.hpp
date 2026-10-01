#pragma once

#include "Bitboard.hpp"
#include "Piece.hpp"

#include <array>

namespace chess::eval {

inline constexpr LookupTable<int, 6, PieceBase> kPieceValues = {100, 310, 330, 500, 900, 0};

inline constexpr auto kPieceSquareTable = []() {
  LookupTable<BoardLookup<int>, kPieceCount, PieceType> PSQT{};
  LookupTable<BoardLookup<int>, 5, PieceBase> kSquareTable = {{
    BoardLookup<int>{{
      0,  0,  0,  0,  0,  0,  0,  0,
      50, 50, 50, 50, 50, 50, 50, 50,
      10, 10, 20, 30, 30, 20, 10, 10,
      5,  5, 10, 25, 25, 10,  5,  5,
      0,  0,  0, 20, 20,  0,  0,  0,
      5, -5,-10,  0,  0, -10, -5,  5,
      5, 10, 10,-20,-20, 10, 10,  5,
      0,  0,  0,  0,  0,  0,  0,  0}},
    BoardLookup<int>{{
      -50,-40,-30,-30,-30,-30,-40,-50,
      -40,-20,  0,  0,  0,  0,-20,-40,
      -30,  0, 10, 15, 15, 10,  0,-30,
      -30,  5, 15, 20, 20, 15,  5,-30,
      -30,  0, 15, 20, 20, 15,  0,-30,
      -30,  5, 10, 15, 15, 10,  5,-30,
      -40,-20,  0,  5,  5,  0,-20,-40,
      -50,-40,-30,-30,-30,-30,-40,-50}},
    BoardLookup<int>{{
      -20,-10,-10,-10,-10,-10,-10,-20,
      -10,  0,  0,  0,  0,  0,  0,-10,
      -10,  0,  5, 10, 10,  5,  0,-10,
      -10,  5,  5, 10, 10,  5,  5,-10,
      -10,  0, 10, 10, 10, 10,  0,-10,
      -10, 10, 10, 10, 10, 10, 10,-10,
      -10,  5,  0,  0,  0,  0,  5,-10,
      -20, -10,-10,-10,-10,-10,-10,-20}},
    BoardLookup<int>{{
      0,  0,  0,  0,  0,  0,  0,  0,
      5, 10, 10, 10, 10, 10, 10,  5,
      -5,  0,  0,  0,  0,  0,  0, -5,
      -5,  0,  0,  0,  0,  0,  0, -5,
      -5,  0,  0,  0,  0,  0,  0, -5,
      -5,  0,  0,  0,  0,  0,  0, -5,
      -5,  0,  0,  0,  0,  0,  0, -5,
      0,  0,  0,  5,  5,  0,  0,  0}},
    BoardLookup<int>{{
      -20,-10,-10, -5, -5,-10,-10,-20,
      -10,  0,  0,  0,  0,  0,  0,-10,
      -10,  0,  5,  5,  5,  5,  0,-10,
      -5,  0,  5,  5,  5,  5,  0, -5,
       0,  0,  5,  5,  5,  5,  0, -5,
      -10,  5,  5,  5,  5,  5,  0,-10,
      -10,  0,  5,  0,  0,  0,  0,-10,
      -20,-10,-10, -5, -5,-10,-10,-20}}
  }};
  for (int i = 0; i < 12; ++i) {
    PieceBase base = static_cast<PieceBase>(i % 6);
    ColorType color = (i <= 5) ? ColorType::kWhite : ColorType::kBlack;
    int perspective = (i <= 5) ? 1 : -1;
    for (Square sq : Board) {
      if (base == PieceBase::kKing) {
        PSQT[base & color][sq] = 0;
        continue;
      }
      PSQT[base & color][sq] = perspective * (kSquareTable[base][Persp(sq, color)] + kPieceValues[base]);
    }
  }

  return PSQT;
}();

inline constexpr BoardLookup<int> kKingMgValues = {{
  -30, -40, -40, -50, -50, -40, -40, -30,
  -30, -40, -40, -50, -50, -40, -40, -30,
  -30, -40, -40, -50, -50, -40, -40, -30,
  -30, -40, -40, -50, -50, -40, -40, -30,
  -20, -30, -30, -40, -40, -30, -30, -20,
  -10, -20, -20, -20, -20, -20, -20, -10,
   20,  20,  0,   0,   0,   0,   20,  20,
   20,  30,  10,  0,   0,   10,  30,  20
}};

inline constexpr BoardLookup<int> kKingEgValues = {{
  -50, -40, -30, -20, -20, -30,  -40, -50,
  -30, -20, -10,  0,   0,  -10,  -20, -30,
  -30, -10,  20,  30,  30,  20,  -10, -30,
  -30, -10,  30,  40,  40,  30,  -10, -30,
  -30, -10,  30,  40,  40,  30,  -10, -30,
  -30, -10,  20,  30,  30,  20,  -10, -30,
  -30, -30,  0,   0,   0,   0,   -30, -30,
  -50, -30, -30, -30, -30,  -30, -30, -50
}};

inline constexpr auto kMVP_LVA = []() {
  LookupTable<LookupTable<int, 8, PieceBase>, 8, PieceBase> table{};
  for (PieceBase agressor : kPieceBases) {
    for (PieceBase victim : kPieceBases) {
      table[victim][agressor] = kPieceValues[victim] - kPieceValues[agressor];
    }
  }

  return table;
}();

inline constexpr auto kMVP_LVA2 = []() {
  std::array<int, kBoardSize> table{};
  for (int i = 0; i < 6; ++i) {
    for (int j = 0; j < 6; ++j) {
      table[(i << 3) + j] = kPieceValues[static_cast<PieceBase>(i)] - kPieceValues[static_cast<PieceBase>(j)];
    }
  }

  return table;
}();

inline constexpr LookupTable<int, 6, PieceBase> kPhaseValues = {0, 1, 1, 2, 4, 0};

inline constexpr int kTotalPhase = kPhaseValues[PieceBase::kPawn] * 16 +
                                   kPhaseValues[PieceBase::kKnight] * 4 +
                                   kPhaseValues[PieceBase::kBishop] * 4 + 
                                   kPhaseValues[PieceBase::kRook] * 4 +
                                   kPhaseValues[PieceBase::kQueen] * 2;// 24

inline int value(const PieceType piece) noexcept {
  return kPieceValues[GetPieceBase(piece)];
}


}// namespace chess::eval