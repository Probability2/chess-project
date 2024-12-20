#include "ChessPiece.hpp"

King::King(const std::string& pos) {
  pos_ = pos;
}

void King::SetPosition(const std::string& pos) {
  pos_ = pos;
}

std::string King::GetPosition() const {
  return pos_;
}

void Knight::Move(std::size_t ind, const std::string& new_pos) {

}

std::vector<std::string> ChessPiece::GetPositions() const {
  // std::cout << "POSITIONS:\n";
  // for (std::size_t i = 0; i < poss_.size(); ++i) {
  //   std::cout << poss_[i] << ' ';
  // }
  // std::cout << '\n';

  return poss_;
}

void ChessPiece::PrintPositions() const {
  for (auto& pos: poss_) {
    std::cout << pos << '\n';
  }
}

std::size_t ChessPiece::size() const {
  return poss_.size();
}

Pawn& Pawn::operator=(const Pawn& pawn) {
  if (this != &pawn) {
    for (auto& pos: pawn.poss_) {
      this->poss_.push_back(pos);
    }
  }

  return *this;
}

Knight& Knight::operator=(const Knight& knight) {
  if (this != &knight) {
    for (auto& pos: knight.poss_) {
      this->poss_.push_back(pos);
    }
  }

  return *this;
}

Bishop& Bishop::operator=(const Bishop& bishop) {
  if (this != &bishop) {
    for (auto& pos: bishop.poss_) {
      this->poss_.push_back(pos);
    }
  }

  return *this;
}

Rook& Rook::operator=(const Rook& rook) {
  if (this != &rook) {
    for (auto& pos: rook.poss_) {
      this->poss_.push_back(pos);
    }
  }

  return *this;
}

Quenn& Quenn::operator=(const Quenn& quenn) {
  if (this != &quenn) {
    for (auto& pos: quenn.poss_) {
      this->poss_.push_back(pos);
    }
  }

  return *this;
}

Knight Knight::operator+(const Knight& knight) const {
  Knight res;
  for (auto& pos: this->poss_) {
    res.poss_.push_back(pos);
  }
  for (auto& pos: knight.poss_) {
    res.poss_.push_back(pos);
  }

  return res;
}

Pawn Pawn::operator+(const Pawn& pawn) const {
  Pawn res;
  for (auto& pos: this->poss_) {
    res.poss_.push_back(pos);
  }
  for (auto& pos: pawn.poss_) {
    res.poss_.push_back(pos);
  }

  return res;
}

Bishop Bishop::operator+(const Bishop& bishop) const {
  Bishop res;
  for (auto& pos: this->poss_) {
    res.poss_.push_back(pos);
  }
  for (auto& pos: bishop.poss_) {
    res.poss_.push_back(pos);
  }

  return res;
}

Rook Rook::operator+(const Rook& rook) const {
  Rook res;
  for (auto& pos: this->poss_) {
    res.poss_.push_back(pos);
  }
  for (auto& pos: rook.poss_) {
    res.poss_.push_back(pos);
  }

  return res;
}

Quenn Quenn::operator+(const Quenn& quenn) const {
  Quenn res;
  for (auto& pos: this->poss_) {
    res.poss_.push_back(pos);
  }
  for (auto& pos: quenn.poss_) {
    res.poss_.push_back(pos);
  }

  return res;
}

void ChessPiece::CheckCorrectness(const std::optional<std::string>& pos) const {
  if (!pos.has_value()) {
    std::cerr << "no value\n";
  }
}

void Knight::PrintPossMoves() const {
  
}