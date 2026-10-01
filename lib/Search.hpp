#pragma once

#include "Evaluation.hpp"
#include "MovePicker.hpp"
#include "Position.hpp"

#include <algorithm>
#include <chrono>
#include <ranges>

namespace chess {

using Ms = std::chrono::milliseconds;
using TimePoint = std::chrono::steady_clock::time_point;

constexpr int kInfinity = 100000;

constexpr int kMxChecksExtension = 25;

constexpr int kCheckTimePeriod = 2000;

struct NodeInfo {
  int alpha_;
  int beta_;
  std::size_t depth_;
  std::size_t checks_;
};

class Searcher {
public:
  Searcher(Position pos);
  Move GetBestMove(const std::size_t depth);
  Move IterativeBestMove(const Ms move_time);
  void MakeMove(const Move move);
  void UnmakeMove(const Move move);
  Position& get_position();

private:
  Position pos_;
  bool is_time_out_ = false;
  std::size_t nodes_ = 0;
  // std::size_t checks = 0;

  std::array<Move, (kMaxDepth * (kMaxDepth + 1)) / 2> pv_moves_;

  int IterativeSearch(NodeInfo info, TimePoint start_time, const Ms move_time, const int ply, const int pv_index,
                                                                               const bool is_main_line);
  inline bool IsTimeOut(const TimePoint start_time, const Ms duration);
  int QuiescenceSearch(NodeInfo info);
  int Search(NodeInfo info, const int ply);

  // void ClearPvArray();
};

}