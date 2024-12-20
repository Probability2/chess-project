#pragma once

#include <algorithm>
#include <array>
#include <iostream>
#include <optional>
#include <string>
#include <utility>
#include <vector>

#define ch char

//Tags
struct White{};

struct Black{};

inline void print(ch coord, int ind) {
  std::cout << coord << ' ' << ind;
}

struct ChessData {
  static const int kMinInd = 0;
  static const int kMaxInd = 8;
  static const ch kMinCoord = 'a';
  static const ch kMaxCoord = 'h';
  //static const std::vector<ch> kCoords{'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'};
};

const struct KnightData {

const static int kKnightDirections = 8;

static constexpr std::array<std::pair<int, int>, kKnightDirections> kKnightMoves = {std::make_pair(-1, 2), std::make_pair(1, 2),
                                                                  std::make_pair(-1, -2), std::make_pair(1, -2),
                                                                  std::make_pair(2, 1), std::make_pair(2, -1),
                                                                  std::make_pair(-2, 1), std::make_pair(-2, -1)};

};


struct ChessPiece {
  ChessPiece() = default;

  template<typename ...Arguments>
  void AddPositions(Arguments... args) {
    poss_.reserve(sizeof...(args));
    (poss_.emplace_back(args), ...);
  }

  void PrintPositions() const;

  virtual void Move(std::size_t ind, const std::string& new_pos) = 0;

  void PrintAllMoves() const;

  virtual void PrintPossMoves() const = 0;

  void SetPosition();

  std::size_t size() const;

  friend std::ostream& operator<<(std::ostream& output, const ChessPiece& piece) {
    std::cout << "Output\n";
    std::cout << piece.poss_.size() << '\n';
    for (auto& pos: piece.poss_) {
      output << pos;
      output << '\n';
    }

    return output;
  }

  std::vector<std::string> GetPositions() const;

  void CheckCorrectness(const std::optional<std::string>& pos) const;

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
    for (auto i: args) {
      poss_.push_back(i);
    }
  }

  Bishop& operator=(const Bishop& bishop);

  Bishop operator+(const Bishop& bishop) const;

  void Move(std::size_t ind, const std::string& new_pos) override {

  };

  void PrintPossMoves() const override {

  };

private:

};

class Rook: public ChessPiece {
public:
  Rook() = default;

  template<typename... Arguments>
  Rook(Arguments... args) {
    for (auto i: args) {
      poss_.push_back(i);
    }
  }

  Rook& operator=(const Rook& rook);

  Rook operator+(const Rook& rook) const;

  void Move(std::size_t ind, const std::string& new_pos) override {

  };

  void PrintPossMoves() const override {

  };
  //std::string GetPosition() const override;
private:

};

class Quenn: public ChessPiece {
public:
  Quenn() = default;

  template<typename... Arguments>
  Quenn(Arguments... args) {
    for (auto i: args) {
      poss_.push_back(i);
    }
  }

  Quenn& operator=(const Quenn& quenn);

  Quenn operator+(const Quenn& quenn) const;

  void Move(std::size_t ind, const std::string& new_pos) override {

  };

  void PrintPossMoves() const override {

  };

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

  void SetPosition(const std::string& pos);

  void Move(const std::string& new_pos);

  bool IsChecked() const;

  bool IsMated() const;

  std::string GetPosition() const;

private:
  std::string pos_;

};