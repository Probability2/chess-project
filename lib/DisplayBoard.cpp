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
  for (std::size_t i = 0; i < (kSquareHeight + 1) * ChessData::kMaxInd; ++i) {
    std::cout << '-';
  }
  std::cout << '\n';
}

void ConsoleImage::PrintSquare(const std::vector<std::string>& vec, const std::size_t ind) const {
  if (ind >= vec.size()) {
    std::cout << ' ';
    return;
  }
  std::cout << vec[ind];
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
  std::cout << "Print\n";
  this->Set();
  for (auto row: cboard_) {
    for (std::size_t i = 0; i < ChessData::kMaxInd; ++i) {
      std::cout << row[i];
    }
    std::cout << '\n';
  }
}

void ConsoleImage::Set() {
  SetPossPiece<std::vector<std::string>>(board_.GetWhitePawns().GetPositions(), kPieceConsoleImages[0], cboard_);
  SetPossPiece<std::vector<std::string>>(board_.GetBlackPawns().GetPositions(), kPieceConsoleImages[0], cboard_);
  SetPossPiece<std::vector<std::string>>(board_.GetWhiteKnights().GetPositions(), kPieceConsoleImages[1], cboard_);
  SetPossPiece<std::vector<std::string>>(board_.GetBlackKnights().GetPositions(), kPieceConsoleImages[1], cboard_);
  SetPossPiece<std::vector<std::string>>(board_.GetWhiteBishops().GetPositions(), kPieceConsoleImages[2], cboard_);
  SetPossPiece<std::vector<std::string>>(board_.GetBlackBishops().GetPositions(), kPieceConsoleImages[2], cboard_);
  SetPossPiece<std::vector<std::string>>(board_.GetWhiteRooks().GetPositions(), kPieceConsoleImages[3], cboard_);
  SetPossPiece<std::vector<std::string>>(board_.GetBlackRooks().GetPositions(), kPieceConsoleImages[3], cboard_);
  SetPossPiece<std::vector<std::string>>(board_.GetWhiteQuenns().GetPositions(), kPieceConsoleImages[4], cboard_);
  SetPossPiece<std::vector<std::string>>(board_.GetBlackQuenns().GetPositions(), kPieceConsoleImages[4], cboard_);
  SetPossPiece<std::vector<std::string>>(board_.GetWhiteKing().GetPosition(), kPieceConsoleImages[5], cboard_);
  SetPossPiece<std::vector<std::string>>(board_.GetBlackKing().GetPosition(), kPieceConsoleImages[5], cboard_);
}

void ConsoleDefault::Set() {
  SetPossPiece<char>(board_.GetWhitePawns().GetPositions(), kWhiteBasicNames[0], cboard_);
  SetPossPiece<char>(board_.GetBlackPawns().GetPositions(), kBlackBasicNames[0], cboard_);
  SetPossPiece<char>(board_.GetWhiteKnights().GetPositions(), kWhiteBasicNames[1], cboard_);
  SetPossPiece<char>(board_.GetBlackKnights().GetPositions(), kBlackBasicNames[1], cboard_);
  SetPossPiece<char>(board_.GetWhiteBishops().GetPositions(), kWhiteBasicNames[2], cboard_);
  SetPossPiece<char>(board_.GetBlackBishops().GetPositions(), kBlackBasicNames[2], cboard_);
  SetPossPiece<char>(board_.GetWhiteRooks().GetPositions(), kWhiteBasicNames[3], cboard_);
  SetPossPiece<char>(board_.GetBlackRooks().GetPositions(), kBlackBasicNames[3], cboard_);
  SetPossPiece<char>(board_.GetWhiteQuenns().GetPositions(), kWhiteBasicNames[4], cboard_);
  SetPossPiece<char>(board_.GetBlackQuenns().GetPositions(), kBlackBasicNames[4], cboard_);
  SetPossPiece<char>(board_.GetWhiteKing().GetPosition(), kWhiteBasicNames[5], cboard_);
  SetPossPiece<char>(board_.GetBlackKing().GetPosition(), kBlackBasicNames[5], cboard_);
}

void BMP::Set() {

}
