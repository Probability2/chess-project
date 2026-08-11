#pragma once

#include "lib/Position.hpp"
#include "lib/FileManager.hpp"
#include "lib/Game.hpp"

#include <map>
#include <memory>

std::expected<chess::Position, std::string_view> GetFromFEN() {
  std::string file_name;
  std::cout << "File name: ";
  if (!std::getline(std::cin, file_name)) {
    return std::unexpected("Input cancelled");
  }
  
  return chess::fen_manager::Get(fs::path(std::move(file_name)));
}


void SaveFEN(const chess::Position& pos) {
  if (auto save_file = chess::fen_manager::Save(pos); save_file.has_value()) {
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