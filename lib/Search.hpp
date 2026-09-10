#pragma once

#include "Evaluation.hpp"
#include "Position.hpp"

#include <algorithm>

namespace chess {

constexpr int kInfinity = 100000;

int Search(Position& pos, const int depth, int alpha, int beta) { //negamax
  if (depth == 0) {
    //return QuiescenceSearch(alpha, beta); //till' there are no captures (+maybe checks)
    return eval::Evaluate(pos);
  }
  MoveList list;
  pos.GenerateMoves<MovesType::kLegal>(list);
  if (list.empty()) {
    if (pos.is_check()) {
      return -kInfinity - depth;
    }
    return 0;
  }
  for (const auto& move: list.AsSpan()) {
    pos.MakeMove(move);
    int score = -Search(pos, depth - 1, -beta, -alpha);
    pos.UnmakeMove(move);
    if (score >= beta) {
      return beta;
    }
    if (score > alpha) {
      alpha = score;
    }
  }

  return alpha;
}

Move GetBestMove(Position& pos, const int depth) {
  if (depth == 0) {
    return Move();
  }
  MoveList list;
  pos.GenerateMoves<MovesType::kLegal>(list);
  if (list.empty()) {
    return Move();
  }
  Move best_move = list[0];
  int alpha = -kInfinity;
  int beta = +kInfinity;
  for (const auto& move: list.AsSpan()) {
    pos.MakeMove(move);
    int score = -Search(pos, depth - 1, -beta, -alpha);
    pos.UnmakeMove(move);
    if (score > alpha) {
      alpha = score;
      best_move = move;
    }
  }

  return best_move;
}

}