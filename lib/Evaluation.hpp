#pragma once

#include "types/Bitboard.hpp"
#include "types/EvaluationTables.hpp"
#include "types/Piece.hpp"

#include "Position.hpp"

namespace chess::eval {

namespace internal {// Tables, Simpified Evaluation Function

inline constexpr LookupTable<int, 5, PieceBase> kPhaseValues = {0, 1, 1, 2, 4};

inline constexpr int kTotalPhase = kPhaseValues[PieceBase::kPawn] * 16 +
                                   kPhaseValues[PieceBase::kKnight] * 4 +
                                   kPhaseValues[PieceBase::kBishop] * 4 + 
                                   kPhaseValues[PieceBase::kRook] * 4 +
                                   kPhaseValues[PieceBase::kQueen] * 2;// 24

}// namespace internal

int Evaluate(const Position& pos);

}// namespace chess::eval