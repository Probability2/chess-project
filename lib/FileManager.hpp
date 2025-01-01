#pragma once

#include "ChessFormat.hpp"

constexpr const char* kFilePrefix = "chess";

class FileManager{
public:
  FileManager() = delete;

  //static void SetValue(Board&&);

  static void SetValue(Board&);

  FileManager& operator=(const FileManager& manager);

  static void SaveImage();

  static void SaveFEN();

  static void GetFromImage(const std::string& file_name);

  static void GetFromFEN(const std::string& file_name);

private:
  static Board* board_;

  static std::size_t call_;

  static void SaveKings(std::vector<std::vector<char>>& display);

  static std::vector<std::vector<char>> GetPicture();

  static void DownloadToFile(std::ofstream& file, const std::vector<std::vector<char>>& display);

  static std::string CreateName();

  static void ReadImage(std::ifstream& file);
};
