#pragma once

#include "types/Bitboard.hpp"
#include "Position.hpp"

#include <algorithm>
#include <concepts>
#include <expected>
#include <filesystem>
#include <format>
#include <fstream>
#include <ranges>

namespace fs = std::filesystem;

namespace chess::fen_manager {

enum class ParseError: uint8_t {
  kFileNotFound,
  kPermissionDenied,
  kSizeTooLarge,
  kDataIsDamaged,
  kNamesOccupied,
  kUnknownError
};

constexpr std::string_view to_string(ParseError code) {
  switch (code) {
    case ParseError::kFileNotFound: return "File is not found";
    case ParseError::kPermissionDenied: return "The permission to the file denied";
    case ParseError::kSizeTooLarge: return "The size of given file is too large";
    case ParseError::kDataIsDamaged: return "The data is damaged";
    case ParseError::kNamesOccupied: return "Unable to create new file because the names are already occupied";
    case ParseError::kUnknownError: return "Unknown error";
  }
  std::unreachable();
}

namespace internal {

constexpr char kFenDelimeter = '/';

constexpr char kSpaceDelimiter = ' ';

constexpr std::size_t kMaxFenSize = 100;

constexpr std::expected<void, ParseError> ReadPlacement(std::string_view& data,
                                                              const uint8_t coord,
                                                              Position& pos) {
  uint8_t col = 0;
  while (!data.empty() && data.front() != kFenDelimeter && data.front() != kSpaceDelimiter && col < 8) {
    char s = data.front();
    if (s > '0' && s < '9' && col + (s - '0') <= 8) {
      pos.SetSquares(PieceType::kNone, coord, col, s - '0');
      col += (s - '0');
    } else if (std::ranges::contains(kPieceSymbols, s) && col <= 7) {
      pos.SetSquare(GetPieceType(s), coord, col);
      col++;
    } else {
      return std::unexpected(ParseError::kDataIsDamaged);
    }
    data.remove_prefix(1);
  }
  if (col != 8) {
    return std::unexpected(ParseError::kDataIsDamaged);
  }
  if (!data.empty()) {
    data.remove_prefix(1);
  }

  return {};
}

constexpr std::expected<void, ParseError> ParseTurn(std::string_view& data, Position& pos) {
  if (data.empty() || (data.front() != 'w' && data.front() != 'b')) {
    return std::unexpected(ParseError::kDataIsDamaged);
  }
  pos.set_white_move(data.front() == 'w');
  data.remove_prefix(2);

  return {};
}

constexpr std::expected<void, ParseError> ParseCastle(std::string_view& data, Position& pos) {
  if (data.empty()) {
    return std::unexpected(ParseError::kDataIsDamaged);
  }
  if (data.front() == '-') {
    data.remove_prefix(2);
    return {};
  }
  for (std::size_t i = 0; i < kMxCastles && !data.empty(); ++i) {
    auto it = std::ranges::find(kCastleChars, data.front());
    if (it == kCastleChars.end()) {
      break;
    }
    data.remove_prefix(1);
    pos.set_castling(std::ranges::distance(kCastleChars.begin(), it));
  }
  if (data.empty()) {
    return std::unexpected(ParseError::kDataIsDamaged);
  }
  data.remove_prefix(1);

  return {};
}

constexpr std::expected<void, ParseError> ParseEnPassant(std::string_view& data, Position& pos) {
  if (data.empty() || ((data.front() < 'a' || data.front() > 'h') && data.front() != '-')) {
    return std::unexpected(ParseError::kDataIsDamaged);
  }
  if (data.front() == '-') {
    data.remove_prefix(2);
    return {};
  }
  int x = data.front() - 'a';
  data.remove_prefix(1);
  if (x < 0 || x > 8 || data.empty() || data.front() < '1' || data.front() > '8') {
    return std::unexpected(ParseError::kDataIsDamaged);
  }
  pos.set_en_passant(coord(data.front() - '1', x));
  data.remove_prefix(2);

  return {};
}

constexpr std::expected<void, ParseError> ParseNoCaptures(std::string_view& data, Position& pos) {
  if (data.empty()) {
    return {};
  }
  int num = 0;
  auto res = std::from_chars(data.data(), data.data() + data.length(), num);
  data.remove_prefix(res.ptr - data.data() + 1);
  if (num < 0) {
    return std::unexpected(ParseError::kDataIsDamaged);
  }
  pos.set_no_captures(num);

  return {};
}

constexpr std::expected<void, ParseError> ParseMoveNumber(std::string_view& data, Position& pos) {
  if (data.empty()) {
    return {};
  }
  int num = 0;
  auto res = std::from_chars(data.data(), data.data() + data.length(), num);
  if (num < 0) {
    return std::unexpected(ParseError::kDataIsDamaged);
  }
  pos.set_move_number(num);
  data.remove_prefix(1);
  
  return {};
}

constexpr std::expected<void, ParseError> ParseParameters(std::string_view& data, Position& pos) {
  if (!ParseTurn(data, pos) || !ParseCastle(data, pos) || !ParseEnPassant(data, pos) ||
      !ParseNoCaptures(data, pos) || !ParseMoveNumber(data, pos)) {
    return std::unexpected(ParseError::kDataIsDamaged);
  }

  return {};
}

}// namespace internal

constexpr std::expected<Position, ParseError> Parse(std::string_view& data) {// lvalue
  Position pos;
  // std::size_t ind = 0;
  std::string_view original = data;
  for (uint8_t i = 0; i < kMaxInd; ++i) {
    if (!internal::ReadPlacement(data, kMaxInd - i - 1, pos)) {
      return std::unexpected(ParseError::kDataIsDamaged);
    }
  }
  // std::size_t ind = data.data() - original.data();
  if (!internal::ParseParameters(data, pos)) {
    return std::unexpected(ParseError::kDataIsDamaged);
  }

  return pos;
}

constexpr std::expected<Position, ParseError> Get(std::string_view original) {// rvalue
  return Parse(original);
}

std::expected<Position, ParseError> Get(std::same_as<fs::path> auto const& file_name) {
  std::error_code ec;
  if (!fs::is_regular_file(file_name, ec)) {
    return std::unexpected(ParseError::kFileNotFound); 
  }
  const auto size = fs::file_size(file_name, ec);
  if (ec || size > internal::kMaxFenSize) {
    return std::unexpected(ParseError::kSizeTooLarge);
  }
  std::ifstream file(file_name, std::ios::in);
  if (!file) {
    return std::unexpected(ParseError::kUnknownError);
  }
  std::string data;
  if (!std::getline(file, data)) {
    return std::unexpected(ParseError::kDataIsDamaged);
  }
  auto res = Get(data);
  if (!res) {
    return std::unexpected(ParseError::kDataIsDamaged);
  }

  return *res;
}

std::expected<void, ParseError> Save(const Position& pos);

std::expected<void, ParseError> Save(const Position& pos, const fs::path& file_name);

}// namespace chess::fen_manager

namespace chess {

inline constexpr std::string_view kDefaultPosition = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

inline constexpr Position kStartingPosition = []() {
  auto res = fen_manager::Get(kDefaultPosition);
  if (!res) {
    throw "Failed to parse FEN in constexpr context";
  }

  return *res;
}();

}