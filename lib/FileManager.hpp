#pragma once

#include "../core/utils.hpp"

#include "Position.hpp"

#include <cassert>
#include <expected>
#include <filesystem>
#include <format>



constexpr const char* kFilePrefix = "chess";

constexpr const char* kDirectoryPrefix = "myfiles";

constexpr char kFenDelimeter = '/';

constexpr char kSpaceDelimiter = ' ';

enum class ErrorCode {
  kFileNotFound,
  kPermissionDenied,
  kMemoryError,
  kDataIsDamaged,
};

constexpr std::string_view to_string(ErrorCode code) {
  switch (code) {
    case ErrorCode::kFileNotFound: return "File is not found";
    case ErrorCode::kPermissionDenied: return "The permission to the file denied";
    case ErrorCode::kMemoryError: return "Memory error of the file";
    case ErrorCode::kDataIsDamaged: return "The data is damaged";
    default: return "Unknown error";
  }
}

class FileManager {
protected:
  FileManager() = delete;

  static std::ofstream CreateFile();

  static std::size_t call_;

  static std::string CreateName();
};



class FenManager: public FileManager {
public:
  FenManager() = delete;

  static std::expected<Position, std::string_view> Get(const fs::path& file_name);

  static constexpr std::expected<Position, std::string_view> Get(std::string_view data) {
    Position pos;
    std::size_t ind = 0;
    for (std::size_t i = 0; i < kMaxInd; ++i) {
      if (!ReadPlacement(data, kMaxInd - i - 1, pos, ind)) {
        return std::unexpected(to_string(ErrorCode::kDataIsDamaged));
      }
    }
    if (ReadParameters(data, ind, pos)) {
      return pos;
    }

    return std::unexpected(to_string(ErrorCode::kDataIsDamaged));
  }

  static void Save(const Position& pos);

  static void Save(const Position& pos, std::string_view file_name);
  
private:
  static constexpr std::expected<void, std::string_view> ReadPlacement(std::string_view data
                                                                       , const std::size_t coord
                                                                       , Position& pos, std::size_t& i) {
    int col = 0;
    while (data[i] != kFenDelimeter && data[i] != kSpaceDelimiter && col <= 8) {
      if (data[i] > '0' && data[i] < '9') {
        pos.SetSquares(Pieces::kEmpty, coord, col, data[i] - '0');
        col += (data[i] - '0');
      } else if (std::ranges::contains(kPieceSymbols, data[i])) {
        pos.SetSquare(GetPieceCode(data[i]), coord, col);
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

  static constexpr std::expected<void, std::string_view> ReadParameters(std::string_view data
                                                                        , std::size_t& ind
                                                                        , Position& pos) {
    Parameters param;
    if (!SetTurn(ind, data, param) || !SetCastle(ind, data, param) || !SetEnPassant(ind, data, param)
        || !SetNoCaptures(ind, data, param) || !SetMoveNumber(ind, data, param)) {
      return std::unexpected(to_string(ErrorCode::kDataIsDamaged));
    }
    pos.SetParameters(param);

    return {};
  }

  constexpr static std::expected<void, std::string_view> SetTurn(std::size_t& ind, std::string_view data
                                                                                 , Parameters& param) {
    if (ind >= data.size() || (data[ind] != 'w' && data[ind] != 'b')) {
      return std::unexpected(to_string(ErrorCode::kDataIsDamaged));
    }
    param.SetWhiteMove(data[ind] == 'w');
    ind += 2;

    return {};
  }

  constexpr static std::expected<void, std::string_view> SetCastle(std::size_t& ind, std::string_view data
                                                                                   , Parameters& param) {
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
      param.SetCastling(kMxCastles - std::distance(kCastles.begin(), it) - 1);
    }
    if (ind == data.size()) {
      return std::unexpected(to_string(ErrorCode::kDataIsDamaged));
    }
    ind++;

    return {};
  }

  constexpr static std::expected<void, std::string_view> SetEnPassant(std::size_t& ind, std::string_view data
                                                                                      , Parameters& param) {
    if (ind >= data.size() || ((data[ind] < 'a' || data[ind] > 'h') && data[ind] != '-')) {
      return std::unexpected(to_string(ErrorCode::kDataIsDamaged));
    }
    if (data[ind] == '-') {
      ind += 2;
      return {};
    }
    uint8_t x = data[ind] - 'a';
    ind++;
    if (ind >= data.size() || data[ind] < '1' && data[ind] > '8') {
      return std::unexpected(to_string(ErrorCode::kDataIsDamaged));
    }
    param.SetEnPassant(utils::coord(static_cast<int>(x), data[ind] - '0'));
    ind += 2;

    return {};
  }

  constexpr static std::expected<void, std::string_view> SetNoCaptures(std::size_t& ind, std::string_view data
                                                                                       , Parameters& param) {
    if (ind >= data.size()) {
      return std::unexpected(to_string(ErrorCode::kDataIsDamaged));
    }
    int num = utils::GetNumber(data, ind);
    ind++;
    param.SetNoCaptures(num);

    return {};
  }

  constexpr static std::expected<void, std::string_view> SetMoveNumber(std::size_t& ind, std::string_view data
                                                                                       , Parameters& param) {
    if (ind >= data.size()) {
      return std::unexpected(to_string(ErrorCode::kDataIsDamaged));
    }
    int num = utils::GetNumber(data, ind);
    ind++;
    param.SetMoveNumber(num);

    return {};
  }

};
