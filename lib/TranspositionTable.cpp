#include "TranspositionTable.hpp"

#include <ranges>

namespace chess {

TTEntry::TTEntry(const Bitboard key, const int score, const Move move, const uint8_t depth, const TTBound bound)
: key_(key)
, score_(score)
, best_move_(move)
, depth_(depth)
, bound_(bound) {
}

bool TTEntry::is_null() const {
  return bound_ == TTBound::kNone;
}

int TTEntry::get_score(const int ply) const {
  return TTEntry::CalculateScore(score_, ply);
}

int TTEntry::CalculateScore(const int score, const int ply) {
  if (score <= -kInfinity + kMaxDepth) [[unlikely]] {
    return -kInfinity + ply;
  } else if (score >= +kInfinity - kMaxDepth) [[unlikely]] {
    return +kInfinity - ply;
  }

  return score;
}

TranspositionTable::TranspositionTable(const std::size_t size) {
  std::size_t final_size = (1 << (20 + std::bit_width(size) - 1)) / sizeof(TTEntry);
  // std::cout << final_size << " fz\n";
  assert(final_size <= entries_.max_size());
  entries_.resize(final_size);
  mask_ = final_size - 1;
}

void TranspositionTable::clear() {
  std::ranges::fill(entries_, TTEntry());
}

const TTEntry* TranspositionTable::find(const uint64_t key) const {
  const TTEntry& entry = entries_[key & mask_];
  if (entry.is_null() || entry.key_ != key) [[unlikely]] {
    return nullptr;
  }
  return &entry;
}

void TranspositionTable::store(const TTEntry& entry) {
  auto& slot = entries_[entry.key_ & mask_];

  if (slot.is_null() || slot.key_ != entry.key_) [[unlikely]] {
    slot = entry;
    return;
  }
  Move new_best_move = entry.best_move_ ? entry.best_move_ : slot.best_move_;
  slot = entry;// always replace
  slot.best_move_ = new_best_move;
}

void TranspositionTable::resize(const std::size_t sz) {
  clear();
  *this = TranspositionTable(sz);
}

}// namespace chess