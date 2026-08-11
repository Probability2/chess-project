#include "FileManager.hpp"

namespace chess::fen_manager {

namespace {

constexpr const char* kFilePrefix = "fenfile";

constexpr const char* kDirectoryPrefix = "myfiles";

constexpr int kMaxNameSkips = 200;

static int counter = 1;

std::string CreateName(const int num) {
  return std::format("chess{}.txt", num);
}

std::optional<fs::path> FileName() {
  std::error_code ec;
  fs::path directory = kDirectoryPrefix;
  for (std::size_t i = 0; i < kMaxNameSkips; ++i) {
    auto file_name = directory / CreateName(counter);
    if (!fs::exists(file_name, ec)) {
      return file_name;
    }
    counter++;
  }

  return std::nullopt;
}

std::expected<std::ofstream, std::string_view> CreateFile() {
  fs::path directorypath = kDirectoryPrefix;
  std::error_code ec;
  std::cout << "errore\n";
  fs::create_directory(directorypath, ec);
  if (ec) {
    if (ec == std::errc::permission_denied) {
      return std::unexpected(internal::to_string(internal::ErrorCode::kPermissionDenied));
    }
    return std::unexpected(internal::to_string(internal::ErrorCode::kUknownError));
  }
  auto file_name = FileName();
  if (!file_name.has_value()) {
    return std::unexpected(internal::to_string(internal::ErrorCode::kNamesOccupied));
  }
  std::ofstream file(*file_name, std::ios::out);
  if (!file) {
    return std::unexpected(internal::to_string(internal::ErrorCode::kUknownError));
  }
  counter++;

  return file;
}

void WritePositionSquares(std::ofstream& file, const Position& pos) {
  for (int i = kMaxInd - 1; i >= 0; --i) {
    int empty_count = 0;
    auto process_empty = [&]() {
      if (empty_count > 0) {
        file << empty_count;
        empty_count = 0;
      }
    };
    for (int j = 0; j < kMaxInd; ++j) {
      PieceType square = pos.get_square(i, j);
      if (square == PieceType::kNone) {
        empty_count++;
        continue;
      }
      process_empty();
      file << kPieceSymbols[static_cast<int>(square)];
    }
    process_empty();
    if (i != 0) {
      file << internal::kFenDelimeter;
    }
  }
  file << internal::kSpaceDelimiter;
}

void WritePositionParams(std::ofstream& file, const Position& pos) {
  file << (pos.is_white_move() ? "w " : "b ") << pos.get_castle().value_or("-") << ' '
       << pos.get_en_passant().value_or("-") << " " << pos.get_no_capture_moves() << ' ' << pos.get_move_number();
}

}// unnamed namespace

std::expected<void, std::string_view> Save(const Position& pos) {
  auto file = CreateFile();
  if (!file) {
    return std::unexpected(file.error());
  }
  WritePositionSquares(*file, pos);
  WritePositionParams(*file, pos);

  return {};
}

std::expected<void, std::string_view> Save(const Position& pos, const fs::path& file_name) {
  std::ofstream file(file_name, std::ios::out);
  if (!file) {
    return std::unexpected(internal::to_string(internal::ErrorCode::kUknownError));
  }

  return {};
}

// std::expected<Position, std::string_view> Get(std::same_as<fs::path> auto const& file_name) {
//   std::error_code ec;
//   if (!fs::is_regular_file(file_name, ec)) {
//     return std::unexpected(internal::to_string(internal::ErrorCode::kFileNotFound)); 
//   }
//   const auto size = fs::file_size(file_name, ec);
//   if (ec || size > internal::kMaxFenSize) {
//     return std::unexpected(internal::to_string(internal::ErrorCode::kSizeTooLarge));
//   }
//   std::ifstream file(file_name, std::ios::in);
//   if (!file) {
//     return std::unexpected(internal::to_string(internal::ErrorCode::kUknownError));
//   }
//   std::string data;
//   if (!std::getline(file, data)) {
//     return std::unexpected(internal::to_string(internal::ErrorCode::kDataIsDamaged));
//   }
//   auto res = Get(std::string_view(data));
//   if (!res) {
//     return std::unexpected(internal::to_string(internal::ErrorCode::kDataIsDamaged));
//   }

//   return *res;
// }

}// namespace chess:fen_manager
