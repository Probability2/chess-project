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
  TxtManager::SetValue(board);
  TxtManager::Get(request);
}

void Start(Board& board) {
  Game game;
}

void SaveFile(Board& board) {
  TxtManager::SetValue(board);
  TxtManager::Save();
}

//void SaveFEN(Board& board) {
//  FenManager::SetValue(board);
//  FenManager::Save();
//}

void EXIT(Board& board) {
  std::exit(EXIT_SUCCESS);
}