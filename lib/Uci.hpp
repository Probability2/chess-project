#pragma once

#include "types/Bitboard.hpp"
#include "types/Piece.hpp"

#include "Position.hpp"
#include "Move.hpp"

#include <ranges>
#include <string_view>

namespace chess::uci {

Move UciToMove(std::string_view str, Position& pos) {
  if (str.length() < 4) {
    return Move();// Incorrect input!!
  }
  int file_from = str[0] - 'a';
  int rank_from = str[1] - '1';
  int file_to = str[2] - 'a';
  int rank_to = str[3] - '1';
  if (file_from < 0 || file_from > 8 || rank_from < 0 || rank_from > 8 ||
      file_to < 0 || file_to > 8 || rank_to < 0 || rank_to > 8) {
    return Move();// Incorrect input!!
  }
  Square from = coord(rank_from, file_from);
  Square to = coord(rank_to, file_to);
  PieceType promoted = PieceType::kNone;
  ColorType side = pos.side_to_move();
  auto& arr = kPromotedPieces[std::to_underlying(side)];
  auto it = std::ranges::find(arr, str[4]);
  if (str.length() > 4 && it != arr.end()) {
    promoted = static_cast<PieceType>(std::to_underlying(PieceBase::kKnight & side) +
                                      std::ranges::distance(arr.begin(), it));
  }
  MoveList list;
  pos.GenerateMoves<MovesType::kLegal>(list);
  for (const auto& move: list.AsSpan()) {
    if (move.get_from() == from && move.get_to() == to &&
       (promoted != PieceType::kNone || promoted == pos.PieceOn(to))) {
      return move;
    }
  }
  
  return Move(); // The move ain't legal
}

}