#pragma once

#include "ChessFormat.hpp"

class Game {
public:
  void ViewBoard() const;
private:
  std::vector<std::string> moves;

  Board board_;
};