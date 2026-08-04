#include "Position.hpp"

bool Parameters::IsWhiteMove() const {
  return isWhiteMove_;
}

bool Parameters::IsEnPassant() const {
  return target_ != 0;
}

std::size_t Parameters::GetNoCaptureMoves() const {
  return no_capture_moves_;
}

std::size_t Parameters::GetMove() const {
  return move_;
}

uint8_t Parameters::GetCastle() const {
  return castle_;
}

std::optional<std::pair<int, int>> Parameters::GetEnPassant() const {
  if (target_ == 0) {
    return std::nullopt;
  }
  
  return std::make_optional(std::make_pair(((target_ >> 3) & 0x07) + 'a', (target_ & 0x07)));
}

Parameters Position::GetParameters() const {
  return param_;
}

std::ostream& operator<<(std::ostream& os, const Position& pos) {
  for (int i = 7; i >= 0; --i) {
    for (int j = 0; j < 8; ++j) {
      os << GetPieceIcon(pos.GetSquare(i, j)) << ' ';
    }
    os << '\n';
  }
  os << pos.GetParameters();

  return os;
}

std::ostream& operator<<(std::ostream& os, const Parameters& param) {
  if (param.IsWhiteMove()) {
    os << "White's move, ";
  } else {
    os << "Black's move, ";
  }
  uint8_t is_castle = param.GetCastle();
  if (!is_castle) {
    os << "no castle";
  }
  for (int i = 0; i < 4; ++i) {
    if ((is_castle >> i) & 1) {
      os << kCastles[i];
    }
  }
  os << ", ";
  std::optional<std::pair<char, char>> en_passant = param.GetEnPassant();
  if (en_passant.has_value()) {
    auto [col, row] = *en_passant;
    os << "en-passant: " << col << static_cast<int>(row);
  } else {
    os << "no en-passant";
  }
  os << ", no capture moves: " << param.GetNoCaptureMoves() << ", move number: " << param.GetMove() << '\n';

  return os;
}