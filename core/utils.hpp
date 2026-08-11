#pragma once

#include <charconv>
#include <filesystem>
#include <iostream>
#include <string_view>
#include <vector>

namespace fs = std::filesystem;

using Bitboard = uint64_t;
using iterator = std::vector<uint8_t>::iterator;
using const_iterator = std::vector<uint8_t>::const_iterator;
using size_type = std::size_t;
using difference_type = std::ptrdiff_t;

namespace utils {
  
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

inline constexpr void PrintBitboard(Bitboard b) {
  for (int i = 0; i < 64; ++i) {
    std::cout << b % 2 << ' ';
    b /= 2;
    if (i % 8 == 7) {
      std::cout << '\n';
    }
  }
}
}