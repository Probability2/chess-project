#pragma once

#include "types/Bitboard.hpp"
#include "types/EvaluationTables.hpp"
#include "types/Piece.hpp"

#include "Position.hpp"

namespace chess::eval {

int Evaluate(const Position& pos);

}// namespace chess::eval