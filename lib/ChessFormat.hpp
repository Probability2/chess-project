#pragma once

#include "ChessPiece.hpp"

#include <fstream>
#include <cstring>

constexpr std::size_t kCountPiece = 10;

class Board {
public:
  Board() = default;

  Board(Board&&);

  void SetPosition();

  void SaveImage() const;

  void SaveFEN() const;

  void GetFromImage(const std::string& file_name);

  void GetFromFEN(const std::string& file_name);
//, typename std::enable_if<std::is_same<T, White>::value || std::is_same<T, Black>::value, int>::type = 0

  void SetPosition(ChessPiece& piece, const std::string& str) {
    piece.AddPositions(str);
  }

  void Clear();

  ~Board() {
    std::cout << "Destruct\n";
  }

  Board(const Board& board);

  Board& operator=(const Board& board);

  void SetDefault();

  const Pawn& GetWhitePawns() const;

  const Pawn& GetBlackPawns() const;

  Pawn GetAllPawns() const;

  const Knight& GetWhiteKnights() const;

  const Knight& GetBlackKnights() const;

  Knight GetAllKnights() const;

  const Bishop& GetWhiteBishops() const;

  const Bishop& GetBlackBishops() const;

  Bishop GetAllBishops() const;

  const Rook& GetWhiteRooks() const;

  const Rook& GetBlackRooks() const;

  Rook GetAllRooks() const;

  const Quenn& GetWhiteQuenns() const;

  const Quenn& GetBlackQuenns() const;

  Quenn GetAllQuenns() const;

  const King& GetWhiteKing() const;

  const King& GetBlackKing() const;

private:
  Pawn w_pawns_;
  Pawn b_pawns_;
  Knight w_knights_;
  Knight b_knights_;
  Bishop w_bishops_;
  Bishop b_bishops_;
  Rook w_rooks_;
  Rook b_rooks_;
  Quenn w_quenns_;
  Quenn b_quenns_;
  King w_king_;
  King b_king_;

  void ReadImage(std::ifstream& file);

  void ReadPosition(const char piece, const std::string& pos);

  void SetColoursPosition(const std::size_t in_state);

  void InputCoords(const std::size_t i);

  std::string GetPosKing();

  std::array<ChessPiece*, kCountPiece> pieces_ = {&w_pawns_, &w_knights_, &w_bishops_, &w_rooks_, &w_quenns_
                                                 ,&b_pawns_, &b_knights_, &b_bishops_, &b_rooks_, &b_quenns_};

  std::array<std::string, kCountPiece> pnames_ = {"White Pawns", "White Knights", "White Bishops", "White Rooks"
                                                 ,"White Queens", "Black Pawns", "Black Knights", "Black Bishops"
                                                 , "Black Rooks", "Black Queens"};
                                                 
};

class ChessFormat {
public:
  virtual void ViewBoard() const = 0;

private:

};

class Standard: public ChessFormat {
public:
  Standard();
  void ViewBoard() const override;
};

class FisherRandom: public ChessFormat {
public:
  FisherRandom();
  void ViewBoard() const override;
  void GetRandomPosition();
  void PrintPlacement() const;

private:
  
};

class Crazyhouse: public ChessFormat {

};

class BugHouse: public ChessFormat {

};

class Atomic: public ChessFormat {

};

class KingOfTheHill: public ChessFormat {

};

class ThreeCheck: public ChessFormat {

};

class Horde: public ChessFormat {

};
