#pragma once

#include "types/Bitboard.hpp"
#include "types/EvaluationTables.hpp"
#include "types/Piece.hpp"

#include "Position.hpp"

#include <optional>

namespace chess::eval {

namespace internal {

inline bool IsInsuffientMaterial(const Position& pos) {
  uint8_t count_all_pieces = std::popcount(pos.get_all_pieces());
  if (count_all_pieces == 3 && (pos.get_bishops() | pos.get_knights()) != 0) [[unlikely]] {
    return true;
  }
  if (count_all_pieces == 4 && (std::popcount(pos.get_piece_metric(PieceType::kWhiteKnight)) == 2 ||
      std::popcount(pos.get_piece_metric(PieceType::kBlackKnight)) == 2)) [[unlikely]] {
    return true;
  }

  return false;
}

}// namespace internal

inline int Evaluate(const Position& pos) {
  if (internal::IsInsuffientMaterial(pos)) [[unlikely]] {
    return 0;
  }
  const int perspective = (pos.is_white_move()) ? 1 : -1;
  const Square sq_white_king = GetLSB(pos.get_piece_metric(PieceType::kWhiteKing));
  const Square sq_black_king = ~GetLSB(pos.get_piece_metric(PieceType::kBlackKing));
  const int king_mg_value = kKingMgValues[sq_white_king] - kKingMgValues[sq_black_king];
  const int king_eg_value = kKingEgValues[sq_white_king] - kKingEgValues[sq_black_king];
  int phase = (pos.get_phase() * 256 + (kTotalPhase / 2)) / kTotalPhase;

  return perspective * (pos.get_score() + king_mg_value + (((king_eg_value - king_mg_value) * phase) / 256));
}

}// namespace chess::eval