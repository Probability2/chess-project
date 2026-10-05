#pragma once

#include "Move.hpp"
#include "MoveGenerator.hpp"
#include "Position.hpp"
#include "MovePicker.hpp"

#include <chrono>
#include <iostream>
#include <set>

namespace chess {

std::size_t Perft(Position pos, const std::size_t depth) {
  if (depth == 0) {
    return 1ULL;
  }
  std::size_t nodes = 0;
  MoveList list;
  move_generator::GenerateMoves<MovesType::kLegal>(pos, list);
  if (depth == 1) {
    return list.size();
  }
  for (const auto& move: list.AsSpan()) {
    pos.MakeMove(move);
    nodes += Perft(pos, depth - 1);
    pos.UnmakeMove(move);
  }
  return nodes;
}

void PrintPerft(Position pos, const std::size_t depth) {
  if (depth == 0) {
    std::cout << " zeros\n";
    return;
  }
  auto start_time = std::chrono::steady_clock::now();
  std::size_t nodes = 0;
  MoveList list;
  move_generator::GenerateMoves<MovesType::kLegal>(pos, list);
  for (const auto& move: list.AsSpan()) {
    pos.MakeMove(move);
    std::size_t num = Perft(pos, depth - 1);
    std::cout << move << ": " << num << '\n';
    nodes += num;
    pos.UnmakeMove(move);
  }
  auto end_time = std::chrono::steady_clock::now();
  auto duration = end_time - start_time;
  double seconds = std::chrono::duration<double>(duration).count();
  // std::println("Depth: {}", depth);
  std::cout << "Depth: " << depth << '\n';
  std::cout << "Nodes: " << nodes << '\n';
  std::cout << "Time: " << seconds << "s\n";
  if (seconds == 0) {
    std::cout << "Too fast to check NPS\n";
  } else {
    std::cout << "NPS: " << static_cast<int>(nodes / seconds) << " nodes per second\n";
  }
}

}
