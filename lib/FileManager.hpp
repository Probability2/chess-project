#pragma once

#include "ChessFormat.hpp"

#include <filesystem>

namespace fs = std::filesystem;

constexpr const char* kFilePrefix = "chess";

constexpr char kFenDelimeter = '/';

struct FenParameters {
  bool is_short_castle_;
  bool is_long_castle_;
  bool is_white_move;
  bool is_en_passant;
  std::size_t no_capture_moves;
  std::size_t move;
};

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

  FenParameters fen_;

  inline static void FillFenSkips(std::size_t& skips, std::ofstream& file);

  static std::ofstream CreateFile();

  static void ProcessFenPositions(std::ofstream& file);

  static void SaveKings(std::vector<std::vector<char>>& display);

  static std::vector<std::vector<char>> GetPicture();

  static void DownloadToFile(std::ofstream& file, const std::vector<std::vector<char>>& display);

  static std::string CreateName();

  static void ReadImage(std::ifstream& file);
};
