#include "lib/History.hpp"
#include "lib/ChessPiece.hpp"
#include "lib/ChessFormat.hpp"
#include "lib/DisplayBoard.hpp"
#include "parser/ArgParser.hpp"

#include <conio.h>

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"  // Пример для GLFW
#include "backends/imgui_impl_opengl3.h"  // Пример для OpenGL

//#include <GLFW/glfw3.h>

const int kEscapeCode = 0x1b;

int main(int argc, char** argv) {
  ArgumentParser::ArgParser parser("ChessProject");
  std::vector<std::string> history_args;
  std::vector<std::string> moves;
  parser.SetUpParser(history_args);

  if (!parser.Parse(argc, argv)) {
    parser.DisplayError();
    return EXIT_FAILURE;
  }

  Board board;
  Console console(board);
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
    } else if (request == "escape") {
      return EXIT_SUCCESS;
    } else {
      continue;
    }
    system("cls");
    console.Print();
  }

  return EXIT_SUCCESS;
}
