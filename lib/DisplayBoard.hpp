#pragma once

#include "ChessFormat.hpp"

#include <memory>
#include <type_traits>

const std::vector<char> kEmptyRow(ChessData::kMaxInd, '.');

const std::vector<char> kWhiteBasicNames = {'P', 'N', 'B', 'R', 'Q', 'K'};

const std::vector<char> kBlackBasicNames = {'p', 'n', 'b', 'r', 'q', 'k'};

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
protected:
  Display() = delete;

  Display(Board&); 

  virtual ~Display() = default;
  
  virtual void Set() = 0;

  Board& board_;
};

class Console: protected Display {
protected:
  Console() = delete;

  Console(Board&);

  virtual ~Console() = default;

  virtual void Set() = 0;

  virtual void Print() = 0;
};


class ConsoleImage: public Console {
public:
  ConsoleImage(Board& board);

  void Print() override final;
  //~ConsoleImage() = default;

  std::vector<std::vector<std::vector<std::string>>> GetBoard() const;
private:
  void Set() override final;

  std::vector<std::vector<std::vector<std::string>>> cboard_;

  void PrintSquare(const std::vector<std::string>& vec, const std::size_t ind) const;

  inline void PrintLine() const;
};

class ConsoleDefault: public Console {
public:
  ConsoleDefault(Board& board);

  void Print() override final;

  std::vector<std::vector<char>> GetBoard() const;

private:
  void Set() override final;

  std::vector<std::vector<char>> cboard_;
};

class BMP: public Display {
public:
  BMP() = delete;

  void Set() override;
private:
  std::vector<int32_t> colours_;
};

