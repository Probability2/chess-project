#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>

#include "lib/Perft.hpp"
#include "lib/Search.hpp"
#include "lib/FileManager.hpp"
#include "lib/Move.hpp"
#include "lib/MoveGenerator.hpp"
#include "lib/Uci.hpp"
#include "parser/ArgParser.hpp"
#include "core/commands.hpp"

#include <conio.h>

#include <chrono>
#include <random>

using namespace chess;
using namespace std::chrono_literals;

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
  std::random_device rd;
  std::mt19937_64 magic_generator(rd());

  // auto pos1 = kStartingPosition;
  auto pos2 = fen_manager::Get("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - ").value();
  auto pos3 = fen_manager::Get("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1 ").value();
  auto pos4 = fen_manager::Get("r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1").value();
  auto pos5 = fen_manager::Get("rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8  ").value();
  auto pos6 = fen_manager::Get("r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10 ").value();

  auto pp = fen_manager::Get("1r2k1r1/pbppnp1p/1b3P2/8/Q7/B1PB1q2/P4PPP/3R2K1 w - - 0 1").value();
  auto pp1 = fen_manager::Get("2q1k1nr/Q1Bn1ppp/4p3/5b2/2pP4/P3P3/3N1PPP/b3KB1R w Kk - 0 2").value();
  auto pp2 = fen_manager::Get("4k2r/pp2Bpp1/4p2p/8/4n2Q/6P1/PP1qPP1P/2R3K1 w k - 0 1").value();// carlsen duda
  auto pp3 = fen_manager::Get("8/p2knpp1/1p5p/2p1PP1P/4N3/1PP2K2/P7/8 w - - 0 0").value();// carlsen nakamura
  
  auto pos = fen_manager::Get("rnb1kb1r/1p1n1pp1/p3p2p/4q3/3NN2B/4Q3/2P3PP/3RKB1R w Kkq - 0 16").value();

  // Perftt(pos, 6);

  // MovePicker picker1(pos);
  // MovePicker picker2(pos);

  Searcher searcher(fen_manager::Get("r4r1k/p1pb4/1p1p3p/3Pq2P/2P2p2/3pBN2/PPQ2P1P/6RK w - - 0 32").value());
  // Perftt(pos, 4);
  uci::BotsIterativePlay(pos, 180000ms);
  // uci::PlayWithIterativeBot(fen_manager::Get("r1bqkbnr/pppp1ppp/2n5/1B2p3/4P3/5N2/PPPP1PPP/RNBQK2R b KQkq - 3 3").value(), 5000ms);
  // std::cout << searcher.IterativeBestMove(60000ms) << '\n';
  // uci::BotsIterativePlay(fen_manager::Get("r4r1k/p1pb4/1p1p3p/3Pq2P/2P1pp2/3BB3/PPQN1P1P/6RK w - - 2 31").value(), 15000ms);


  // for (const auto move : list.AsSpan()) {
    // std::cout << std::boolalpha << move << ' ' << pos.IsGoodCapture(move) << '\n';
  // }

  // auto pos = fen_manager::Get("8/8/4kpp1/3p1b2/p6P/2B5/6P1/6K1 b - - 0 1").value();
  // Searcher searcher(pos);
  // std::cout << searcher.IterativeBestMove(16000ms);

  // uci::BotsIterativePlay(kStartingPosition, 60000ms);

  // uci::BotsIterativePlay(fen_manager::Get("r1bqkb1r/ppp1pppp/2np1n2/8/3P4/2N2N2/PPP1PPPP/R1BQKB1R w KQkq - 0 4").value(), 15000ms);
  // searcher.IterativeBestMove(60000ms);
  // PerftPrint(pos2, 6);
  // MovePicker picker(pp3, true);
  // while (auto move = picker.YieldMove()) {
  //   std::cout << move << '\n';
  // }
  
  // PerftPrint(pos5, 5);
  
  // Bitboard magic = 0;
  // for (int rank = 0; rank < 8; ++rank) {
  //   for (int file = 0; file < 8; ++file) {
  //     std::vector<int> bits;
  //     for (int diff = -8; diff < 8; ++diff) {
  //       if (diff == 0 || rank + diff < 1 || rank + diff > 6 || file + diff < 1 || file + diff > 6) {
  //         continue;
  //       }
  //       bits.push_back(utils::coord(rank + diff, file + diff));
  //     }
  //     for (int diff = -8; diff < 8; ++diff) {
  //       if (diff == 0 || rank + diff < 1 || rank + diff > 6 || file - diff < 1 || file - diff > 6) {
  //         continue;
  //       }
  //       bits.push_back(utils::coord(rank + diff, file - diff));
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
