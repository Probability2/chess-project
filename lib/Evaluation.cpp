#include "Evaluation.hpp"

namespace chess::eval {

int Evaluate(const Position& pos) {
  constexpr PieceType white_king = PieceBase::kKing & ColorType::kWhite;
  constexpr PieceType black_king = PieceBase::kKing & ColorType::kBlack;
  if (std::popcount(pos.get_all_pieces()) == 3 &&
     (std::popcount(pos.get_bishops() | pos.get_knights()) != 0)) [[unlikely]] {
    return 0;
  }
  const int perspective = (pos.is_white_move()) ? 1 : -1;
  const int score = pos.get_score();
  const Square sq_white_king = GetLSB(pos.get_piece_metric(white_king));
  const Square sq_black_king = ~GetLSB(pos.get_piece_metric(black_king));
  const int king_mg_value = kKingMgValues[sq_white_king] - kKingMgValues[sq_black_king];
  const int king_eg_value = kKingEgValues[sq_white_king] - kKingEgValues[sq_black_king];
  int phase = (pos.get_phase() * 256 + (kTotalPhase / 2)) / kTotalPhase;
  return perspective * (score + king_mg_value + (((king_eg_value - king_mg_value) * phase) / 256));
  // return perspective * (((score + king_mg_value) * (256 - phase) +
                        //  (score + king_eg_value) * phase) / 256);
  // return perspective * (((score + king_mg_value) * (256 - phase) +
  //                        (score + king_eg_value) * phase) / 256);
}

}// namespace chess::eval