#include "Position.hpp"

bool Position::is_white_move() const {
  return isWhiteMove_;
}

bool Position::is_en_passant() const {
  return target_ != 0;
}

std::size_t Position::get_no_capture_moves() const {
  return no_capture_moves_;
}

std::size_t Position::get_move_number() const {
  return move_;
}

std::optional<std::string> Position::get_castle() const {
  std::string castling_rights;
  for (int i = 0; i < 4; ++i) {
    if ((castle_ >> i) & 1) {
      castling_rights += kCastles[i];
    }
  }
  if (castling_rights.empty()) {
    return std::nullopt;
  }

  return std::make_optional(castling_rights);
}

std::optional<std::string> Position::get_en_passant() const {
  if (target_ == 0) {
    return std::nullopt;
  }
  std::string s;
  s += ((target_ >> 3) & 0x07) + 'a';
  s += (target_ & 0x07) + '1';

  return std::make_optional(s);
}

std::ostream& operator<<(std::ostream& os, const Position& pos) {
  for (int i = kMaxInd - 1; i >= 0; --i) {
    for (int j = 0; j < kMaxInd; ++j) {
      os << GetPieceIcon(pos.get_square(i, j)) << ' ';
    }
    os << '\n';
  }
  os << (pos.is_white_move() ? "White's move, " : "Black's move, ") << pos.get_castle().value_or(" ") << ", ";
  auto en_passant = pos.get_en_passant();
  if (en_passant.has_value()) {
    os << "en-passant: " << *en_passant;
  } else {
    os << "no en-passant";
  }
  os << ", no capture moves: " << pos.get_no_capture_moves() << ", move number: " << pos.get_move_number() << '\n';

  return os;
}
