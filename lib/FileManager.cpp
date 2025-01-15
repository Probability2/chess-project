#include "FileManager.hpp"

std::size_t FileManager::call_ = 0;

std::unique_ptr<Board> TxtManager::board_ = nullptr;

std::unique_ptr<GameState> FenManager::game_ = nullptr;

void TxtManager::SetValue(std::unique_ptr<Board>&& board) {
  board_ = std::move(board);
  if (board_ == nullptr) {
    std::cout << "ALCARAAAAAAAAAAAAAAZ\n";
  }
}

void FenManager::SetValue(std::unique_ptr<GameState> state) {
  game_ = std::move(state);
}

inline void SaveKings(std::unique_ptr<Board>& board, std::vector<std::vector<char>>& display) {
  display[ChessData::kMaxInd - (board->GetWhiteKing().GetPosition()[1] - '0')][board->GetWhiteKing().GetPosition()[0] - 'a'] = 'K';
  display[ChessData::kMaxInd - (board->GetBlackKing().GetPosition()[1] - '0')][board->GetBlackKing().GetPosition()[0] - 'a'] = 'k';
}

std::vector<std::vector<char>> GetPicture(std::unique_ptr<Board>& board) {
  std::vector<std::vector<char>> display(ChessData::kMaxInd, std::vector<char>(ChessData::kMaxInd, '.'));
  for (std::size_t i = 0; i < board->pieces_.size(); ++i) {
    for (const auto pos: board->pieces_[i]->GetPositions()) {
      display[ChessData::kMaxInd - (pos[1] - '0')][pos[0] - 'a'] = pchars_[i];
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
  if (skip > 0) {
    file << skip;
    skip = 0;
  }
}

void FenManager::ProcessFenRow(std::size_t& skip, std::vector<char>& vec, std::ofstream& file) {
  for (auto v: vec) {
    if (v != kEmptySquare) {
      FillFenSkips(skip, file);
      file << v;
      continue;
    }
    ++skip;
  }
}

void FenManager::ProcessFenBoard(std::ofstream& file) {
  std::vector<std::vector<char>> vec = GetPicture(game_->GetBoard());
  std::size_t skip = 0;
  for (auto& v: vec) {
    ProcessFenRow(skip, v, file);
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
  return std::format("chess{}.txt", call_);
}

void FenManager::Get(const std::string& file_name) {
  std::ifstream file(file_name, std::ios::in);
  if (!file) {
    std::cerr << "File " << file_name << " is not found\n";
    return;
  }
  file.close();
}

void TxtManager::Get(const std::string& file_name) {
  std::ifstream file(file_name, std::ios::in);
  if (!file) {
    std::cerr << "File " << file_name << " is not found\n";
    return;
  }
  std::cout << "Do know\n";
  ReadImage(file);
  file.close();
}

void TxtManager::ReadImage(std::ifstream& file) {
  std::string line;
  char k = '0' + ChessData::kMaxInd;
  while (std::getline(file, line) && k >= '1') {
    ProcessRow(line, k);
    --k;
  }
}

void TxtManager::ProcessRow(const std::string& line, const int k) {
  std::cout << "pppp\n";
  if (board_ == nullptr) {
    std::cout << "NULLPTR\n";
  }
  for (std::size_t i = 0; i < line.size(); ++i) {
    std::cout << "yep\n";
    board_->ReadPosition(line[i], std::format("{}{}", static_cast<char>(ChessData::kMinCoord + i), k - '0'));
  }
  std::cout << k << '\n';
}