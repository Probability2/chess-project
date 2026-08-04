#include "Display.hpp"


// std::vector<std::vector<std::vector<std::string>>> ConsoleImage::GetBoard() const {
//   return cboard_;
// }

std::vector<std::vector<char>> ConsoleDefault::GetBoard() const {
  return cboard_;
}

// inline void ConsoleImage::PrintLine() const {
//   std::cout << std::string((kSquareHeight + 1) * kMaxInd, '-') << '\n';
// }

// inline void ConsoleImage::PrintSquare(const std::vector<std::string>& vec, const std::size_t ind) const {
//   std::cout << (ind < vec.size() ? vec[ind] : " ");
// }

// void ConsoleImage::PrintRow(const std::vector<std::vector<std::string>>& row) const {
//   for (std::size_t i = 0; i < kSquareLength; ++i) {
//     for (auto sq: row) {
//       PrintSquare(sq, i);
//     }
//     std::cout << '\n';
//   }
// }

// void ConsoleImage::Print() {
//   this->Set();
//   for (auto row: cboard_) {
//     PrintRow(row);
//     PrintLine();
//   }
// }

void ConsoleDefault::Print() {
  // this->Set();
  // for (const auto& row: cboard_) {
  //   for (const auto el: row) {
  //     std::cout << el;
  //   }
  //   std::cout << '\n';
  // }
}

// void ConsoleImage::Set() {
//   auto bound_function = [this](auto a, auto b) {
//     SetPossPiece<std::vector<std::string>>(a, b, cboard_);
//   };
//   bound_function(board_.GetWhitePawns().GetPositions(), kPieceConsoleImages[0]);
//   bound_function(board_.GetBlackPawns().GetPositions(), kPieceConsoleImages[0]);
//   bound_function(board_.GetWhiteKnights().GetPositions(), kPieceConsoleImages[1]);
//   bound_function(board_.GetBlackKnights().GetPositions(), kPieceConsoleImages[1]);
//   bound_function(board_.GetWhiteBishops().GetPositions(), kPieceConsoleImages[2]);
//   bound_function(board_.GetBlackBishops().GetPositions(), kPieceConsoleImages[2]);
//   bound_function(board_.GetWhiteRooks().GetPositions(), kPieceConsoleImages[3]);
//   bound_function(board_.GetBlackRooks().GetPositions(), kPieceConsoleImages[3]);
//   bound_function(board_.GetWhiteQuenns().GetPositions(), kPieceConsoleImages[4]);
//   bound_function(board_.GetBlackQuenns().GetPositions(), kPieceConsoleImages[4]);
//   bound_function(board_.GetWhiteKing().GetPosition(), kPieceConsoleImages[5]);
//   bound_function(board_.GetBlackKing().GetPosition(), kPieceConsoleImages[5]);
// }

void ConsoleDefault::Set() {
  // auto bound_function = [this](auto a, auto b) {
  //   SetPossPiece<char>(a, b, cboard_);
  // };
  // bound_function(board_.GetWhitePawns().GetPositions(), kWhiteBasicNames[0]);
  // bound_function(board_.GetBlackPawns().GetPositions(), std::tolower(kWhiteBasicNames[0]));
  // bound_function(board_.GetWhiteKnights().GetPositions(), kWhiteBasicNames[1]);
  // bound_function(board_.GetBlackKnights().GetPositions(), std::tolower(kWhiteBasicNames[1]));
  // bound_function(board_.GetWhiteBishops().GetPositions(), kWhiteBasicNames[2]);
  // bound_function(board_.GetBlackBishops().GetPositions(), std::tolower(kWhiteBasicNames[2]));
  // bound_function(board_.GetWhiteRooks().GetPositions(), kWhiteBasicNames[3]);
  // bound_function(board_.GetBlackRooks().GetPositions(), std::tolower(kWhiteBasicNames[3]));
  // bound_function(board_.GetWhiteQuenns().GetPositions(), kWhiteBasicNames[4]);
  // bound_function(board_.GetBlackQuenns().GetPositions(), std::tolower(kWhiteBasicNames[4]));
  // bound_function(board_.GetWhiteKing().GetPosition(), kWhiteBasicNames[5]);
  // bound_function(board_.GetBlackKing().GetPosition(), std::tolower(kWhiteBasicNames[5]));
}

void BMP::Set() {

}