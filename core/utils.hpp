#pragma once

#include <concepts>
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
  
inline constexpr int coord(int rank, int file) {
  return 8 * rank + file;
}

inline void PrintBitboard(Bitboard b) {
  for (int rank = 7; rank >= 0; --rank) {
    std::cout << rank << " | ";
    for (int file = 0; file < 8; ++file) {
      std::cout << ((b >> coord(rank, file)) & 1) << ' ';
    }
    std::cout << "| " << rank << '\n';
  }
}

inline std::string get_notation(const uint8_t move) {
  return std::string{static_cast<char>((move & 0x07) + 'a'),
                     static_cast<char>(((move >> 3) & 0x07) + '1')};
}

void BitLooping(Bitboard bb, std::invocable<uint8_t> auto&& f) {
  while (bb > 0) {
    uint8_t ind = std::countr_zero(bb);
    f(ind);
    bb &= (bb - 1); //Brian Kernighan's algorithm
  }
}

}// namespace utils