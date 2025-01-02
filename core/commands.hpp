#pragma once

#include "lib/ChessFormat.hpp"
#include "lib/FileManager.hpp"
#include "lib/ChessGame.hpp"

#include <map>

void SetDefault(Board& board) {
  board.SetDefault();
}

void Clear(Board& board) {
  board.Clear();
}

void SetPosition(Board& board) {
  board.SetPosition();
}

void GetFromFile(Board& board) {
  std::string request;
  std::cout << "File name: ";
  std::getline(std::cin, request);
  FileManager::SetValue(board);
  FileManager::GetFromImage(request);
}

void Start(Board& board) {
  Game game;
}

void SaveFile(Board& board) {
  FileManager::SetValue(board);
  FileManager::SaveImage();
}

void SaveFEN(Board& board) {
  FileManager::SetValue(board);
  FileManager::SaveFEN();
}

void EXIT(Board& board) {
  std::exit(EXIT_SUCCESS);
}