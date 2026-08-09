#pragma once

#include "../core/utils.hpp"
#include "Piece.hpp"

#include <cstring>
#include <fstream>
#include <iostream>
#include <optional>

inline constexpr int kMxCastles = 4;

inline constexpr std::array<char, kMxCastles> kCastles = {'K', 'Q', 'k', 'q'};

class Position {
public:
  constexpr Position() {
    for (std::size_t i = 0; i < utils::kBoardSize; ++i) {
      pos_[i] = Pieces::kEmpty;
    }
  }

  constexpr void set_squares(Pieces p, const std::size_t x, const std::size_t y, const std::size_t n) {
    for (int i = 0; i < n; ++i) {
      set_square(p, x, y + i);
    }
  }

  constexpr void set_square(Pieces p, const std::size_t x, const std::size_t y) {
    pos_[utils::coord(x, y)] = p;
  }

  //constexpr void set_parameters(const Parameters& param) {
  //  param_ = param;
  //}

  //friend std::ostream& operator<<(std::ostream& os, const Position& pos);


  constexpr Pieces get_square(const int x, const int y) const {
    return pos_[utils::coord(x, y)];
  }

  //Parameters get_parameters() const;

  bool is_white_move() const;

  bool is_en_passant() const;
  
  std::size_t get_no_capture_moves() const;

  std::size_t get_move_number() const;

  std::optional<std::string> get_castle() const;

  std::optional<std::string> get_en_passant() const;

  constexpr void set_castling(const int position) {
    castle_ |= (1 << position);
  }

  constexpr void set_white_move(const bool move) {
    if !consteval {
      std::cout << std::boolalpha << move << " is white's move\n";
    }
    isWhiteMove_ = move;
  }

  constexpr void set_en_passant(const int ind) {
    target_ = ind;
    if !consteval {
      std::cout << static_cast<int>(target_) << " target\n";
    }
  }

  constexpr void set_no_captures(const std::size_t moves) {
    no_capture_moves_ = moves;
  }

  constexpr void set_move_number(const std::size_t moves) {
    move_ = moves;
  }

  
  
private:
  std::array<Pieces, utils::kBoardSize> pos_;
  bool isWhiteMove_ = true;
  uint8_t castle_ = 0;
  uint8_t target_ = 0;
  std::size_t no_capture_moves_ = 0;
  std::size_t move_ = 0;
};


std::ostream& operator<<(std::ostream& os, const Position& pos);
