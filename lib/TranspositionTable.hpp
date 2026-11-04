#pragma once

#include "types/Bitboard.hpp"
#include "Move.hpp"

#include <array>
#include <bit>
#include <vector>

namespace chess {

constexpr int kMaxDepth = 64;

constexpr std::size_t kDefaultTTSize = 8;

constexpr int kInfinity = 100000;

enum class TTBound: uint8_t {
  kNone,
  kLower,
  kUpper,
  kExact
};

struct TTEntry {
  TTEntry() = default;
  TTEntry(const Bitboard key, const int score, const Move move, const uint8_t depth,
                                                                const TTBound bound = TTBound::kNone);

  bool is_null() const;
  int get_score(const int ply) const;

  static int CalculateScore(const int score, const int ply);

  uint64_t key_;
  int score_;
  Move best_move_;
  uint8_t depth_;
  TTBound bound_ = TTBound::kNone;
};

class TranspositionTable {
public:
  TranspositionTable(const std::size_t size = kDefaultTTSize);
  void clear();
  const TTEntry* find(const uint64_t key) const;
  void store(const TTEntry& entry);
  void resize(const std::size_t sz);

private:
  std::vector<TTEntry> entries_;
  std::size_t mask_ = (1ULL << (sizeof(kDefaultTTSize) + 1)) - 1;
};

}// namespace chess