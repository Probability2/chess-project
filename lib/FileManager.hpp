#pragma once

#include "ChessFormat.hpp"
#include "ChessGame.hpp"

#include <filesystem>
#include <format> 

namespace fs = std::filesystem;

constexpr const char* kFilePrefix = "chess";

constexpr const char* kDirectoryPrefix = "myfiles";

constexpr char kFenDelimeter = '/';

constexpr char kSpaceDelimiter = ' ';

constexpr size_type kCastleCount = 4;

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
  TxtManager& operator=(TxtManager&& manager) = delete;

  static void SetValue(std::unique_ptr<Board>);

  static void Save();

  static void Get(const std::string& file_name);

private:
  static std::unique_ptr<Board> board_;

  static void DownloadToFile(std::ofstream& file, const std::vector<std::vector<char>>& display);

  static void ReadImage(std::ifstream& file);

  static void ProcessRow(const std::string& line, const int k);
};

class FenManager: public FileManager {
public:
  static void SetValue(std::unique_ptr<GameState>);

  static void Save();

  static void Get(const std::string& file_name);
  
private:
  static std::unique_ptr<GameState> game_;

  static void ReadFen(std::ifstream& file);

  static void ReadPlacement(std::ifstream& file);

  inline static void FillFenSkips(std::size_t& skips, std::ofstream& file);

  static void ProcessFenBoard(std::ofstream& file);

  static void ProcessFenRow(std::size_t& skip, std::vector<char>& vec, std::ofstream& file);

  static void SetTurn(std::ifstream& file, Parameters& param);

  static void SetCastle(std::ifstream& file, Parameters& param);

  static void GetCastle(const char castle, Parameters& param);

  static void SetEnPassant(std::ifstream& file, Parameters& param);

  static void SetNoCaptures(std::ifstream& file, Parameters& param);

  static void SetMove(std::ifstream& file, Parameters& param);
};