#pragma once

#include <algorithm>
#include <array>
#include <iostream>
#include <optional>
#include <string>
#include <utility>
#include <vector>

constexpr std::size_t kCountPiece = 13;

enum class Pieces {
  kEmpty,
  kWhitePawn, kWhiteKnight, kWhiteBishop, kWhiteRook, kWhiteQueen, kWhiteKing,
  kBlackPawn, kBlackKnight, kBlackBishop, kBlackRook, kBlackQueen, kBlackKing
};

constexpr char kEmptySquare = '.';

const std::array<std::string, kCountPiece> kPieceImages = {".", "♙", "♘", "♗", "♖", "♕", "♔",
                                                         "♟", "♞", "♝", "♜", "♛", "♚"};

const std::array<char, kCountPiece> kPieceSymbols = {'.', 'P', 'N', 'B', 'R', 'Q', 'K',
                                                         'p', 'n', 'b', 'r', 'q', 'k'};

struct PieceData {
  Pieces piece;
  char code;
  const char* icon;
};


// constexpr const char* kReset = "\033[0m";
// constexpr const char* kWhiteText = "\033[97m";
// constexpr const char* kBlackText = "\033[30m";

inline constexpr std::array<PieceData, kCountPiece> kPieceTable {{
    { Pieces::kEmpty,       '.', "." },
    { Pieces::kWhitePawn,   'P', "\033[97m♟\033[0m"},
    { Pieces::kWhiteKnight, 'N', "\033[97m♞\033[0m" },
    { Pieces::kWhiteBishop, 'B', "\033[97m♝\033[0m" },
    { Pieces::kWhiteRook,   'R', "\033[97m♜\033[0m" },
    { Pieces::kWhiteQueen,  'Q', "\033[97m♛\033[0m" },
    { Pieces::kWhiteKing,   'K', "\033[97m♚\033[0m" },
    { Pieces::kBlackPawn,   'p', "\033[30m♟\033[0m" },
    { Pieces::kBlackKnight, 'n', "\033[30m♞\033[0m" },
    { Pieces::kBlackBishop, 'b', "\033[30m♝\033[0m" },
    { Pieces::kBlackRook,   'r', "\033[30m♜\033[0m" },
    { Pieces::kBlackQueen,  'q', "\033[30m♛\033[0m" },
    { Pieces::kBlackKing,   'k', "\033[30m♚\033[0m" }
}};

constexpr Pieces GetPieceCode(char p) {
  for (auto& data: kPieceTable) {
    if (data.code == p) {
      return data.piece;
    }
  }

  return Pieces::kEmpty;
}

constexpr std::string GetPieceIcon(Pieces piece) {
  return kPieceTable[static_cast<int>(piece)].icon;
}



inline void print(char coord, int ind) {
  std::cout << coord << ' ' << ind << '\n';
}

static constexpr int kMinInd = 0;
static constexpr int kMaxInd = 8;
static constexpr char kMinCoord = 'a';
static constexpr char kMaxCoord = 'h';



struct ChessPiece {
  ChessPiece() = default;

  template<typename ...Arguments>
  void AddPositions(Arguments... args) {
    poss_.reserve(sizeof...(args));
    (poss_.emplace_back(args), ...);
  }

  void HelloWorld() const {
    std::cout << "Hello, World!\n";
  }

  void PrintPositions() const;

  virtual void Move(std::size_t ind, const std::string& new_pos) = 0;

  virtual void PrintPossMoves() const = 0;

  void SetPosition();

  std::size_t size() const;

  friend std::ostream& operator<<(std::ostream& output, const ChessPiece& piece) {
    std::cout << piece.poss_.size() << '\n';
    for (auto& pos: piece.poss_) {
      output << pos;
      output << '\n';
    }

    return output;
  }

  std::vector<std::string> GetPositions();

  std::vector<std::string> poss_;
};

//std::ostream& operator<<(std::ostream& output, ChessPiece& piece);

class Pawn: public ChessPiece {
public:
  Pawn() = default;

  Pawn& operator=(const Pawn& pawn);

  Pawn operator+(const Pawn& pawn) const;

  void Move(std::size_t ind, const std::string& new_pos) override {

  };

  void PrintPossMoves() const override {

  };

  std::string GetPosition() const;

  class PawnMovement;

private:

};

class Knight: public ChessPiece {
public:
  Knight() = default;

  template<typename... Arguments>
  Knight(Arguments... args) {
    AddPositions(args...);
  }

  Knight& operator=(const Knight& knight);

  Knight operator+(const Knight& knight) const;

  void Move(std::size_t ind, const std::string& new_pos) override;

  void PrintPossMoves() const override;

  class KnightMovement;

  //std::string GetPosition() const override;
private:
  //std::vector<std::string> vv;
  //std::optional<std::string> pos1_;
  //std::optional<std::string> pos2_;
};

class Bishop: public ChessPiece {
public:
  Bishop() = default;

  template<typename... Arguments>
  Bishop(Arguments... args) {
    (poss_.push_back(args), ...);
  }

  Bishop& operator=(const Bishop& bishop);

  Bishop operator+(const Bishop& bishop) const;

  void Move(std::size_t ind, const std::string& new_pos) override {

  };

  void PrintPossMoves() const override {

  };

  class BishopMovement;

private:

};

class Rook: public ChessPiece {
public:
  Rook() = default;

  template<typename... Arguments>
  Rook(Arguments... args) {
    // Троеточие слева раскрывает пакет параметров для каждого аргумента по очереди
    (poss_.push_back(args), ...);
  }

  Rook& operator=(const Rook& rook);

  Rook operator+(const Rook& rook) const;

  void Move(std::size_t ind, const std::string& new_pos) override {

  };

  void PrintPossMoves() const override {

  };

  class RookMovement;
  //std::string GetPosition() const override;
private:

};

class Quenn: public ChessPiece {
public:
  Quenn() = default;

  template<typename... Arguments>
  Quenn(Arguments... args) {
    // Троеточие слева раскрывает пакет параметров для каждого аргумента по очереди
    (poss_.push_back(args), ...);
  }

  Quenn& operator=(const Quenn& quenn);

  Quenn operator+(const Quenn& quenn) const;

  void Move(std::size_t ind, const std::string& new_pos) override {

  };

  void PrintPossMoves() const override {

  };

  class QueenMovement;

private:

};

// template<typename... Arguments>
// Bishop::Bishop(Arguments... args) {
//   for (auto i: args) {
//     poss_.push_back(i);
//   }
// }

class King {
public:
  King() = default;

  King(const std::string& str);

  void SetPosition(std::string pos);

  void Move(const std::string& new_pos);

  bool IsChecked() const;

  bool IsMated() const;

  std::string GetPosition() const;

  class KingMovement;

private:
  std::string pos_;

};