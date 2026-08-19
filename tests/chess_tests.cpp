#include "lib/FileManager.hpp"
#include "lib/Position.hpp"
#include "lib/MoveGenerator.hpp"

#include "gtest/gtest.h"
#include <unordered_set>

namespace chess {

inline constexpr std::string_view perft4 = "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1";

std::string to_notation(const Move& move) {
  std::string notation;
  if (!move.is_pawn()) {
    notation += GetPieceCode(move.piece_);
  }
  notation += utils::get_notation(move.from_);
  notation += utils::get_notation(move.to_);
  if (move.has_promoted_piece()) {
    notation += GetPieceCode(move.promoted_piece_);
  }

  return notation;
}

TEST(PseudoPawnMoves, Position_1) {
  auto pos = fen_manager::Get("rnbqkbnr/ppp1p1pp/8/7P/PP6/3p1p2/2PPPPP1/RNBQKBNR w Qkq - 0 7");
  std::unordered_set<std::string> expected_moves = {"a4a5", "b4b5", "c2c3", "c2c4", "c2d3", "e2d3", "e2e3",
                                                         "e2e4", "e2f3", "g2f3", "g2g3", "g2g4", "h5h6"};
  MoveList moves = move_generator::GenerateMoves<MovesType::kPseudo>(*pos);
  std::unordered_set<std::string> actual_moves;
  for (auto& move: moves.AsSpan()) {
    if (move.is_pawn()) {
      actual_moves.insert(to_notation(move));
    }
  }
  ASSERT_EQ(expected_moves, actual_moves);
}

TEST(PseudoPawnMoves, Position_2) {
  auto pos = fen_manager::Get(perft4);
  std::unordered_set<std::string> expected_moves = {"c4c5", "d2d3", "d2d4", "e4e5",
                                                    "g2g3", "g2g4", "h2h3", "h2h4"};
  MoveList moves = move_generator::GenerateMoves<MovesType::kPseudo>(*pos);
  std::unordered_set<std::string> actual_moves;
  for (auto& move: moves.AsSpan()) {
    if (move.is_pawn()) {
      actual_moves.insert(to_notation(move));
    }
  }
  ASSERT_EQ(expected_moves, actual_moves);
}

TEST(PseudoPawnMoves, Position_3) {
  auto pos = fen_manager::Get("rnbqkbnr/2p1ppp1/1p5p/p1PpP3/8/8/PP1P1PPP/RNBQKBNR w KQkq d6 0 5");
  std::unordered_set<std::string> expected_moves = {"a2a3", "a2a4", "b2b3", "b2b4", "d2d3", "d2d4",
                                                    "c5b6", "c5c6", "c5d6", "e5d6", "e5e6", "f2f4",
                                                    "f2f3", "g2g3", "g2g4", "h2h3", "h2h4"};
  MoveList moves = move_generator::GenerateMoves<MovesType::kPseudo>(*pos);
  std::unordered_set<std::string> actual_moves;
  for (auto& move: moves.AsSpan()) {
    if (move.is_pawn()) {
      actual_moves.insert(to_notation(move));
    }
  }
  ASSERT_EQ(expected_moves, actual_moves);
}

TEST(PseudoPawnMoves, Position_4) {
  auto pos = fen_manager::Get("rn1qkbnr/p1Pp1ppp/b3P1P1/4p3/3Pp3/5P2/Ppp5/RNBQKBNR b KQkq d3 0 1");
  std::unordered_set<std::string> expected_moves = {"ng8f6", "ng8e7", "ng8h6", "nb8c6"};
  MoveList moves = move_generator::GenerateMoves<MovesType::kPseudo>(*pos);
  std::unordered_set<std::string> actual_moves;
  for (auto& move: moves.AsSpan()) {
    if (move.is_knight()) {
      actual_moves.insert(to_notation(move));
    }
  }
  ASSERT_EQ(expected_moves, actual_moves);
}

TEST(PseudoKnightMoves, Position_1) {
  auto pos = fen_manager::Get("rnbqkb1N/1ppppppp/p3n3/3N4/NN3N2/3N4/PPPPPPPP/R1BQKB1R w KQq - 0 1");
  std::unordered_set<std::string> expected_moves = {"Na4c5", "Na4b6", "Na4c3", "Nb4c6", "Nb4a6",
                                                    "Nd5e7", "Nd5c7", "Nd5e3", "Nd5c3", "Nd5f6",
                                                    "Nd5b6", "Nf4e6", "Nf4g6", "Nf4h3", "Nf4h5",
                                                    "Nh8g6", "Nh8f7", "Nd3c5", "Nd3e5"};
  MoveList moves = move_generator::GenerateMoves<MovesType::kPseudo>(*pos);
  std::unordered_set<std::string> actual_moves;
  for (auto& move: moves.AsSpan()) {
    if (move.is_knight()) {
      actual_moves.insert(to_notation(move));
    }
  }
  ASSERT_EQ(expected_moves, actual_moves);
}

TEST(PseudoKingMoves, Position_1) {
  auto pos = fen_manager::Get("rnbqkbnr/pppppppp/8/8/6P1/1PBP1P1N/P2QP1BP/RN2K2R w KQkq - 0 1");
  std::unordered_set<std::string> expected_moves = {"Ke1d1", "Ke1f1", "Ke1f2", "Ke1g1"};
  MoveList moves = move_generator::GenerateMoves<MovesType::kPseudo>(*pos);
  std::unordered_set<std::string> actual_moves;
  for (auto& move: moves.AsSpan()) {
    if (move.is_king()) {
      actual_moves.insert(to_notation(move));
    }
  }
  ASSERT_EQ(expected_moves, actual_moves);
}

TEST(PseudoKingMoves, Position_2) {
  auto pos = fen_manager::Get("rnbqkbnr/pppppppp/8/8/6P1/NPBPPP1N/P1Q3BP/R3K2R w KQkq - 0 1");
  std::unordered_set<std::string> expected_moves = {"Ke1d1", "Ke1f1", "Ke1d2", "Ke1e2", "Ke1f2", "Ke1g1", "Ke1c1"};
  MoveList moves = move_generator::GenerateMoves<MovesType::kPseudo>(*pos);
  std::unordered_set<std::string> actual_moves;
  for (auto& move: moves.AsSpan()) {
    if (move.is_king()) {
      actual_moves.insert(to_notation(move));
    }
  }
  ASSERT_EQ(expected_moves, actual_moves);
}

TEST(PseudoRookMoves, Position_1) {
  auto pos = fen_manager::Get("RnbqkbRr/Pppppppp/8/7P/3R3R/P6P/1PPPPPPP/RNBQKBNR w KQk - 0 1");
  std::unordered_set<std::string> expected_moves = {"Ra8b8", "Ra1a2", "Rg8g7", "Rg8h8",
                                                    "Rg8f8", "Rd4d3", "Rd4d5", "Rd4d6", "Rd4d7",
                                                    "Rd4c4", "Rd4b4", "Rd4a4", "Rd4e4", "Rd4f4",
                                                    "Rd4g4", "Rh4g4", "Rh4f4", "Rh4e4"};
  MoveList moves = move_generator::GenerateMoves<MovesType::kPseudo>(*pos);
  std::unordered_set<std::string> actual_moves;
  for (auto& move: moves.AsSpan()) {
    if (move.is_rook()) {
      actual_moves.insert(to_notation(move));
    }
  }
  ASSERT_EQ(expected_moves, actual_moves);
}

}