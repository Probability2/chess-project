#include "Evaluation.hpp"

namespace chess::eval {

// namespace {
//   int CalculatePhase(const Position& pos) {
//     int phase = kTotalPhase;
//     std::size_t ind = 0;
//     for (Bitboard bb : pos.GetPiecesArray()) {
//       phase -= kPhaseValues[std::min(ind, )]
//     }
//   }
// }

int Evaluate(const Position& pos) {
  constexpr PieceType white_king = PieceBase::kKing & ColorType::kWhite;
  constexpr PieceType black_king = PieceBase::kKing & ColorType::kBlack;
  int phase = internal::kTotalPhase;
  int w_pieces_value = 0;
  int b_pieces_value = 0;
  int king_mg_value = 0;
  int king_eg_value = 0;
  BitLooping(pos.get_all_pieces() ^ pos.get_kings(), [&](const Square sq){
    const PieceType piece = pos.PieceOn(sq);
    const std::size_t base = std::to_underlying(GetPieceBase(piece));
    if (Color(piece) == ColorType::kWhite) {
      w_pieces_value += (internal::kPieceValues[base] + internal::kPieceSquareTable[base][sq]);
    } else {
      b_pieces_value += (internal::kPieceValues[base] + internal::kPieceSquareTable[base][~sq]);
    }
    phase -= internal::kPhaseValues[base];
  });
  phase = (phase * 256 + (internal::kTotalPhase / 2)) / internal::kTotalPhase;
  king_mg_value += internal::kKingMgValues[GetLSB(pos.get_piece_metric(white_king))];
  king_mg_value -= internal::kKingMgValues[~GetLSB(pos.get_piece_metric(black_king))];
  king_eg_value += internal::kKingEgValues[GetLSB(pos.get_piece_metric(white_king))];
  king_eg_value -= internal::kKingEgValues[~GetLSB(pos.get_piece_metric(black_king))];
  const int perspective = (pos.is_white_move()) ? 1 : -1;
  king_mg_value *= perspective;
  king_eg_value *= perspective;
  const int score = (w_pieces_value - b_pieces_value) * perspective;
  return ((score + king_mg_value) * (256 - phase) +
          (score + king_eg_value) * phase) / 256;
}

}// namespace chess::eval