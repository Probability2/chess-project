#pragma once

#include "lib/Position.hpp"
#include "lib/FileManager.hpp"
#include "lib/Game.hpp"

#include <map>
#include <memory>

void SetDefault(Game& game) {
  //state.board_->SetDefault();
}

void Clear(Game& game) {
  //state.board_->Clear();
}

void SetPosition(Game& game) {
  //state.board_->SetPosition();
}

std::expected<Position, std::string_view> GetFromFEN() {
  std::string file_name;
  std::cout << "File name: ";
  std::getline(std::cin, file_name);
  
  return FenManager::Get(fs::path(file_name));
}

void Start(Game&) {
  Game game;
}


void SaveFEN(Game& game) {
  // FenManager::Save();
}

void EXIT() {
  std::exit(EXIT_SUCCESS);
}

void Move(std::string move) {
  
}