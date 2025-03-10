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

  bool IsWhiteShortCastle() const;

  bool IsWhiteLongCastle() const;

  bool IsBlackShortCastle() const;

  bool IsBlackLongCastle() const;

  bool IsWhiteMove() const;

  bool IsEnPassant() const;

  void SetWhiteShortCastle(const bool is);

  void SetWhiteLongCastle(const bool is);
  
  void SetBlackShortCastle(const bool is);

  void SetBlackLongCastle(const bool is);

  void SetWhiteMove(const bool is);

  void SetEnPassant(const bool is);

  void SetNoCaptures(const size_type moves);

  void SetMove(const size_type moves);

  bool is_white_short_castle_{true};
  bool is_white_long_castle_{true};
  bool is_black_short_castle_{true};
  bool is_black_long_castle_{true};
  bool is_white_move_{true};
  bool is_en_passant_{false};
  size_type no_capture_moves_{0};
  size_type move_{0};
};

class GameState {
public:
  GameState();

  explicit GameState(GameState&& other);

  GameState& operator=(GameState&& other) noexcept;

  GameState(const GameState&) = delete;

  GameState& operator=(const GameState&) = delete;

  void SetParameters(const Parameters& param);

  std::unique_ptr<Board> board_;

  size_type MoveNumber() const;

  size_type NoCaptureMoves() const;

  bool IsWhiteMove() const;

  bool IsWhiteCastle() const;

  bool IsWhiteShortCastle() const;

  bool IsWhiteLongCastle() const;

  void ViewImageBoard() const;

  void ViewConsoleBoard() const;

  std::unique_ptr<Board>& GetBoard();

  Parameters GetParameters() const;

private:
  Parameters param_;

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