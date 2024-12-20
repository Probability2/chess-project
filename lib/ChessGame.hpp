#pragma once
#include "ChessFormat.hpp"

class ChessGame {
public:
  void ViewBoard() const;
private:
  Board board_;
};