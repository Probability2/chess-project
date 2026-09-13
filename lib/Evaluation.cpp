#include "Evaluation.hpp"

namespace chess::eval {

int Evaluate(const Position& pos) {
  constexpr PieceType white_king = PieceBase::kKing & ColorType::kWhite;
  constexpr PieceType black_king = PieceBase::kKing & ColorType::kBlack;
  int phase = internal::kTotalPhase - std::popcount(pos.get_all_pieces() ^ pos.get_kings());
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