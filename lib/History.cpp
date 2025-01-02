#include "History.hpp"

std::size_t ChessHistory::calls = 0;

void ViewHistory(const std::vector<std::string>& vec) {
  for (std::string str: vec) {
    std::unique_ptr<ChessHistory> history;
    if (str == "champions") {
      history = std::make_unique<Champions>();
    } else if (str == "records") {
      history = std::make_unique<Records>();
    } else if (str == "history_game") {
      history = std::make_unique<HistoryGame>();
    } else if (str == "nowadays") {
      history = std::make_unique<Nowadays>();
    } else {
      history = std::make_unique<Introduction>();
    }
    history->ViewHistory();
  }
}

void Records::ViewHistory() const {
  std::cout << "Records\n";
}

void HistoryGame::ViewHistory() const {
  std::cout << "India\n";
}

void Nowadays::ViewHistory() const {
  std::cout << "Nowadays chess is becoming more and more popular, especially after COVID-19, when people have started "
            << "online.\n"
            << "The most presigious chess tournament in the world except of World Classical Championship are:\n"
            << "Norway chess\nTata-Steel-Classic\nWorld Cup\nGrand Swiss\nSinquifield Cup\n World Rapid, Blitz Championsips\n";
}

void Introduction::ViewHistory() const {
  std::cout << "Introduction\n";
}


void Champions::ViewHistory() const {
  std::cout << "Steinitz:   1886 - 1894\n"
            << "Lasker:     1894 - 1921\n"
            << "Capablanca: 1921 - 1927\n"
            << "Alekhine:   1927 - 1935;\n"
            << "            1937 - 1946\n"
            << "Euwe:       1935 - 1937\n"
            << "Botvinnik:  1948 - 1957;\n"
            << "            1958 - 1960;\n"
            << "            1961 - 1963\n"
            << "Smyslov:    1957 - 1958\n"
            << "Tal:        1960 - 1961\n"
            << "Petrosyan:  1963 - 1969\n"
            << "Spassky:    1969 - 1972\n"
            << "Fisher:     1972 - 1975\n"
            << "Karpov:     1975 - 1985\n"
            << "Kasparov:   1985 - 2000\n"
            << "Kramnik:    2000 - 2007\n"
            << "Anand:      2007 - 2013\n"
            << "Carlsen:    2013 - 2023\n"
            << "Ding:       2023 - 2024\n"
            << "Gukesh:     2024 - ...\n";
}

inline void ChessHistory::PrintTheme() const {
  std::cout << "Chess History:\n";
}