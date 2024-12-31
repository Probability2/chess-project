#include "FileManager.hpp"

std::size_t FileManager::call_ = 0;

Board* FileManager::board_;

FileManager& FileManager::operator=(const FileManager& manager) {
  board_ = manager.board_;

  return *this;
}

void FileManager::SetValue(Board& board) {
  std::cout << "22222222\n";
  board_ = &board;
}

std::vector<std::vector<char>> FileManager::GetPicture() {
  std::vector<std::vector<char>> display(ChessData::kMaxInd, std::vector<char>(ChessData::kMaxInd, '.'));
  for (std::size_t i = 0; i < board_->pieces_.size(); ++i) {
    for (auto pos: board_->pieces_[i]->GetPositions()) {
      display[pos[1] - '0' - 1][pos[0] - 'a'] = board_->pchars_[i];
    }
  }

  return display;
}

void FileManager::DownloadToFile(std::ofstream& file, const std::vector<std::vector<char>>& display) {
  for (std::size_t i = 0; i < ChessData::kMaxInd; ++i) {
    for (std::size_t j = 0; j < ChessData::kMaxInd; ++j) {
      file << display[i][j];
    }
    file << '\n';
  }
}

void FileManager::SaveImage() {
  ++call_;
  std::ofstream file(CreateName(), std::ios::out);
  if (!file) {
    std::cerr << "Cannot create the file" << '\n';
    std::exit(EXIT_FAILURE);
  }
  DownloadToFile(file, GetPicture());

  file.close();
}

void FileManager::SaveFEN() {
  ++call_;
  std::ofstream file(CreateName(), std::ios::out);
  file.close();
}

std::string FileManager::CreateName() {
  return kFilePrefix + std::to_string(call_) + ".txt";
}

void FileManager::GetFromFEN(const std::string& file_name) {

}

void FileManager::GetFromImage(const std::string& file_name) {
  std::ifstream file(file_name, std::ios::in);
  if (!file) {
    std::cerr << "File " << file_name << " is not found\n";
    std::exit(EXIT_FAILURE);
  }
  ReadImage(file);
  file.close();
}

void FileManager::ReadImage(std::ifstream& file) {
  std::string line;
  char k = '0' + ChessData::kMaxInd;
  while (std::getline(file, line) && k >= '1') {
    for (std::size_t i = 0; i < ChessData::kMaxInd; ++i) {
      board_->ReadPosition(line[i], std::string(1, static_cast<char>('a' + i)) + std::string(1, k));
    }
    --k;
  }
}
