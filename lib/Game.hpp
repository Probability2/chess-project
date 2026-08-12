#pragma once

#include "Position.hpp"
#include "FileManager.hpp"
#include "../core/utils.hpp"

#include <compare>
#include <iostream>

namespace chess {

inline constexpr std::string_view kDefaultPosition = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

inline constexpr Position kStartingPosition = []() {
  auto res = fen_manager::Get(kDefaultPosition);
  if (!res) {
    throw res.error();
  }

  return *res;
}();

enum class GameStatus: uint8_t {
  kWin,
  kLoss,
  kDraw,
  kContinue,
};

// std::ostream& operator<<(std::ostream& out, const Parameters& param);


class Game { // +game iterator that works as vector with Position update
public:
  Game() = default;

  Game(const std::vector<uint8_t>& moves);

  Game(const Game& game);

  Game(const Position& pos);

  //Game& operator=(const Game& game);

  bool operator==(const Game& game);

  bool operator!=(const Game& game);

  void clear();

  void PrintPosition() const;

  Position get_current_position() const;

  Position get_init_position() const;

  size_type size() const;

  iterator begin();

  iterator end();

  const_iterator cbegin() const;

  const_iterator cend() const;

  bool empty() const;

  Game& operator++() const;

  Game& operator++(int) const;

  Game& operator--() const;

  Game& operator--(int) const;

private:
  std::vector<uint8_t> moves_;
  Position initPos_ = kStartingPosition;
  Position currPos_ = initPos_;
  size_type current_move_;
  std::optional<std::string> player1_;
  std::optional<std::string> player2_;
};

}// namespace chess