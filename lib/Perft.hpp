#pragma once

#include "Move.hpp"
#include "Position.hpp"

#include <chrono>
#include <iostream>

namespace chess {

void Perft(Position& pos, const std::size_t curr_depth,
           const std::size_t max_depth, std::size_t& nodes) {
  if (curr_depth == max_depth) {
    nodes++;
    return;
  }
  MoveList list;
  pos.GenerateMoves<MovesType::kLegal>(list);
  for (const auto& move : list.AsSpan()) {
    pos.MakeMove(move);
    Perft(pos, curr_depth + 1, max_depth, nodes);
    pos.UnmakeMove(move);
  }
}

void PerftPrint(Position& pos, const std::size_t depth) {
  if (depth == 0) {
    std::cout << " zeros\n";
    return;
  }
  std::size_t nodes = 0;
  auto start_time = std::chrono::high_resolution_clock::now();
  Perft(pos, 0, depth, nodes);
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

std::size_t Perft(Position& pos, const std::size_t depth) {
  if (depth == 0) {
    return 1;
  }
  std::size_t nodes = 0;
  Perft(pos, 0, depth, nodes);
  
  return nodes;
}

}
