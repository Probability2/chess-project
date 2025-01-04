#include "lib/History.hpp"
#include "lib/ChessPiece.hpp"
#include "lib/ChessFormat.hpp"
#include "lib/ChessGame.hpp"
#include "lib/DisplayBoard.hpp"
#include "lib/FileManager.hpp"
#include "parser/ArgParser.hpp"
#include "core/commands.hpp"
#include "utils/Random.hpp"

#include <conio.h>

constexpr int kEscapeCode = 0x1b;

int main(int argc, char** argv) {
  system("chcp 65001");
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
  Board board;
  ConsoleDefault console(board);
  std::vector<std::string> moves = {"e4", "e5", "Nf3", "Nc6", "Bc4", "Nf6", "Ng5", "d5", "ed5", "Na5", "Bb5+", "c6", "dc6",
                                    "bc6", "Bd3"};
  Game game(moves);
  for (auto i: moves) {
    std::cout << i;
  }
  std::map<std::string, std::function<void(Board&)>> funcs = {{"default", SetDefault}, {"clear", Clear}, {"set", SetPosition}
                                                             ,{"get from image", GetFromFile}, {"get from file", GetFromFile}
                                                             ,{"start", Start}, {"save image", SaveFile}, {"save file", SaveFile}
                                                             , {"escape", EXIT}, {"exit", EXIT}};
  for (;;) {
    if (_getch() == kEscapeCode) {
      return EXIT_SUCCESS;
    }
    std::string request;
    std::getline(std::cin, request);
    if (funcs.contains(request)) {
      funcs[request](board);
    }
    system("cls");
    console.Print();
  }

  return EXIT_SUCCESS;
}