#pragma once

#include "Position.hpp"

#include <functional>
#include <memory>
#include <type_traits>


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

  Display(Position* board)
  : board_(*board) {
  }

  virtual ~Display() = default;
  
  virtual void Set() = 0;

  Position& board_;
};

class Console: protected Display {
protected:
  Console() = delete;

  template<typename T>
  Console(T&& board)
  : Display(std::forward<T>(board)) {
  }

  virtual ~Console() = default;

  virtual void Set() = 0;

  virtual void Print() = 0;
};

class ConsoleDefault: public Console {
public:
  // template<typename T>
  // ConsoleDefault(T&& board)
  // : Console(std::forward<T>(board)) {
  //   cboard_ = std::vector<std::vector<char>>(kMaxInd, kEmptyRow);
  // }

  void Print() override final;

  std::vector<std::vector<char>> GetBoard() const;

private:
  void Set() override final;

  std::vector<std::vector<char>> cboard_;
};

class BMP: public Display {
public:
  BMP() = delete;

private:
  void Set() override;

  std::vector<int32_t> colours_;
};
