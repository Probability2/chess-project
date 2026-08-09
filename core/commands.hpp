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
  
  return fen_manager::Get(fs::path(file_name));
}


void SaveFEN(Game& game) {
  if (auto save_file = fen_manager::Save(game.get_current_position()); save_file.has_value()) {
    std::cout << "The file created successfully\n";
  } else {
    std::cerr << save_file.error() << '\n';
  }
}

void EXIT() {
  std::exit(EXIT_SUCCESS);
}

void Move(std::string move) {
  
}