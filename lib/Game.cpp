#include "Game.hpp"

Game::Game(const std::vector<uint8_t>& moves)
: moves_(moves) {
}

Game::Game(const Game& game) {
  *this = game;
}

// Game& Game::operator=(const Game& game) {
//   this->moves_ = game.moves_;
//   //this->param_ = game.param_;
//   //this->board_ = game.board_;

//   return *this;
// }

bool Game::operator==(const Game& game) {
  return std::equal(this->begin(), this->end(), game.cbegin(), game.cend());
}

bool Game::operator!=(const Game& game) {
  return !(*this == game);
}

size_type Game::size() const {
  return moves_.size();
}

Game::Game(const Position& pos)
: initPos_(pos) {
}

Position Game::get_current_position() const {
  return currPos_;
}

Position Game::get_init_position() const {
  return initPos_;
}

void Game::PrintPosition() const {
  std::cout << currPos_;
}

iterator Game::begin() {
  return moves_.begin();
}

iterator Game::end() {
  return moves_.end();
}

const_iterator Game::cbegin() const {
  return moves_.cbegin();
}

const_iterator Game::cend() const {
  return moves_.cend();
}

bool Game::empty() const {
  return (size() == 0);
}

void Game::clear() {
  //nulling the game, there will be only initial position??
}