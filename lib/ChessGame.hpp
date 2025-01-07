#pragma once

#include "ChessFormat.hpp"
#include "DisplayBoard.hpp"
#include "../core/aliases.hpp"

#include <compare>

constexpr size_type kMaxMoves = 1000;

enum class Status {
  WIN,
  LOSS,
  DRAW,
  CONTINUE,
};

struct Parameters {
  Parameters() = default;

  Parameters(const Parameters& param);

  Parameters& operator=(const Parameters& param);

  void SetDefault();

  bool is_short_castle_{true};
  bool is_long_castle_{true};
  bool is_white_move{true};
  bool is_en_passant{false};
  size_type no_capture_moves{0};
  size_type move{0};
};

struct GameState {
  Parameters param_;

  Board* board_;

  size_type MoveNumber() const;

  size_type NoCaptureMoves() const;

  bool IsWhiteMove() const;

  bool IsCastle() const;

  void ViewImageBoard() const;

  void ViewConsoleBoard() const;

  Board* GetBoard();
};

class Game {
public:
  Game() = default;

  Game(const std::vector<std::string>& moves);

  Game(const Game& game);

  class GameState;

  Game& operator=(const Game& game);

  friend std::strong_ordering operator<=>(Game lhs, Game rhs) {
    if (lhs.size() < rhs.size())
      return std::strong_ordering::less;
    if (lhs.size() > rhs.size())
      return std::strong_ordering::greater;
    return std::strong_ordering::equal;
  }

  bool operator==(const Game& game);

  bool operator!=(const Game& game);

  void destroy();

  void clear();

  size_type size() const;

  iterator begin();

  iterator end();

  const_iterator cbegin() const;

  const_iterator cend() const;

  bool empty() const;

  size_type max_size() const;

private:
  std::vector<std::string> moves_;

  //Board* board_;

  //Parameters param_;
};