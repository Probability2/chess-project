#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>

#include "lib/Perft.hpp"
#include "lib/Search.hpp"
#include "lib/FileManager.hpp"
#include "lib/Move.hpp"
#include "lib/MoveGenerator.hpp"
#include "lib/Uci.hpp"

#include <conio.h>

#include <chrono>
#include <random>

using namespace chess;
using namespace std::chrono_literals;

int main(int argc, char** argv) {
  #ifdef _WIN32
  SetConsoleOutputCP(CP_UTF8);
  SetConsoleCP(CP_UTF8);
  #endif

  std::random_device rd;
  std::mt19937_64 magic_generator(rd());

  auto pos1 = kStartingPosition;
  auto pos2 = fen_manager::Get("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - ").value();
  auto pos3 = fen_manager::Get("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1 ").value();
  auto pos4 = fen_manager::Get("r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1").value();
  auto pos5 = fen_manager::Get("rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8  ").value();
  auto pos6 = fen_manager::Get("r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10 ").value();

  auto pp = fen_manager::Get("1r2k1r1/pbppnp1p/1b3P2/8/Q7/B1PB1q2/P4PPP/3R2K1 w - - 0 1").value();
  auto pp1 = fen_manager::Get("2q1k1nr/Q1Bn1ppp/4p3/5b2/2pP4/P3P3/3N1PPP/b3KB1R w Kk - 0 2").value();
  auto pp2 = fen_manager::Get("4k2r/pp2Bpp1/4p2p/8/4n2Q/6P1/PP1qPP1P/2R3K1 w k - 0 1").value();// carlsen duda
  auto pp3 = fen_manager::Get("8/p2knpp1/1p5p/2p1PP1P/4N3/1PP2K2/P7/8 w - - 0 0").value();// carlsen nakamura
  
  auto pos = fen_manager::Get("r5k1/1R2pp1p/3p2p1/3P4/b3P3/1p2B2P/1P1K1PP1/8 w - - 0 1").value();

  // uci::PlayWithIterativeBot(fen_manager::Get("r5k1/1R2pp1p/3p2p1/3P4/b3P3/1p2B2P/1P1K1PP1/8 b - - 0 1").value(), 1000ms);

  uci::Run();

  return EXIT_SUCCESS;
}
