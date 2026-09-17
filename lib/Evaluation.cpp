#include "Evaluation.hpp"

namespace chess::eval {

int Evaluate(const Position& pos) {
  constexpr PieceType white_king = PieceBase::kKing & ColorType::kWhite;
  constexpr PieceType black_king = PieceBase::kKing & ColorType::kBlack;
  int phase = internal::kTotalPhase;
  phase -= std::popcount(pos.get_pawns()) * internal::kPhaseValues[PieceBase::kPawn];
  phase -= std::popcount(pos.get_knights()) * internal::kPhaseValues[PieceBase::kKnight];
  phase -= std::popcount(pos.get_bishops()) * internal::kPhaseValues[PieceBase::kBishop];
  phase -= std::popcount(pos.get_rooks()) * internal::kPhaseValues[PieceBase::kRook];
  phase -= std::popcount(pos.get_queens()) * internal::kPhaseValues[PieceBase::kQueen];
  const int perspective = (pos.is_white_move()) ? 1 : -1;
  const int score = pos.GetWhiteScore();
  const int king_mg_value = kKingMgValues[GetLSB(pos.get_piece_metric(white_king))] -
                            kKingMgValues[~GetLSB(pos.get_piece_metric(black_king))];
  const int king_eg_value = kKingEgValues[GetLSB(pos.get_piece_metric(white_king))] -
                            kKingEgValues[~GetLSB(pos.get_piece_metric(black_king))];
  phase = (phase * 256 + (internal::kTotalPhase / 2)) / internal::kTotalPhase;
  return perspective * (((score + king_mg_value) * (256 - phase) +
                         (score + king_eg_value) * phase) / 256);
}

}// namespace chess::eval