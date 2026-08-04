#include "FileManager.hpp"

std::size_t FileManager::call_ = 0;

template<typename T>
inline bool contains(const std::vector<T>& vec, const T& p) {
  return std::find(vec.begin(), vec.end(), p) != vec.end();
}

std::string FileManager::CreateName() {
  return std::format("chess{}.txt", call_);
}

std::ofstream FileManager::CreateFile() {
  call_++;
  fs::path directorypath = kDirectoryPrefix;
  if (!fs::exists(directorypath)) {
    fs::create_directory(directorypath); 
  }
  std::ofstream file(directorypath / CreateName(), std::ios::out);
  if (!file) {
    std::cerr << "Cannot create the file" << '\n';
    throw std::invalid_argument("Cannot create file");
  }

  return file;
}

void FenManager::Save(const Position& pos, std::string_view file_name) {
  std::ofstream file = CreateFile();

}

std::expected<Position, std::string_view> FenManager::Get(const fs::path& file_name) {
  std::ifstream file(file_name, std::ios::in);
  Position pos;
  if (!file) {
    return std::unexpected(to_string(ErrorCode::kFileNotFound));
  }
  std::string data;
  std::getline(file, data);
  auto res = Get(std::string_view(data));
  if (!res) {
    return std::unexpected(to_string(ErrorCode::kDataIsDamaged));
  }
  pos = *res;
  file.close();

  return pos;
}
