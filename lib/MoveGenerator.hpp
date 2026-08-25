#pragma once

#include "Position.hpp"

#include <utility>

namespace chess::move_generator {

template<MovesType type>
MoveList GenerateMoves(const Position& pos);

template<ColorType Color>
bool IsLegal(const Position& pos, const Move& move);

}// namespace chess::move_generator