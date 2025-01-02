#pragma once

#include "ChessFormat.hpp"

#include <functional>
#include <memory>
#include <type_traits>

const std::vector<char> kEmptyRow(ChessData::kMaxInd, '.');

const std::vector<char> kWhiteBasicNames = {'P', 'N', 'B', 'R', 'Q', 'K'};

const std::vector<char> kBlackBasicNames = {'p', 'n', 'b', 'r', 'q', 'k'};

constexpr std::size_t kSquareLength = 6;

constexpr std::size_t kSquareHeight = 11;

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

  template<typename T>
  Display(T&& board)
  : board_(std::forward<T>(board)) {
  }

  virtual ~Display() = default;
  
  virtual void Set() = 0;

  Board& board_;
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


class ConsoleImage: public Console {
public:
  template<typename T>
  ConsoleImage(T&& board)
  : Console(std::forward<T>(board)) {
  cboard_ = std::vector<std::vector<std::vector<std::string>>>(ChessData::kMaxInd
                                            , std::vector<std::vector<std::string>>(ChessData::kMaxInd, kEmptySquareImage));
  }

  void Print() override final;
  //~ConsoleImage() = default;

  std::vector<std::vector<std::vector<std::string>>> GetBoard() const;
private:
  void Set() override final;

  std::vector<std::vector<std::vector<std::string>>> cboard_;

  inline void PrintSquare(const std::vector<std::string>& vec, const std::size_t ind) const;

  inline void PrintLine() const;
};

class ConsoleDefault: public Console {
public:
  template<typename T>
  ConsoleDefault(T&& board)
  : Console(std::forward<T>(board)) {
    cboard_ = std::vector<std::vector<char>>(ChessData::kMaxInd, kEmptyRow);
  }

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
