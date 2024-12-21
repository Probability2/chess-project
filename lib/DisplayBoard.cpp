#include "DisplayBoard.hpp"

//Пока выглядит грустно, но что поделать

Display::Display(Board& board)
: board_(board) {
}

Console::Console(Board& board)
: Display(board) {
  cboard_ = std::vector<std::vector<std::vector<std::string>>>(ChessData::kMaxInd
                                            , std::vector<std::vector<std::string>>(ChessData::kMaxInd, kEmptySquareImage));
}

void Console::SetPossPiece(const std::vector<std::string>& poss, const std::vector<std::string>& name) {
  if (poss.empty()) {
    return;
  }
  for (auto pos: poss) {
    cboard_[ChessData::kMaxInd - (pos[1] - '0')][pos[0] - 'a'] = name;
  }
}

void Console::SetPossPiece(const std::string& pos, const std::vector<std::string>& name) {
  if (pos.empty()) {
    return;
  }
  cboard_[ChessData::kMaxInd - (pos[1] - '0')][pos[0] - 'a'] = name;
}

std::vector<std::vector<std::vector<std::string>>> Console::GetBoard() const {
  return cboard_;
}

inline void Console::PrintLine() const {
  for (std::size_t i = 0; i < (kSquareHeight + 1) * ChessData::kMaxInd; ++i) {
    std::cout << '-';
  }
  std::cout << '\n';
}

void Console::PrintSquare(const std::vector<std::string>& vec, const std::size_t ind) const {
  if (ind >= vec.size()) {
    std::cout << ' ';
    return;
  }
  std::cout << vec[ind];
  //std::cout << '|';
}

void Console::Print() {
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

void Console::Set() {
  SetPossPiece(board_.GetWhitePawns().GetPositions(), kPieceConsoleImages[0]);
  SetPossPiece(board_.GetBlackPawns().GetPositions(), kPieceConsoleImages[0]);
  SetPossPiece(board_.GetWhiteKnights().GetPositions(), kPieceConsoleImages[1]);
  SetPossPiece(board_.GetBlackKnights().GetPositions(), kPieceConsoleImages[1]);
  SetPossPiece(board_.GetWhiteBishops().GetPositions(), kPieceConsoleImages[2]);
  SetPossPiece(board_.GetBlackBishops().GetPositions(), kPieceConsoleImages[2]);
  SetPossPiece(board_.GetWhiteRooks().GetPositions(), kPieceConsoleImages[3]);
  SetPossPiece(board_.GetBlackRooks().GetPositions(), kPieceConsoleImages[3]);
  SetPossPiece(board_.GetWhiteQuenns().GetPositions(), kPieceConsoleImages[4]);
  SetPossPiece(board_.GetBlackQuenns().GetPositions(), kPieceConsoleImages[4]);
  SetPossPiece(board_.GetWhiteKing().GetPosition(), kPieceConsoleImages[5]);
  SetPossPiece(board_.GetBlackKing().GetPosition(), kPieceConsoleImages[5]);
}

void BMP::Set() {

}
