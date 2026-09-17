#pragma once

#include "Evaluation.hpp"
#include "MovePicker.hpp"
#include "Position.hpp"

#include <algorithm>

namespace chess {

constexpr int kInfinity = 100000;

const int kMxChecksExtension = 10;

int QuiescenceSearch(Position& pos, int alpha, const int beta) {
  int score = eval::Evaluate(pos);
  if (score >= beta) {
    return beta;
  }
  if (score > alpha) {
    alpha = score;
  }
  MovePicker picker(pos, true);
  while (picker.has_next()) {
    Move move = picker.YieldMove();
    pos.MakeMove(move);
    score = -QuiescenceSearch(pos, -beta, -alpha);
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

int Search(Position& pos, const int depth, int alpha, const int beta, int checks) { //negamax
  if (depth == 0 && !pos.is_check()) {
    return QuiescenceSearch(pos, alpha, beta); //till' there are no captures (+maybe checks)
    // return eval::Evaluate(pos);
  }
  checks = (pos.is_check()) ? checks + 1 : checks;
  int extension = (pos.is_check() && checks < kMxChecksExtension) ? 0 : 1;
  MovePicker picker(pos);
  while (picker.has_next()) {
    Move move = picker.YieldMove();
    pos.MakeMove(move);
    int score = -Search(pos, depth - extension, -beta, -alpha, checks);
    pos.UnmakeMove(move);
    if (score >= beta) {
      return beta;
    }
    if (score > alpha) {
      alpha = score;
    }
  }
  if (picker.empty()) {
    if (pos.is_check()) {
      return -kInfinity - depth;
    }
    return 0;
  }

  return alpha;
}

Move GetBestMove(Position pos, const int depth) {
  if (depth == 0) {
    return Move();
  }
  MovePicker picker(pos);
  Move best_move = Move();
  int alpha = -kInfinity;
  int beta = +kInfinity;
  while (picker.has_next()) {
    Move move = picker.YieldMove();
    pos.MakeMove(move);
    int score = -Search(pos, depth - 1, -beta, -alpha, 0);
    pos.UnmakeMove(move);
    if (score > alpha) {
      alpha = score;
      best_move = move;
    }
  }
  if (best_move || picker.empty()) {
    return best_move;
  }

  return picker[0];
}

}