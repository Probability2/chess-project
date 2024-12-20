#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <vector>

void ViewHistory(const std::vector<std::string>& vec);

class ChessHistory {
public:
  ChessHistory() = default;

  static std::size_t calls;

  virtual void ViewHistory() const = 0;

  void PrintTheme() const;
};

class Champions: public ChessHistory {
public:
  Champions() = default;
  void ViewHistory() const override;
};

class Records: public ChessHistory {
public:
  Records() = default;
  void ViewHistory() const override;

private:
  int longest_hakhtara = 27;
};

class HistoryGame: public ChessHistory {
public:
  HistoryGame() = default;
  void ViewHistory() const override;

};

class Nowadays: public ChessHistory {
public:
  Nowadays() = default;
  void ViewHistory() const override;

};

class Introduction: public ChessHistory {
public:
  Introduction() = default;
  void ViewHistory() const override;

};