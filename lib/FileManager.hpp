#pragma once

#include "ChessFormat.hpp"
#include "ChessGame.hpp"

#include <filesystem>

namespace fs = std::filesystem;

constexpr const char* kFilePrefix = "chess";

constexpr char kFenDelimeter = '/';

class FileManager {
protected:
  FileManager() = delete;

  virtual ~FileManager();

  static std::ofstream CreateFile();

  static std::size_t call_;

  static std::string CreateName();
};


class TxtManager: public FileManager {
public:
  TxtManager& operator=(const TxtManager& manager);

  static void SetValue(Board&);

  static void Save();

  static void Get(const std::string& file_name);

private:
  static Board* board_;

  static void DownloadToFile(std::ofstream& file, const std::vector<std::vector<char>>& display);

  static void ReadImage(std::ifstream& file);
};

class FenManager: public FileManager {
public:
  static void Save();

  static void Get(const std::string& file_name);
  
private:
  static Game* game_;

  inline static void FillFenSkips(std::size_t& skips, std::ofstream& file);

  static void ProcessFenPositions(std::ofstream& file);
};