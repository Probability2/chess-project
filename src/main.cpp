#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>

#include "lib/History.hpp"
#include "lib/Piece.hpp"
#include "lib/Position.hpp"
#include "lib/Game.hpp"
#include "lib/Display.hpp"
#include "lib/FileManager.hpp"
#include "parser/ArgParser.hpp"
#include "core/commands.hpp"

#include <conio.h>

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
  //GameState game_state;
  //ConsoleDefault console(game_state.board_);
  std::vector<std::string> moves = {"e4", "e5", "Nf3", "Nc6", "Bc4", "Nf6", "Ng5", "d5", "ed5", "Na5", "Bb5+", "c6", "dc6",
                                    "bc6", "Bd3", "Nd5", "Nf3", "Bd6", "0-0"};
  Game game;
  //for (auto i: game) {
  //  std::cout << i << ' ';
  //}
  //std::cout << '\n';
  // std::map<std::string, std::function<void(Game&)>> funcs = {{"default", SetDefault}, {"clear", Clear}, {"set", SetPosition}
  //                                                             , {"get from file", GetFromFEN}
  //                                                             ,{"get from fen", GetFromFEN}, {"save fen", SaveFEN}
  //                                                            , {"start", Start}, {"save pos", SaveFEN}, {"save", SaveFEN}
  //                                                            , {"save game", SaveFEN}, {"escape", EXIT}, {"exit", EXIT}};
  std::string request;
  for (;;) {
    game.PrintPosition();
    if (!std::getline(std::cin, request)) {
      return EXIT_SUCCESS;
    }
    if (request == "get from file" || request == "get from fen") {
      if (auto res = GetFromFEN(); res.has_value()) {
        game = Game(*res);
      } else {
        std::cerr << res.error() << '\n';
      }
    } else if (request == "save") {
      SaveFEN(game);
    } else if (request == "exit" || request == "quit") {
      break;
    }
  }

  return EXIT_SUCCESS;
}

// #include <SFML/Graphics.hpp>
// #include <iostream>
