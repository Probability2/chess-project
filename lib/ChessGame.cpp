#include "ChessGame.hpp"

std::size_t Game::size() const {
  return moves.size();
}

void Game::ViewConsoleBoard() const {
  ConsoleDefault console(*board_);
  console.Print();
}

void Game::ViewImageBoard() const {
  ConsoleImage console(*board_);
  console.Print();
}

Board* Game::GetBoard() {
  return board_;
}

bool Game::IsCastle() const {
  return param_.is_short_castle_ || param_.is_long_castle_;
}

std::size_t Game::MoveNumber() const {
  return param_.move;
}

std::size_t Game::NoCaptureMoves() const {
  return param_.no_capture_moves;
}

bool Game::IsWhiteMove() const {
  return param_.is_white_move;
}