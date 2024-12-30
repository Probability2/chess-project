#include "FileManager.hpp"

std::size_t FileManager::call_ = 0;

Board FileManager::board_;

void FileManager::SetValue(Board&& board) {
  board_ = std::forward<Board>(board);
}

FileManager& FileManager::operator=(const FileManager& manager) {
  board_ = manager.board_;

  return *this;
}

void FileManager::SaveImage() {
  ++call_;
  std::ofstream file(CreateName(), std::ios::out);
  file.close();
}

void FileManager::SaveFEN() {
  ++call_;
  std::ofstream file(CreateName(), std::ios::out);
  file.close();
}

std::string FileManager::CreateName() {
  return kFilePrefix + std::to_string(call_) + "txt";
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
      board_.ReadPosition(line[i], std::string(1, static_cast<char>('a' + i)) + std::string(1, k));
    }
    --k;
  }
}
