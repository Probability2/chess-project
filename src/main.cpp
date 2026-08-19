#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>

#include "lib/History.hpp"
#include "lib/FileManager.hpp"
#include "lib/MoveGenerator.hpp"
#include "parser/ArgParser.hpp"
#include "core/commands.hpp"

#include <conio.h>

#include <random>

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
  chess::Position pos;
  std::string request;
  Bitboard b = 0x0000000010000000ULL;//e4 square, rooks
  
  std::random_device rd;
  std::mt19937_64 magic_generator(rd());

  // chess::MoveList list;
  // chess::move_generator::GenerateRookMoves<ColorType::kWhite>(list, fen_manager::Get("3qk3/8/3p4/8/1bBR4/8/3P4/4K3 w - - 0 1"));

  auto pp = *(chess::fen_manager::Get("3qk3/8/3p4/8/1bBR4/8/3P4/4K3 w - - 0 1"));
  auto list = chess::move_generator::GenerateMoves<MovesType::kPseudo>(pp);
  std::cout << flipped(pp) << '\n';

  // Bitboard magic = 0;
  // // std::array<int, 10> bits = {9, 17, 26, 27, 28, 29, 30, 33, 41, 49};
  // for (int i = 0; i < 8; ++i) {
  //   for (int k = 0; k < 8; ++k) {
  //     int rank = i;
  //     int file = k;
  //     std::vector<int> bits;
  //     for (int f = 1; f < 7; ++f) {
  //       if (f == file) {
  //         continue;
  //       }
  //       bits.push_back(utils::coord(rank, f));
  //     }
  //     for (int r = 1; r < 7; ++r) {
  //       if (r == rank) {
  //         continue;
  //       }
  //       bits.push_back(utils::coord(r, file));
  //     }
  //     int sz = bits.size();
  //     int num_blockers = 1;
  //     for (int r = 0; r < sz; ++r) {
  //       num_blockers *= 2;
  //     }
  //     for (;;) {
  //       magic = magic_generator() & magic_generator();
  //       // std::cout << magic << " magic\n";
  //       std::vector<bool> nums(num_blockers, false);
  //       bool isOk = true;
  //       for (int block = 0; block < num_blockers; ++block) {
  //         Bitboard sq = 0;
  //         int j = block;
  //         int k = 0;
  //         while (j > 0) {
  //           if (j % 2 == 1) {
  //             sq |= (1ULL << bits[k]);
  //           }
  //           k++;
  //           j /= 2;
  //         }
  //         Bitboard num = sq * magic;
  //         int index = num >> (64 - sz);
  //         if (nums[index]) {
  //           isOk = false;
  //           break;
  //         }
  //         nums[index] = true;
  //       }
  //       if (isOk) {
  //         std::cout << "0x" << std::hex << std::uppercase << magic << "ULL, ";
  //         break;
  //       }
  //     }
  //   }
  //   std::cout << '\n';
  // }

  

  // MoveList moves = chess::move_generator::GenerateMoves<MovesType::kPseudo>(pos);
  // std::cout << moves.size() << '\n';
  // std::cout << moves << '\n';

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
