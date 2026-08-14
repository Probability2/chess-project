#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>

#include "lib/History.hpp"
#include "lib/FileManager.hpp"
#include "lib/MoveGenerator.hpp"
#include "parser/ArgParser.hpp"
#include "core/commands.hpp"

#include <conio.h>

using namespace chess;

constexpr int kEscapeCode = 0x1b;

int main(int argc, char** argv) {
  #ifdef _WIN32
  SetConsoleOutputCP(CP_UTF8);
  SetConsoleCP(CP_UTF8);
  #endif


  ArgumentParser::ArgParser parser("ChessProject");
  parser.SetUpParser();
  if (!parser.Parse(argc, argv)) {
    parser.DisplayError();
    return EXIT_FAILURE;
  }
  if (parser.Help()) {  
    std::cout << parser.HelpDescription() << '\n';
    return EXIT_SUCCESS;
  }
  // Bitboard x = (1ULL << 34) | (1ULL << 17) | (1ULL << 13) | (1ULL << 10) | (1ULL << 8) | (1ULL << 2);
  // utils::PrintBitboard(x);
  chess::Position pos;
  std::string request;
  pos = *(chess::fen_manager::Get("rnbqkbnr/ppp1p1pp/8/7P/PP6/3p1p2/2PPPPP1/RNBQKBNR w Qkq - 0 7"));
  std::cout << pos;
  MoveList moves = chess::move_generator::GenerateMoves<MovesType::kPseudo>(pos);
  std::cout << moves.size() << '\n';
  std::cout << moves << '\n';

  // for (;;) {
  //   std::cout << pos << '\n';
  //   // game.PrintPosition();
  //   if (!std::getline(std::cin, request)) {
  //     return EXIT_SUCCESS;
  //   }
  //   if (request == "get from file" || request == "get from fen") {
  //     if (auto res = GetFromFEN(); res.has_value()) {
  //       pos = chess::Position(*res);
  //     } else {
  //       std::cerr << res.error() << '\n';
  //     }
  //   } else if (request == "save") {
  //     SaveFEN(pos);
  //   } else if (request == "exit" || request == "quit") {
  //     break;
  //   }
  // }

  return EXIT_SUCCESS;
}

// #include <SFML/Graphics.hpp>
// #include <iostream>
