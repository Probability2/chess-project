#pragma once

#include "lib/ChessFormat.hpp"
#include "lib/FileManager.hpp"
#include "lib/ChessGame.hpp"

#include <map>

void SetDefault(GameState& state) {
  state.board_->SetDefault();
}

void Clear(GameState& state) {
  state.board_->Clear();
}

void SetPosition(GameState& state) {
  state.board_->SetPosition();
}

void GetFromFile(GameState& state) {
  std::string request;
  std::cout << "File name: ";
  std::getline(std::cin, request);
  TxtManager::SetValue(*(state.board_));
  std::cout << "Sett\n";
  TxtManager::Get(request);
}

void GetFromFEN(GameState& state) {
  std::string request;
  std::cout << "File name: ";
  std::getline(std::cin, request);
  FenManager::SetValue(state);
  FenManager::Get(request);
}

void Start(GameState&) {
  Game game;
}

void SaveFile(GameState& state) {
  TxtManager::SetValue(*(state.board_));
  TxtManager::Save();
}

void SaveFEN(GameState& state) {
  FenManager::SetValue(state);
  FenManager::Save();
}

void EXIT(GameState&) {
  std::exit(EXIT_SUCCESS);
}