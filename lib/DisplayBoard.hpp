#pragma once

#include "ChessFormat.hpp"

#include <memory>

const std::vector<char> kEmptyRow(ChessData::kMaxInd, 'o');

const std::vector<char> kBasicNames = {'P', 'N', 'B', 'R', 'Q', 'K'};

const std::size_t kSquareLength = 6;

const std::size_t kSquareHeight = 11;

const std::vector<std::string> kEmptySquareImage = {"           ",
                                                    "           ",
                                                    "           ",
                                                    "           ",
                                                    "           ",
                                                    "           "};

const std::vector<std::string> kBishopImage = {"     |     ",
                                               "    / \\    ",
                                               "   |   |   ",
                                               "   \\   /   ",
                                               "  __|___|_ "};

const std::vector<std::string> kPawnImage =   {"   _|=|_   ",
                                               "  |__ __|  ",
                                               "    | |    ",
                                               "   /   \\   ",
                                               "  |_____|  "};
                                               
const std::vector<std::string> kRookImage =   {"|-|_|-|_|-|",
                                               "|__     __|",
                                               "  |     |  ",
                                               " _|     |_ ",
                                               "|_________|"};

const std::vector<std::string> kKingImage =   {"      +_   ",
                                               "   __/ \\__ ",
                                               "  \\|\\_/_|_/",
                                               "   \\|||||/ ",
                                               "    |||||  ",
                                               "   /_____\\ "};

const std::vector<std::string> kQuennImage =  {"   \\\\ | // ",
                                               "    \\_|_/  ",
                                               "    |||||  ",
                                               "    -----  ",
                                               "   /|||||\\ "};

const std::vector<std::string> kKnightImage = {"   /|___   ",
                                               "  /   _ \\  ",
                                               "  \\_/\\ / \\ ",
                                               "     /   |_",
                                               "    /_____|"};

const std::vector<std::vector<std::string>> kPieceConsoleImages = {kPawnImage, kKnightImage, kBishopImage,
                                                             kRookImage, kQuennImage, kKingImage};

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

  std::vector<std::vector<std::vector<std::string>>> GetBoard() const;

  void Print();

private:
  std::vector<std::vector<std::vector<std::string>>> cboard_;

  void PrintSquare(const std::vector<std::string>& vec, const std::size_t ind) const;

  void SetPossPiece(const std::vector<std::string>& poss, const std::vector<std::string>& name);

  void SetPossPiece(const std::string& pos, const std::vector<std::string>& name);

  inline void PrintLine() const;

};

class BMP: public Display {
public:
  BMP() = delete;

  void Set() override;
private:
  std::vector<int32_t> colours_;
};

