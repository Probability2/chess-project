#pragma once

#include "../core/utils.hpp"

#include "Position.hpp"

#include <cassert>
#include <expected>
#include <filesystem>
#include <format>

namespace fen_manager {

namespace internal {

constexpr char kFenDelimeter = '/';

constexpr char kSpaceDelimiter = ' ';

constexpr std::size_t kMaxFenSize = 100;

enum class ErrorCode {
  kFileNotFound,
  kPermissionDenied,
  kSizeTooLarge,
  kDataIsDamaged,
  kNamesOccupied,
  kUknownError
};

constexpr std::string_view to_string(ErrorCode code) {
  switch (code) {
    case ErrorCode::kFileNotFound: return "File is not found";
    case ErrorCode::kPermissionDenied: return "The permission to the file denied";
    case ErrorCode::kSizeTooLarge: return "The size of given file is too large";
    case ErrorCode::kDataIsDamaged: return "The data is damaged";
    case ErrorCode::kNamesOccupied: return "Unable to create new file because the names are already occupied";
    default: return "Unknown error";
  }
}

constexpr std::expected<void, std::string_view> ReadPlacement(std::string_view data,
                                                              const std::size_t coord,
                                                              Position& pos, std::size_t& i) {
  std::size_t col = 0;
  while (i < data.size() && data[i] != kFenDelimeter && data[i] != kSpaceDelimiter && col <= 8) {
    if (data[i] > '0' && data[i] < '9') {
      pos.set_squares(Pieces::kEmpty, coord, col, data[i] - '0');
      col += (data[i] - '0');
    } else if (std::ranges::contains(kPieceSymbols, data[i])) {
      pos.set_square(GetPieceCode(data[i]), coord, col);
      col++;
    } else {
      return std::unexpected(to_string(ErrorCode::kDataIsDamaged));
    }
    i++;
  }
  if (col != 8) {
    return std::unexpected(to_string(ErrorCode::kDataIsDamaged));
  }
  i++;

  return {};
}

constexpr std::expected<void, std::string_view> ParseTurn(std::size_t& ind, std::string_view data,
                                                          Position& pos) {
  if (ind >= data.size() || (data[ind] != 'w' && data[ind] != 'b')) {
    return std::unexpected(to_string(ErrorCode::kDataIsDamaged));
  }
  pos.set_white_move(data[ind] == 'w');
  ind += 2;

  return {};
}

constexpr std::expected<void, std::string_view> ParseCastle(std::size_t& ind, std::string_view data,
                                                            Position& pos) {
  if (ind >= data.size()) {
    return std::unexpected(to_string(ErrorCode::kDataIsDamaged));
  }
  if (data[ind] == '-') {
    ind += 2;
    return {};
  }
  for (int i = 0; i < 4 && ind < data.size(); ++i) {
    auto it = std::ranges::find(kCastles, data[ind]);
    ind++;
    if (it == kCastles.end()) {
      break;
    }
    pos.set_castling(kMxCastles - std::distance(kCastles.begin(), it) - 1);
  }
  if (ind == data.size()) {
    return std::unexpected(to_string(ErrorCode::kDataIsDamaged));
  }
  ind++;

  return {};
}

constexpr std::expected<void, std::string_view> ParseEnPassant(std::size_t& ind, std::string_view data,
                                                               Position& pos) {
  if (ind >= data.size() || ((data[ind] < 'a' || data[ind] > 'h') && data[ind] != '-')) {
    return std::unexpected(to_string(ErrorCode::kDataIsDamaged));
  }
  if (data[ind] == '-') {
    ind += 2;
    return {};
  }
  uint8_t x = data[ind] - 'a';
  ind++;
  if (ind >= data.size() || data[ind] < '1' || data[ind] > '8') {
    return std::unexpected(to_string(ErrorCode::kDataIsDamaged));
  }
  pos.set_en_passant(utils::coord(static_cast<int>(x), data[ind] - '1'));
  ind += 2;

  return {};
}

constexpr std::expected<void, std::string_view> ParseNoCaptures(std::size_t& ind, std::string_view data,
                                                                Position& pos) {
  if (ind >= data.size()) {
    return std::unexpected(to_string(ErrorCode::kDataIsDamaged));
  }
  int num = utils::GetNumber(data, ind);
  ind++;
  pos.set_no_captures(num);

  return {};
}

constexpr std::expected<void, std::string_view> ParseMoveNumber(std::size_t& ind, std::string_view data,
                                                                Position& pos) {
  if (ind >= data.size()) {
    return std::unexpected(to_string(ErrorCode::kDataIsDamaged));
  }
  int num = utils::GetNumber(data, ind);
  ind++;
  pos.set_move_number(num);

  return {};
}

constexpr std::expected<void, std::string_view> ParseParameters(std::string_view data, std::size_t& ind,
                                                                Position& pos) {
  if (!ParseTurn(ind, data, pos) || !ParseCastle(ind, data, pos) || !ParseEnPassant(ind, data, pos)
      || !ParseNoCaptures(ind, data, pos) || !ParseMoveNumber(ind, data, pos)) {
    return std::unexpected(to_string(ErrorCode::kDataIsDamaged));
  }

  return {};
}

} // end of internal namespace

std::expected<Position, std::string_view> Get(const fs::path& file_name);

constexpr std::expected<Position, std::string_view> Get(std::string_view data) {
  Position pos;
  std::size_t ind = 0;
  for (std::size_t i = 0; i < kMaxInd; ++i) {
    if (!internal::ReadPlacement(data, kMaxInd - i - 1, pos, ind)) {
      return std::unexpected(internal::to_string(internal::ErrorCode::kDataIsDamaged));
    }
  }
  if (!internal::ParseParameters(data, ind, pos)) {
    return std::unexpected(internal::to_string(internal::ErrorCode::kDataIsDamaged));
  }

  return pos;
}

std::expected<void, std::string_view> Save(const Position& pos);

std::expected<void, std::string_view> Save(const Position& pos, std::string_view file_name);

}//end of fen_manager namespace