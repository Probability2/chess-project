#pragma once

#include "Piece.hpp"
#include "Position.hpp"
#include "../core/utils.hpp"

#include <utility>

namespace chess::move_generator {

template<MovesType type>
MoveList GenerateMoves(const Position& pos);

}// namespace chess::move_generator