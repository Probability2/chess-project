#include "movement.hpp"

std::vector<std::pair<int, int>> KnightMovement::kKnightMoves{std::make_pair(-1, 2), std::make_pair(1, 2),
                                                              std::make_pair(-1, -2), std::make_pair(1, -2),
                                                              std::make_pair(2, 1), std::make_pair(2, -1),
                                                              std::make_pair(-2, 1), std::make_pair(-2, -1)};

std::vector<std::pair<int, int>> kKingMoves{{1, 1}, {1, 0}, {1, -1}, {0, 1}, {0, -1}, {-1, 0}, {-1, -1}, {-1, 1}};

Movement::Movement(const std::string& init)
: init_(init) {
}

Movement::Movement(const std::string& init, const std::vector<std::pair<int, int>>& moves)
: Movement(init) {
}

PawnMovement::PawnMovement(const std::string& init)
: Movement(init) {
}

KnightMovement::KnightMovement(const std::string& init)
: Movement(init) {
}

BishopMovement::BishopMovement(const std::string& init)
: Movement(init) {
}

RookMovement::RookMovement(const std::string& init)
: Movement(init) {
}

QueenMovement::QueenMovement(const std::string& init)
: Movement(init) {
}

KingMovement::KingMovement(const std::string& init)
: Movement(init) {
}

void PawnMovement::Move(const std::string& move) {

}

void KnightMovement::Move(const std::string& move) {
  for (const auto& pair: kKnightMoves) {
    if (move[0] - init_[0] == pair.first && move[1] - init_[1] == pair.second) {
      init_ = move;
    }
  }
}

void BishopMovement::Move(const std::string& move) {
  if (move[0] - init_[0] == move[1] - init_[1]) {
    init_ = move;
  }
}

void RookMovement::Move(const std::string& move) {
  if (move[0] - init_[0] == 0 || move[1] - init_[1] == 0) {
    init_ = move;
  }
}

void QueenMovement::Move(const std::string& move) {
  if (move[0] - init_[0] == move[1] - init_[1]) {
    init_ = move;
  }
  if (move[0] - init_[0] == 0 || move[1] - init_[1] == 0) {
    init_ = move;
  }
}

void KingMovement::Move(const std::string& move) {
  for (const auto& pair: kKingMoves) {
    if (move[0] - init_[0] == pair.first && move[1] - init_[1] == pair.second) {
      init_ = move;
    }
  }
}

void PawnMovement::ShowPossMoves() {

}

void KnightMovement::ShowPossMoves() {
  
}

void BishopMovement::ShowPossMoves() {
  
}

void RookMovement::ShowPossMoves() {
  
}

void QueenMovement::ShowPossMoves() {
  
}

void KingMovement::ShowPossMoves() {
  
}