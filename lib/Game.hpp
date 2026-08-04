#pragma once

#include "Position.hpp"
#include "Display.hpp"
#include "FileManager.hpp"
#include "../core/utils.hpp"

#include <compare>
#include <iostream>

constexpr size_type kMaxMoves = 1000;

inline constexpr std::string_view kDefaultPosition = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

inline constexpr Position kStartingPosition = []() {
  auto res = FenManager::Get(kDefaultPosition);
  if (!res) {
    throw res.error();
  }

  return *res;
}();
  //FenManager::Get(kDefaultPosition); 

enum class Status {
  Win,
  Loss,
  Draw,
  Continue,
};

// std::ostream& operator<<(std::ostream& out, const Parameters& param);

class GameState {
public:
  GameState() = default;

  // explicit GameState(GameState&& other);

  // GameState& operator=(GameState&& other) noexcept;

  GameState(const GameState&);

  GameState& operator=(const GameState&);

  void SetParameters(std::unique_ptr<Parameters> param);

  size_type MoveNumber() const;

  size_type NoCaptureMoves() const;

  bool IsWhiteMove() const;

  bool IsWhiteCastle() const;

  bool IsWhiteShortCastle() const;

  bool IsWhiteLongCastle() const;

  void ViewImageBoard() const;

  void ViewConsoleBoard() const;

  Position GetBoard();

  std::unique_ptr<Parameters>& CurrentState();
  
  Position* board_;

private:
  std::unique_ptr<Parameters> param_;

};

class Game { // +game iterator that works as vector with Position update
public:
  Game() = default;

  Game(const std::vector<std::string>& moves);

  Game(const Game& game);

  Game(const Position& pos);

  //Game& operator=(const Game& game);

  bool operator==(const Game& game);

  bool operator!=(const Game& game);

  void clear();

  void PrintPosition();

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
  std::vector<std::string> moves_;

  Position currPos_;

  size_type current_move_;
  
  Position initPos_ = kStartingPosition;


  //Board* board_;

  //Parameters param_;
};