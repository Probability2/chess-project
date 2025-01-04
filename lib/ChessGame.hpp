#pragma once

#include "ChessFormat.hpp"
#include "DisplayBoard.hpp"

struct Parameters {
  bool is_short_castle_ = true;
  bool is_long_castle_ = true;
  bool is_white_move = true;
  bool is_en_passant = false;
  std::size_t no_capture_moves = 0;
  std::size_t move = 0;
};

class Game {
public:
  Game() = default;

  void ViewConsoleBoard() const;

  void ViewImageBoard() const;

  std::size_t size() const;

  Board* GetBoard();

  bool IsCastle() const;

  std::size_t MoveNumber() const;

  std::size_t NoCaptureMoves() const;
  
  bool IsWhiteMove() const;

private:
  std::vector<std::string> moves;

  Board* board_;

  Parameters param_;
};