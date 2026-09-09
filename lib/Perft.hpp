#pragma once

#include "Move.hpp"
#include "Position.hpp"

#include <chrono>
#include <iostream>

namespace chess {

std::size_t Perft(Position& pos, const std::size_t depth) {
  if (depth == 0) {
    return 1ULL;
  }
  std::size_t nodes = 0;
  MoveList list;
  pos.GenerateMoves<MovesType::kLegal>(list);
  if (depth == 1) {
    // for (const auto& move : list.AsSpan()) {
    //   if (move.is_castle()) {
    //       // std::cout << move << " last move\n";
    //       nodes++;
    //     }
    //   }
    // return nodes;
    return list.size();
  }
  for (const auto& move: list.AsSpan()) {
    pos.MakeMove(move);
    nodes += Perft(pos, depth - 1);
    pos.UnmakeMove(move);
  }
  return nodes;
}

void PerftPrint(Position& pos, const std::size_t depth) {
  if (depth == 0) {
    std::cout << " zeros\n";
    return;
  }
  auto start_time = std::chrono::high_resolution_clock::now();
  std::size_t nodes = 0;
  MoveList list;
  pos.GenerateMoves<MovesType::kLegal>(list);
  for (const auto& move: list.AsSpan()) {
    pos.MakeMove(move);
    std::size_t num = Perft(pos, depth - 1);
    std::cout << move << ": " << num << '\n';
    nodes += num;
    pos.UnmakeMove(move);
  }
  auto end_time = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();
  std::cout << "Depth: " << depth << '\n';
  std::cout << "Nodes: " << nodes << '\n';
  std::cout << "Time: " << duration << '\n';
  if (duration == 0) {
    std::cout << "Too fast to check NPS\n";
  } else {
    std::cout << "NPS: " << (nodes * 1000) / duration << " nodes per second\n";
  }
}

}
