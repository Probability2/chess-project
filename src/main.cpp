#define UNICODE

#include "lib/History.hpp"
#include "lib/ChessPiece.hpp"
#include "lib/ChessFormat.hpp"
#include "lib/ChessGame.hpp"
#include "lib/DisplayBoard.hpp"
#include "parser/ArgParser.hpp"
#include "lib/FileManager.hpp"

#include <conio.h>

const int kEscapeCode = 0x1b;

int main(int argc, char** argv) {
  system("chcp 65001");

  ArgumentParser::ArgParser parser("ChessProject");
  parser.SetUpParser();
  std::vector<std::string> moves;

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
  for (;;) {
    if (_getch() == kEscapeCode) {
      return EXIT_SUCCESS;
    }
    std::string request;
    std::getline(std::cin, request);
    if (request == "default") {
      board.SetDefault();
    } else if (request == "clear") {
      board.Clear();
    } else if (request == "set") {
      board.SetPosition();
    } else if (request == "get from image") {
      std::cout << "File name: ";
      std::getline(std::cin, request);
      FileManager::SetValue(std::move(board));
      FileManager::GetFromImage(request);

      //board.GetFromImage(request);
    } else if (request == "escape") {
      return EXIT_SUCCESS;
    } else if (request == "change lang") {

    } else if (request == "start") {
      Game game;
    } else if (request == "save image") {
      //board.SaveImage();
    } else if (request == "save fen") {
      //board.SaveFEN();
    } else {
      continue;
    }
    //system("cls");
    console.Print();
  }

  return EXIT_SUCCESS;
}
