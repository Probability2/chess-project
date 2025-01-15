#include "ChessGame.hpp"

Parameters::Parameters(const Parameters& param) {
  *this = param;
}

Parameters& Parameters::operator=(const Parameters& param) {
  this->is_en_passant = param.is_en_passant;
  this->is_long_castle_ = param.is_long_castle_;
  this->is_short_castle_ = param.is_short_castle_;
  this->is_white_move = param.is_white_move;
  this->move = param.move;
  this->no_capture_moves = param.no_capture_moves;

  return *this;
}

void Parameters::SetDefault() {
  bool is_short_castle_ = true;
  bool is_long_castle_ = true;
  bool is_white_move = true;
  bool is_en_passant = false;
  size_type no_capture_moves = 0;
  size_type move = 0;
}

Game::Game(const std::vector<std::string>& moves)
: moves_(moves) {
}

Game::Game(const Game& game) {
  *this = game;
}

Game& Game::operator=(const Game& game) {
  this->moves_ = game.moves_;
  //this->param_ = game.param_;
  //this->board_ = game.board_;

  return *this;
}

bool Game::operator==(const Game& game) {
  return std::equal(this->begin(), this->end(), game.cbegin(), game.cend());
}

GameState::GameState(GameState&& other)
: board_(std::move(other.board_)) {
};

GameState& GameState::operator=(GameState&& other) noexcept {
  if (this != &other) {
    board_ = std::move(other.board_);
  }
    
  return *this;
};

bool Game::operator!=(const Game& game) {
  return !(*this == game);
}

size_type Game::size() const {
  return moves_.size();
}

void GameState::ViewConsoleBoard() const {
  ConsoleDefault console(*board_);
  console.Print();
}

void GameState::ViewImageBoard() const {
  ConsoleImage console(*board_);
  console.Print();
}

std::unique_ptr<Board>& GameState::GetBoard() {
  return board_;
}

bool GameState::IsCastle() const {
  return param_.is_short_castle_ || param_.is_long_castle_;
}

size_type GameState::MoveNumber() const {
  return param_.move;
}

size_type GameState::NoCaptureMoves() const {
  return param_.no_capture_moves;
}

bool GameState::IsWhiteMove() const {
  return param_.is_white_move;
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

size_type Game::max_size() const {
  return kMaxMoves;
}

void Game::destroy() {
  moves_.clear();
  //GameState::param_.SetDefault();
  //board_ = nullptr;
}

void Game::clear() {
  this->destroy();
}