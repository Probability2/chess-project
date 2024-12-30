#pragma once

#include "ChessFormat.hpp"

constexpr const char* kFilePrefix = "chess";

class FileManager{
public:
  FileManager() = delete;

  static void SetValue(Board&&);

  FileManager& operator=(const FileManager& manager);

  static void SaveImage();

  static void SaveFEN();

  static void GetFromImage(const std::string& file_name);

  static void GetFromFEN(const std::string& file_name);

private:
  static Board board_;

  static std::size_t call_;

  static void ReadImage(std::ifstream& file);
};