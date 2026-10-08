#pragma once

#include "types/Piece.hpp"

#include "Evaluation.hpp"
#include "Move.hpp"
#include "MoveGenerator.hpp"
#include "Position.hpp"

#include <array>
#include <concepts>
#include <optional>

namespace chess {

inline constexpr int kMaxDepth = 64;

inline constexpr int kMaxKillerMoves = 2;

enum class PickerStage: uint8_t {
  kPrincipalVariation,
  kGenCaptures,
  kGoodCaptures,
  kGenQuiets,
  kQuiets,
  kBadCaptures,
  kNone
};

inline PickerStage& operator++(PickerStage& stage) {
  [[assume(stage != PickerStage::kNone)]];
  stage = static_cast<PickerStage>(std::to_underlying(stage) + 1);

  return stage;
}

class MovePicker {
public:
  MovePicker(const Position& pos);
  MovePicker(const Position& pos, const bool only_captures);
  MovePicker(const Position& pos, const Move pv_move);
  MovePicker(const Position& pos, const std::array<Move, kMaxKillerMoves>& killers);
  MovePicker(const Position& pos, const Move pv_move, const std::array<Move, kMaxKillerMoves>& killers);
  Move YieldMove();
  std::size_t size() const;
  bool empty() const;
  bool empty2() const;
  Move YieldMove2();

private:
  const Position& pos_;
  MoveList<> list_;
  std::size_t ind_ = 0;
  bool only_captures_ = false;

  MoveList<MoveEntry> captures_;
  MoveList<MoveEntry> bad_captures_;
  MoveList<MoveEntry> quiets_;
  std::size_t ind2_ = 0;

  std::array<Move, kMaxKillerMoves> killers_;// it is quiet

  std::size_t left_ = 0;
  std::size_t right_ = 0;

  std::optional<Move> pv_move_;
  PickerStage stage_ = PickerStage::kNone;

  PickerStage stage2_ = PickerStage::kPrincipalVariation;

  void SortOutCapture();
  Move YieldGoodCapture();
  Move YieldQuiet();
  int GetCaptureScore(const Move move) const;
  void ScoreCaptures();
  int GetQuietScore(const Move move) const;
  void ScoreQuiets();

  std::size_t IndMxScore(MoveList<MoveEntry>& list, const std::size_t left, const std::size_t right);
};

}// namespace chess