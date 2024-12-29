#include "ChessFormat.hpp"

Board::Board(const Board& board) {
  *this = board;
}

Board::Board(Board&& board) {
  *this = board;
}

Board& Board::operator=(const Board& board) {
  if (this != &board) {
    this->w_pawns_ = board.GetWhitePawns();
    this->b_pawns_ = board.GetBlackPawns();

    this->w_knights_ = board.GetWhiteKnights();
    this->b_knights_ = board.GetBlackKnights();

    this->w_bishops_ = board.GetWhiteBishops();
    this->b_bishops_ = board.GetBlackBishops();

    this->w_rooks_ = board.GetWhiteRooks();
    this->b_rooks_ = board.GetBlackRooks();

    this->w_quenns_ = board.GetWhiteQuenns();
    this->b_quenns_ = board.GetBlackQuenns();

    this->w_king_ = board.GetWhiteKing();
    this->b_king_ = board.GetBlackKing();
  }

  return *this;
}

Standard::Standard() {
}

FisherRandom::FisherRandom() {
}

void Standard::ViewBoard() const {
}

void FisherRandom::ViewBoard() const {
}

void Board::SetColoursPosition(const std::size_t in_state) {
  for (std::size_t i = in_state; i < in_state + kCountPiece / 2; ++i) {
    std::cout << "I: " << i << '\n';
    std::cout << pnames_[i] << ": ";
    InputCoords(i);
  }
}

void Board::InputCoords(const std::size_t ind) {
  std::string pos;
  while (std::getline(std::cin, pos) && !pos.empty()) {
      this->SetPosition(*pieces_[ind], pos);
  }
}

std::string Board::GetPosKing() {
  std::string pos;
  std::getline(std::cin, pos);

  return pos;
}

void Board::SetPosition() {
  std::cout << "White:\n";
  SetColoursPosition(0);
  std::cout << "White King: ";
  w_king_.SetPosition(GetPosKing());
  std::cout << "Black:\n";
  SetColoursPosition(kCountPiece / 2);
  std::cout << "Black King: ";
  b_king_.SetPosition(GetPosKing());
}

void Board::SetDefault() {
  w_pawns_.AddPositions("a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2");
  b_pawns_.AddPositions("a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7");
  w_knights_.AddPositions("b1", "g1");
  b_knights_.AddPositions("b8", "g8");
  w_bishops_.AddPositions("c1", "f1");
  b_bishops_.AddPositions("c8", "f8");
  w_rooks_.AddPositions("a1", "h1");
  b_rooks_.AddPositions("a8", "h8");
  w_quenns_.AddPositions("d1");
  b_quenns_.AddPositions("d8");
  w_king_.SetPosition("e1");
  b_king_.SetPosition("e8");
}

void Board::ReadPosition(const char piece, const std::string& pos) {
  std::cout << pos << " poss\n";
  switch (piece) {
    case 'p':
      b_pawns_.AddPositions(pos);
      break;
    case 'P':
      w_pawns_.AddPositions(pos);
      break;
    case 'n':
      b_knights_.AddPositions(pos);
      break;
    case 'N':
      w_knights_.AddPositions(pos);
      break;
    case 'b':
      b_bishops_.AddPositions(pos);
      break;
    case 'B':
      w_bishops_.AddPositions(pos);
      break;
    case 'r':
      b_rooks_.AddPositions(pos);
      break;
    case 'R':
      w_rooks_.AddPositions(pos);
      break;
    case 'q':
      b_quenns_.AddPositions(pos);
      break;
    case 'Q':
      w_quenns_.AddPositions(pos);
      break;
    case 'k':
      b_king_.SetPosition(pos);
      break;
    case 'K':
      w_king_.SetPosition(pos);
      break;
    default:
      break;
  }
}

void Board::ReadImage(std::ifstream& file) {
  std::string line;
  char k = ChessData::kMaxInd + '0' + 1;
  while (std::getline(file, line) && k-->'0') {
    for (std::size_t i = 0; i < ChessData::kMaxInd; ++i) {
      ReadPosition(line[i], std::string(1, 'a' + i) + std::string(1, k));
    }
  }
}

void Board::GetFromImage(const std::string& file_name) {
  std::ifstream file(file_name, std::ios::in);
  if (!file) {
    std::cerr << "File " << file_name << " is not found\n";
    std::exit(EXIT_FAILURE);
  }
  ReadImage(file);
  file.close();
}

const Pawn& Board::GetWhitePawns() const {
  return w_pawns_;
}

void Board::SaveImage() const {
  //std::ofstream("hello.txt", std::in);
}

void Board::SaveFEN() const {
  //std::ofstream("hello.txt", std::in);
}

const Pawn& Board::GetBlackPawns() const {
  return b_pawns_;
}

const Knight& Board::GetWhiteKnights() const {
  return w_knights_;
}

const Knight& Board::GetBlackKnights() const {
  return b_knights_;
}

const Bishop& Board::GetWhiteBishops() const {
  return w_bishops_;
}

const Bishop& Board::GetBlackBishops() const {
  return b_bishops_;
}

const Rook& Board::GetWhiteRooks() const {
  return w_rooks_;
}

const Rook& Board::GetBlackRooks() const {
  return b_rooks_;
}

const Quenn& Board::GetWhiteQuenns() const {
  return w_quenns_;
}

const Quenn& Board::GetBlackQuenns() const {
  return b_quenns_;
}

const King& Board::GetWhiteKing() const {
  return w_king_;
}

const King& Board::GetBlackKing() const {
  return b_king_;
}

Pawn Board::GetAllPawns() const {
  return w_pawns_ + b_pawns_;
}

Knight Board::GetAllKnights() const {

  return w_knights_ + b_knights_;
}

Bishop Board::GetAllBishops() const {
  return w_bishops_ + b_bishops_;
}

Rook Board::GetAllRooks() const {

  return w_rooks_ + b_rooks_;
}

Quenn Board::GetAllQuenns() const {
  return w_quenns_ + b_quenns_;
}

void Board::Clear() {
  std::cout << "Clear\n";
}
