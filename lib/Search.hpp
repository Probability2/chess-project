#pragma once

#include "Evaluation.hpp"
#include "MovePicker.hpp"
#include "Position.hpp"

#include <algorithm>
#include <array>
#include <chrono>
#include <ranges>

namespace chess {

using Ms = std::chrono::milliseconds;
using TimePoint = std::chrono::steady_clock::time_point;

constexpr int kInfinity = 100000;

constexpr int kMxChecksExtension = 25;

constexpr int kCheckTimePeriod = 2047;

constexpr std::size_t kMxNodes = std::numeric_limits<std::size_t>::max();

struct NodeInfo {
  int alpha_;
  int beta_;
  std::size_t depth_;
  std::size_t checks_;
};

// enum class LimitTypes: uint8_t {
//   kTimeLimit,
//   kDepthLimit,
//   kNodesLimit,
//   kInfinity
// };

struct SearchLimits {
  // std::chrono::literals start_time;
  Ms move_time_;
  std::size_t depth_ = kMaxDepth;
  std::size_t mx_nodes_ = kMxNodes;
};

class Searcher {
public:
  Searcher();
  Searcher(Position pos);
  void SetPosition(const Position& pos);
  Move GetBestMove(const std::size_t depth);
  Move IterativeBestMove();
  void MakeMove(const Move move);
  void UnmakeMove(const Move move);
  Position& get_position();
  void clear();
  void set_limits(const SearchLimits& limits);
  void flip_side();

private:
  Position pos_;
  bool is_cancelled_ = false;

  SearchLimits limits_;

  TimePoint start_time_;

  std::size_t nodes_ = 0;

  std::array<Move, (kMaxDepth * (kMaxDepth + 1)) / 2> pv_moves_;

  std::array<std::array<Move, kMaxKillerMoves>, kMaxDepth> killers_{};

  int IterativeSearch(NodeInfo info, const int ply, const int pv_index, const bool is_main_line);
  inline bool IsTimeOut() const;
  int QuiescenceSearch(NodeInfo info);
  // int QuiescenceSearch(NodeInfo info, const TimePoint start_time, const Ms move_time);
  int Search(NodeInfo info, const int ply);

  void AddKillerMove(const Move& move, const int ply);

  // void ClearPvArray();
};

}