#include "movement.hpp"

std::vector<std::pair<int, int>> KnightMovement::kKnightMoves = {std::make_pair(-1, 2), std::make_pair(1, 2),
                                                 std::make_pair(-1, -2), std::make_pair(1, -2),
                                                 std::make_pair(2, 1), std::make_pair(2, -1),
                                                 std::make_pair(-2, 1), std::make_pair(-2, -1)};

Movement::Movement(const std::string& init)
: init_(init) {
}

Movement::Movement(const std::string& init, const std::vector<std::pair<int, int>>& moves)
: Movement(init) {
  moves_ = moves;
}

PawnMovement::PawnMovement(const std::string& init)
: Movement(init) {
}

KnightMovement::KnightMovement(const std::string& init)
: Movement(init) {
  moves_ = kKnightMoves;
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

void Movement::Move(const std::string& move) {
  for (const auto& pair: moves_) {
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