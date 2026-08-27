#pragma once

#include "Position.hpp"

#include <utility>

namespace chess::move_generator {

template<MovesType type>
MoveList GenerateMoves(const Position& pos);

template<ColorType Color>
bool IsLegal(const Position& pos, const Move& move);

template<ColorType Color>
Bitboard GetPinnedPieces(const Position& pos);

template<ColorType Color>
Bitboard GetKingAttackers(const Position& pos, const uint8_t king_square);

}// namespace chess::move_generator