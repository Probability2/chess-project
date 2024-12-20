#pragma once

#include "ChessFormat.hpp"

#include <memory>

const std::vector<char> kEmptyRow(ChessData::kMaxInd, 'o');

const std::vector<char> kBasicNames = {'P', 'N', 'B', 'R', 'Q', 'K'};

const std::vector<std::string> kBishop = {"  |  ",
                                          " / \ ",
                                          "|   |",
                                          "\   /",
                                        "__|___|_"};

const std::vector<std::string> kPawn =   {"  |‾|  ",
                                          "|‾   ‾| ",
                                          "‾‾| |‾‾",
                                          " /   \ ",
                                          "|_____| "};

const std::vector<std::string> kRook =   {"|‾|_|‾|_|‾|",
                                          "|         |",
                                          "‾‾|		 |‾‾",
                                          " _|     |_",
                                          "|_________|"};

const std::vector<std::string> kKing =   {"    †   ",
                                          " __/‾\__ ",
                                          "\|\_/_|_/ "
                                          " \|||||/",
                                          "  |||||  ",
                                          " /_____\ "};

class Display {
public:
  //Display() = delete;

  Display(Board&); 
  
  virtual void Set() = 0;

  Board& board_;

};

class Console: public Display {
public:
  Console() = delete;

  Console(Board&);

  void Set() override;

  std::vector<std::vector<char>> GetBoard() const;

  void Print();

private:
  std::vector<std::vector<char>> cboard_;

  void SetPossPiece(const std::vector<std::string>& poss, const char name);

  void SetPossPiece(const std::string& pos, const char name);

};

class BMP: public Display {
public:
  BMP() = delete;

  void Set() override;
private:
  std::vector<int32_t> colours_;
};
