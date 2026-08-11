#include "lib/FileManager.hpp"
#include "lib/Position.hpp"
#include "gtest/gtest.h"

namespace chess {

TEST(PseudoMovesSuite, Pawns) {
  auto pos = fen_manager::Get("rnbqkbnr/ppp1p1pp/8/7P/PP6/3p1p2/2PPPPP1/RNBQKBNR w Qkq - 0 7");

  MoveList moves;
  Move move1(PieceType::kWhitePawn, 12, 20);
  Move move2(PieceType::kWhitePawn, 12, 28);
  Move move3(PieceType::kWhitePawn, 12, 19);
  Move move4(PieceType::kWhitePawn, 12, 21);
  pos->AddPseudoPawnMoves(moves, 12);
  ASSERT_EQ(moves.size(), 4);
  ASSERT_EQ(moves.contains(move1), true);
  ASSERT_EQ(moves.contains(move2), true);
  ASSERT_EQ(moves.contains(move3), true);
  ASSERT_EQ(moves.contains(move4), true);

  ASSERT_EQ("hello", "hello");
}

}