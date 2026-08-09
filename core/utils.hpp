#pragma once

#include <charconv>
#include <filesystem>
#include <string>
#include <vector>

namespace fs = std::filesystem;

using iterator = std::vector<uint8_t>::iterator;
using const_iterator = std::vector<uint8_t>::const_iterator;
using size_type = std::size_t;
using difference_type = std::ptrdiff_t;

namespace utils {

inline constexpr int kBoardSize = 64;

inline constexpr int coord(int x, int y) {
  return 8 * x + y;
}

inline constexpr int GetNumber(std::string_view data, std::size_t& ind) {
  int num = 0;
  while (ind < data.length() && data[ind] >= '0' && data[ind] <= '9') {
    num = num * 10 + (data[ind] - '0');
    ind++;
  }

  return num;
}
}