#include "FileManager.hpp"

std::size_t FileManager::call_ = 0;

Board* TxtManager::board_;

Game* FenManager::game_;

TxtManager& TxtManager::operator=(const TxtManager& manager) {
  board_ = manager.board_;

  return *this;
}

void TxtManager::SetValue(Board& board) {
  board_ = &board;
}

inline void SaveKings(Board* board, std::vector<std::vector<char>>& display) {
  display[ChessData::kMaxInd - (board->GetWhiteKing().GetPosition()[1] - '0')][board->GetWhiteKing().GetPosition()[0] - 'a'] = 'K';
  display[ChessData::kMaxInd - (board->GetBlackKing().GetPosition()[1] - '0')][board->GetBlackKing().GetPosition()[0] - 'a'] = 'k';
}

std::vector<std::vector<char>> GetPicture(Board* board) {
  std::vector<std::vector<char>> display(ChessData::kMaxInd, std::vector<char>(ChessData::kMaxInd, '.'));
  for (std::size_t i = 0; i < board->pieces_.size(); ++i) {
    for (const auto pos: board->pieces_[i]->GetPositions()) {
      display[ChessData::kMaxInd - (pos[1] - '0')][pos[0] - 'a'] = board->pchars_[i];
    }
  }
  SaveKings(board, display);

  return display;
}

void TxtManager::DownloadToFile(std::ofstream& file, const std::vector<std::vector<char>>& display) {
  for (std::size_t i = 0; i < ChessData::kMaxInd; ++i) {
    for (std::size_t j = 0; j < ChessData::kMaxInd; ++j) {
      file << display[i][j];
    }
    file << '\n';
  }
}

std::ofstream FileManager::CreateFile() {
  ++call_;
  fs::path directorypath = "myfiles";
  if (!fs::exists(directorypath)) {
    fs::create_directory(directorypath); 
  }
  std::ofstream file(directorypath / CreateName(), std::ios::out);
  if (!file) {
    std::cerr << "Cannot create the file" << '\n';
    std::exit(EXIT_FAILURE);
  }

  return file;
}

void TxtManager::Save() {
  std::ofstream file = CreateFile();
  std::vector<std::vector<char>> vec;
  DownloadToFile(file, GetPicture(board_));
  file.close();
}

inline void FenManager::FillFenSkips(std::size_t& skip, std::ofstream& file) {
  if (!skip) {
    file << skip;
    skip = 0;
  }
}

void FenManager::ProcessFenRow(std::size_t& skip, std::vector<char>& vec, std::ofstream& file) {
  for (std::size_t j = 0; j < ChessData::kMaxInd; ++j) {
    if (vec[j] != kEmptySquare) {
      FillFenSkips(skip, file);
      file << vec[j];
      continue;
    }
    ++skip;
  }
}

void FenManager::ProcessFenBoard(std::ofstream& file) {
  std::vector<std::vector<char>> vec = GetPicture(game_->GetBoard());
  std::size_t skip = 0;
  for (std::size_t i = 0; i < ChessData::kMaxInd; ++i) {
    ProcessFenRow(skip, vec[i], file);
    file << kFenDelimeter;
    FillFenSkips(skip, file);
  }
}

void FenManager::Save() {
  std::ofstream file = CreateFile();
  ProcessFenBoard(file);

  file.close();
}

std::string FileManager::CreateName() {
  return kFilePrefix + std::to_string(call_) + ".txt";
}

void FenManager::Get(const std::string& file_name) {
  std::ifstream file(file_name, std::ios::in);
  if (!file) {
    std::cerr << "File " << file_name << " is not found\n";
    std::exit(EXIT_FAILURE);
  }
  file.close();
}

void TxtManager::Get(const std::string& file_name) {
  std::ifstream file(file_name, std::ios::in);
  if (!file) {
    std::cerr << "File " << file_name << " is not found\n";
    std::exit(EXIT_FAILURE);
  }
  ReadImage(file);
  file.close();
}

void TxtManager::ReadImage(std::ifstream& file) {
  std::string line;
  char k = '0' + ChessData::kMaxInd;
  while (std::getline(file, line) && k >= '1') {
    for (std::size_t i = 0; i < ChessData::kMaxInd; ++i) {
      board_->ReadPosition(line[i], std::string(1, static_cast<char>(ChessData::kMinCoord + i)) + std::string(1, k));
    }
    --k;
  }
  std::cout << "Positions 1:\t\t" << board_->GetWhiteKing().GetPosition() << '\n';
  std::cout << "Positions 2:\t\t" << board_->GetBlackKing().GetPosition() << '\n';
}