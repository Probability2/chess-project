#pragma once

#include <string>
#include <array>
#include <vector>

class Movement {
protected:
  std::string init_;

  std::vector<std::pair<int, int>> moves_;

  Movement(const std::string& init);

  Movement(const std::string& init, const std::vector<std::pair<int, int>>& moves);

  virtual void Move(const std::string& move) = 0;

  virtual void ShowPossMoves() = 0;

  //void Move(const std::string& move);
};

class PawnMovement: public Movement {
public:
  PawnMovement(const std::string& init);

  void ShowPossMoves() override final;

  void Move(const std::string& move) override;

private:

};

class KnightMovement: public Movement {
public:
  KnightMovement(const std::string& init);

  void ShowPossMoves() override final;

  void Move(const std::string& move) override;

private:
  static std::vector<std::pair<int, int>> kKnightMoves;
};

class BishopMovement: public Movement {
public:
  BishopMovement(const std::string& init);

  void ShowPossMoves() override final;

  void Move(const std::string& move) override;

private:

};

class RookMovement: public Movement {
public:
  RookMovement(const std::string& init);

  void ShowPossMoves() override final;

  void Move(const std::string& move) override;

private:

};

class QueenMovement: public Movement {
public:
  QueenMovement(const std::string& init);

  void ShowPossMoves() override final;

  void Move(const std::string& move) override;

private:

};

class KingMovement: public Movement {
public:
  KingMovement(const std::string& init);

  void ShowPossMoves() override final;

  void Move(const std::string& move) override;

private:
  static std::vector<std::pair<int, int>> kKingMoves;
};