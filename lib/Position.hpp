#pragma once

#include "../core/utils.hpp"
#include "Piece.hpp"

#include <cstring>
#include <fstream>
#include <iostream>
#include <memory>

inline constexpr int kMxCastles = 4;

inline constexpr std::array<char, kMxCastles> kCastles = {'K', 'Q', 'k', 'q'};

class Parameters {
public:
  constexpr Parameters() = default;

  bool IsWhiteMove() const;

  bool IsEnPassant() const;
  
  std::size_t GetNoCaptureMoves() const;

  std::size_t GetMove() const;

  uint8_t GetCastle() const;

  std::optional<std::pair<int, int>> GetEnPassant() const;

  constexpr void SetCastling(const int position) {
    castle_ ^= (1 << position);
  }

  constexpr void SetWhiteMove(const bool move) {
    if !consteval {
      std::cout << std::boolalpha << move << " is white's move\n";
    }
    isWhiteMove_ = move;
  }

  constexpr void SetEnPassant(const int ind) {
    target_ = ind;
    if !consteval {
      std::cout << static_cast<int>(target_) << " target\n";
    }
  }

  constexpr void SetNoCaptures(const std::size_t moves) {
    no_capture_moves_ = moves;
  }

  constexpr void SetMoveNumber(const std::size_t moves) {
    move_ = moves;
  }

private:
  bool isWhiteMove_ = true;
  uint8_t castle_ = 0;
  uint8_t target_ = 0;
  std::size_t no_capture_moves_ = 0;
  std::size_t move_ = 0;
};



class Position {
public:
  constexpr Position() {
    for (std::size_t i = 0; i < utils::kBoardSize; ++i) {
      pos_[i] = Pieces::kEmpty;
    }
  }

  constexpr void SetSquares(Pieces p, const int x, const int y, const int n) {
    for (int i = 0; i < n; ++i) {
      SetSquare(p, x, y + i);
    }
  }

  constexpr void SetSquare(Pieces p, const int x, const int y) {
    pos_[utils::coord(x, y)] = p;
  }

  constexpr void SetParameters(Parameters& param) {
    param_ = param;
  }

  //friend std::ostream& operator<<(std::ostream& os, const Position& pos);


  constexpr Pieces GetSquare(const int x, const int y) const {
    return pos_[utils::coord(x, y)];
  }

  Parameters GetParameters() const;
  
private:
  std::array<Pieces, utils::kBoardSize> pos_;

  Parameters param_;
};


std::ostream& operator<<(std::ostream& os, const Position& pos);

std::ostream& operator<<(std::ostream& os, const Parameters& param);
