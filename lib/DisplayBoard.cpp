#include "DisplayBoard.hpp"

Display::Display(Board& board)
: board_(board) {
}

Console::Console(Board& board)
: Display(board) {
  cboard_ = std::vector<std::vector<char>>(ChessData::kMaxInd, kEmptyRow);
}

void Console::SetPossPiece(const std::vector<std::string>& poss, const char name) {
  if (poss.empty()) {
    return;
  }
  for (auto pos: poss) {
    cboard_[ChessData::kMaxInd - (pos[1] - '0')][pos[0] - 'a'] = name;
  }
}

void Console::SetPossPiece(const std::string& pos, const char name) {
  if (pos.empty()) {
    return;
  }
  cboard_[ChessData::kMaxInd - (pos[1] - '0')][pos[0] - 'a'] = name;
}

std::vector<std::vector<char>> Console::GetBoard() const {
  return cboard_;
}

void Console::Print() {
  std::cout << "Print\n";
  this->Set();
  std::cout << "Finish Set\n";
  for (auto& row: cboard_) {
    for (char p: row) {
      std::cout << p;
    }
    std::cout << '\n';
  }
}

void Console::Set() {
  SetPossPiece(board_.GetWhitePawns().GetPositions(), kBasicNames[0]);
  SetPossPiece(board_.GetBlackPawns().GetPositions(), kBasicNames[0]);
  SetPossPiece(board_.GetWhiteKnights().GetPositions(), kBasicNames[1]);
  SetPossPiece(board_.GetBlackKnights().GetPositions(), kBasicNames[1]);
  SetPossPiece(board_.GetWhiteBishops().GetPositions(), kBasicNames[2]);
  SetPossPiece(board_.GetBlackBishops().GetPositions(), kBasicNames[2]);
  SetPossPiece(board_.GetWhiteRooks().GetPositions(), kBasicNames[3]);
  SetPossPiece(board_.GetBlackRooks().GetPositions(), kBasicNames[3]);
  SetPossPiece(board_.GetWhiteQuenns().GetPositions(), kBasicNames[4]);
  SetPossPiece(board_.GetBlackQuenns().GetPositions(), kBasicNames[4]);
  SetPossPiece(board_.GetWhiteKing().GetPosition(), kBasicNames[5]);
  SetPossPiece(board_.GetBlackKing().GetPosition(), kBasicNames[5]);
}

void BMP::Set() {

}