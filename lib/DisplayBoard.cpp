#include "DisplayBoard.hpp"

template<typename T, typename std::enable_if<std::is_same<T, char>::value || std::is_same<T, std::vector<std::string>>::value>::type* = nullptr>
void SetPossPiece(const std::vector<std::string>& poss, const T& name, std::vector<std::vector<T>>& cboard_) {
  if (poss.empty()) {
    return;
  }
  for (auto pos: poss) {
    cboard_[ChessData::kMaxInd - (pos[1] - '0')][pos[0] - 'a'] = name;
  }
}

template<typename T, typename std::enable_if<std::is_same<T, char>::value || std::is_same<T, std::vector<std::string>>::value>::type* = nullptr>
void SetPossPiece(const std::string& pos, const T& name, std::vector<std::vector<T>>& cboard_) {
  if (pos.empty()) {
    return;
  }
  cboard_[ChessData::kMaxInd - (pos[1] - '0')][pos[0] - 'a'] = name;
}

std::vector<std::vector<std::vector<std::string>>> ConsoleImage::GetBoard() const {
  return cboard_;
}

std::vector<std::vector<char>> ConsoleDefault::GetBoard() const {
  return cboard_;
}

inline void ConsoleImage::PrintLine() const {
  std::cout << std::string((kSquareHeight + 1) * ChessData::kMaxInd, '-') << '\n';
}

inline void ConsoleImage::PrintSquare(const std::vector<std::string>& vec, const std::size_t ind) const {
  std::cout << (ind < vec.size() ? vec[ind] : " ");
}

void ConsoleImage::Print() {
  std::cout << "Print\n";
  this->Set();
  for (auto row: cboard_) {
    for (std::size_t i = 0; i < kSquareLength; ++i) {
      for (auto sq: row) {
        PrintSquare(sq, i);
      }
      std::cout << '\n';
    }
    PrintLine();
  }
}

void ConsoleDefault::Print() {
  this->Set();
  for (auto row: cboard_) {
    for (std::size_t i = 0; i < ChessData::kMaxInd; ++i) {
      std::cout << row[i];
    }
    std::cout << '\n';
  }
}

void ConsoleImage::Set() {
  auto bound_function = [this](auto a, auto b) {
    SetPossPiece<std::vector<std::string>>(a, b, cboard_);
  };
  bound_function(board_.GetWhitePawns().GetPositions(), kPieceConsoleImages[0]);
  bound_function(board_.GetBlackPawns().GetPositions(), kPieceConsoleImages[0]);
  bound_function(board_.GetWhiteKnights().GetPositions(), kPieceConsoleImages[1]);
  bound_function(board_.GetBlackKnights().GetPositions(), kPieceConsoleImages[1]);
  bound_function(board_.GetWhiteBishops().GetPositions(), kPieceConsoleImages[2]);
  bound_function(board_.GetBlackBishops().GetPositions(), kPieceConsoleImages[2]);
  bound_function(board_.GetWhiteRooks().GetPositions(), kPieceConsoleImages[3]);
  bound_function(board_.GetBlackRooks().GetPositions(), kPieceConsoleImages[3]);
  bound_function(board_.GetWhiteQuenns().GetPositions(), kPieceConsoleImages[4]);
  bound_function(board_.GetBlackQuenns().GetPositions(), kPieceConsoleImages[4]);
  bound_function(board_.GetWhiteKing().GetPosition(), kPieceConsoleImages[5]);
  bound_function(board_.GetBlackKing().GetPosition(), kPieceConsoleImages[5]);
}

void ConsoleDefault::Set() {
  auto bound_function = [this](auto a, auto b) {
    SetPossPiece<char>(a, b, cboard_);
  };
  bound_function(board_.GetWhitePawns().GetPositions(), kWhiteBasicNames[0]);
  bound_function(board_.GetBlackPawns().GetPositions(), std::tolower(kWhiteBasicNames[0]));
  bound_function(board_.GetWhiteKnights().GetPositions(), kWhiteBasicNames[1]);
  bound_function(board_.GetBlackKnights().GetPositions(), std::tolower(kWhiteBasicNames[1]));
  bound_function(board_.GetWhiteBishops().GetPositions(), kWhiteBasicNames[2]);
  bound_function(board_.GetBlackBishops().GetPositions(), std::tolower(kWhiteBasicNames[2]));
  bound_function(board_.GetWhiteRooks().GetPositions(), kWhiteBasicNames[3]);
  bound_function(board_.GetBlackRooks().GetPositions(), std::tolower(kWhiteBasicNames[3]));
  bound_function(board_.GetWhiteQuenns().GetPositions(), kWhiteBasicNames[4]);
  bound_function(board_.GetBlackQuenns().GetPositions(), std::tolower(kWhiteBasicNames[4]));
  bound_function(board_.GetWhiteKing().GetPosition(), kWhiteBasicNames[5]);
  bound_function(board_.GetBlackKing().GetPosition(), std::tolower(kWhiteBasicNames[5]));
}

void BMP::Set() {

}
