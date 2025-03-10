#include "ChessGame.hpp"

Parameters::Parameters(const Parameters& param) {
  *this = param;
}

Parameters& Parameters::operator=(const Parameters& param) {
  this->is_en_passant_ = param.is_en_passant_;
  this->is_white_long_castle_ = param.is_white_long_castle_;
  this->is_white_short_castle_ = param.is_white_short_castle_;
  this->is_black_long_castle_ = param.is_black_long_castle_;
  this->is_black_short_castle_ = param.is_black_short_castle_;
  this->is_white_move_ = param.is_white_move_;
  this->move_ = param.move_;
  this->no_capture_moves_ = param.no_capture_moves_;

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

void Parameters::SetWhiteShortCastle(const bool is) {
  is_white_short_castle_ = is;
}

void Parameters::SetWhiteLongCastle(const bool is) {
  is_white_long_castle_ = is;
}

void Parameters::SetBlackShortCastle(const bool is) {
  is_black_short_castle_ = is;
}

void Parameters::SetBlackLongCastle(const bool is) {
  is_black_long_castle_ = is;
}

void Parameters::SetWhiteMove(const bool is) {
  is_white_move_ = is;
}

void Parameters::SetEnPassant(const bool is) {
  is_en_passant_ = is;
}

void Parameters::SetNoCaptures(const size_type moves) {
  no_capture_moves_ = moves;
}

void Parameters::SetMove(const size_type moves) {
  move_ = moves;
}

bool Parameters::IsWhiteShortCastle() const {
  return is_white_short_castle_;
}

bool Parameters::IsWhiteLongCastle() const {
  return is_white_long_castle_;
}

bool Parameters::IsBlackShortCastle() const {
  return is_black_short_castle_;
}

bool Parameters::IsBlackLongCastle() const {
  return is_black_long_castle_;
}

bool Parameters::IsWhiteMove() const {
  return is_white_move_;
}

bool Parameters::IsEnPassant() const {
  return is_en_passant_;
}

GameState::GameState()
: board_(std::make_unique<Board>()) {
}

void GameState::SetParameters(const Parameters& param) {
  this->param_ = param;
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

Parameters GameState::GetParameters() const {
  return param_;
}

void GameState::ViewImageBoard() const {
  ConsoleImage console(*board_);
  console.Print();
}

std::unique_ptr<Board>& GameState::GetBoard() {
  return board_;
}

bool GameState::IsWhiteCastle() const {
  return this->IsWhiteShortCastle() || this->IsWhiteLongCastle();
}

bool GameState::IsWhiteShortCastle() const {
  return param_.is_white_short_castle_;
}

bool GameState::IsWhiteLongCastle() const {
  return param_.is_white_long_castle_;
}

size_type GameState::MoveNumber() const {
  return param_.move_;
}

size_type GameState::NoCaptureMoves() const {
  return param_.no_capture_moves_;
}

bool GameState::IsWhiteMove() const {
  return param_.is_white_move_;
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