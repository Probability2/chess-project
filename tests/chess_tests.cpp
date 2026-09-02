#include "lib/FileManager.hpp"
#include "lib/Position.hpp"
#include "lib/MoveGenerator.hpp"

#include "gtest/gtest.h"
#include <unordered_set>

namespace chess {

inline constexpr std::string_view perft4 = "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1";

std::string to_notation(const PieceType piece, const Move& move) {
  std::string notation;
  if (piece != PieceType::kWhitePawn && piece != PieceType::kBlackPawn) {
    notation += GetPieceCode(piece);
  }
  notation += get_notation(move.get_from());
  notation += get_notation(move.get_to());
  if (move.has_promoted_piece()) {
    notation += GetPieceCode(move.get_promoted_piece());
  }

  return notation;
}

TEST(PseudoPawnMoves, Position_1) {
  auto pos = fen_manager::Get("rnbqkbnr/ppp1p1pp/8/7P/PP6/3p1p2/2PPPPP1/RNBQKBNR w Qkq - 0 7");
  std::unordered_set<std::string> expected_moves = {"a4a5", "b4b5", "c2c3", "c2c4", "c2d3", "e2d3", "e2e3",
                                                         "e2e4", "e2f3", "g2f3", "g2g3", "g2g4", "h5h6"};
  MoveList moves = move_generator::GenerateMoves<MovesType::kPseudo>(*pos);
  std::unordered_set<std::string> actual_moves;
  for (const auto& move: moves.AsSpan()) {
    if (pos->is_pawn(move.get_from())) {
      actual_moves.insert(to_notation(pos->get_piece(move.get_from()), move));
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
  for (const auto& move: moves.AsSpan()) {
    if (pos->is_pawn(move.get_from())) {
      actual_moves.insert(to_notation(pos->get_piece(move.get_from()), move));
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
  for (const auto& move: moves.AsSpan()) {
    if (pos->is_pawn(move.get_from())) {
      actual_moves.insert(to_notation(pos->get_piece(move.get_from()), move));
    }
  }
  ASSERT_EQ(expected_moves, actual_moves);
}

TEST(PseudoKnightMoves, Position_1) {
  auto pos = fen_manager::Get("rn1qkbnr/p1Pp1ppp/b3P1P1/4p3/3Pp3/5P2/Ppp5/RNBQKBNR b KQkq d3 0 1");
  std::unordered_set<std::string> expected_moves = {"ng8f6", "ng8e7", "ng8h6", "nb8c6"};
  MoveList moves = move_generator::GenerateMoves<MovesType::kPseudo>(*pos);
  std::unordered_set<std::string> actual_moves;
  for (auto& move: moves.AsSpan()) {
    if (pos->is_knight(move.get_from())) {
      actual_moves.insert(to_notation(pos->get_piece(move.get_from()), move));
    }
  }
  ASSERT_EQ(expected_moves, actual_moves);
}

TEST(PseudoKnightMoves, Position_2) {
  auto pos = fen_manager::Get("rnbqkb1N/1ppppppp/p3n3/3N4/NN3N2/3N4/PPPPPPPP/R1BQKB1R w KQq - 0 1");
  std::unordered_set<std::string> expected_moves = {"Na4c5", "Na4b6", "Na4c3", "Nb4c6", "Nb4a6",
                                                    "Nd5e7", "Nd5c7", "Nd5e3", "Nd5c3", "Nd5f6",
                                                    "Nd5b6", "Nf4e6", "Nf4g6", "Nf4h3", "Nf4h5",
                                                    "Nh8g6", "Nh8f7", "Nd3c5", "Nd3e5"};
  MoveList moves = move_generator::GenerateMoves<MovesType::kPseudo>(*pos);
  std::unordered_set<std::string> actual_moves;
  for (const auto& move: moves.AsSpan()) {
    if (pos->is_knight(move.get_from())) {
      actual_moves.insert(to_notation(pos->get_piece(move.get_from()), move));
    }
  }
  ASSERT_EQ(expected_moves, actual_moves);
}

TEST(PseudoKingMoves, Position_1) {
  auto pos = fen_manager::Get("rnbqkbnr/pppppppp/8/8/6P1/1PBP1P1N/P2QP1BP/RN2K2R w KQkq - 0 1");
  std::unordered_set<std::string> expected_moves = {"Ke1d1", "Ke1f1", "Ke1f2", "Ke1g1"};
  MoveList moves = move_generator::GenerateMoves<MovesType::kPseudo>(*pos);
  std::unordered_set<std::string> actual_moves;
  for (const auto& move: moves.AsSpan()) {
    if (pos->is_king(move.get_from())) {
      actual_moves.insert(to_notation(pos->get_piece(move.get_from()), move));
    }
  }
  ASSERT_EQ(expected_moves, actual_moves);
}

TEST(PseudoKingMoves, Position_2) {
  auto pos = fen_manager::Get("rnbqkbnr/pppppppp/8/8/6P1/NPBPPP1N/P1Q3BP/R3K2R w KQkq - 0 1");
  std::unordered_set<std::string> expected_moves = {"Ke1d1", "Ke1f1", "Ke1d2", "Ke1e2", "Ke1f2", "Ke1g1", "Ke1c1"};
  MoveList moves = move_generator::GenerateMoves<MovesType::kPseudo>(*pos);
  std::unordered_set<std::string> actual_moves;
  for (const auto& move: moves.AsSpan()) {
    if (pos->is_king(move.get_from())) {
      actual_moves.insert(to_notation(pos->get_piece(move.get_from()), move));
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
  for (const auto& move: moves.AsSpan()) {
    if (pos->is_rook(move.get_from())) {
      actual_moves.insert(to_notation(pos->get_piece(move.get_from()), move));
    }
  }
  ASSERT_EQ(expected_moves, actual_moves);
}

TEST(PseudoBishopMoves, Position_1) {
  auto pos = fen_manager::Get("rnbqkb1r/pp3ppp/2p2n2/3pp1B1/2B1P3/3P4/PPP2PPP/RN1QK1NR w KQkq - 0 5");
  std::unordered_set<std::string> expected_moves = {"Bc4a6", "Bc4b5", "Bc4d5", "Bc4b3",
                                                    "Bg5f6", "Bg5h6", "Bg5h4", "Bg5f4", "Bg5e3",
                                                    "Bg5d2", "Bg5c1"};
  MoveList moves = move_generator::GenerateMoves<MovesType::kPseudo>(*pos);
  std::unordered_set<std::string> actual_moves;
  for (const auto& move: moves.AsSpan()) {
    if (pos->is_bishop(move.get_from())) {
      actual_moves.insert(to_notation(pos->get_piece(move.get_from()), move));
    }
  }
  ASSERT_EQ(expected_moves, actual_moves);
}

TEST(PseudoQueenMoves, Position_1) {
  auto pos = fen_manager::Get("rnb1kbnr/p1p4p/1p1p4/2q2Pp1/1p1Q1P2/2p5/PPPPP1PP/RNB1KBNR b - - 0 1");
  std::unordered_set<std::string> expected_moves = {"qc5a5", "qc5b5", "qc5c6", "qc5c4",
                                                    "qc5d4", "qc5d5", "qc5e5", "qc5f5"};
  MoveList moves = move_generator::GenerateMoves<MovesType::kPseudo>(*pos);
  std::unordered_set<std::string> actual_moves;
  for (const auto& move: moves.AsSpan()) {
    if (pos->is_queen(move.get_from())) {
      actual_moves.insert(to_notation(pos->get_piece(move.get_from()), move));
    }
  }
  ASSERT_EQ(expected_moves, actual_moves);
}

TEST(PseudoQueenMoves, Position_2) {
  auto pos = fen_manager::Get("rnb1kbnr/p1p4p/1p1p4/2q2Pp1/1p1Q1P2/2p5/PPPPP1PP/RNB1KBNR w - - 0 1");
  std::unordered_set<std::string> expected_moves = {"Qd4c5", "Qd4c4", "Qd4c3", "Qd4d3",
                                                    "Qd4e3", "Qd4f2", "Qd4e4", "Qd4d5",
                                                    "Qd4d6", "Qd4e5", "Qd4b4", "Qd4f6",
                                                    "Qd4g7", "Qd4h8"};
  MoveList moves = move_generator::GenerateMoves<MovesType::kPseudo>(*pos);
  std::unordered_set<std::string> actual_moves;
  for (const auto& move: moves.AsSpan()) {
    if (pos->is_queen(move.get_from())) {
      actual_moves.insert(to_notation(pos->get_piece(move.get_from()), move));
    }
  }
  ASSERT_EQ(expected_moves, actual_moves);
}

TEST(LegalMoves, Position_1) {
  auto pos = fen_manager::Get("4rk2/3r3b/8/3B2b1/1b3P2/2Q5/rR1K1Nr1/8 w - - 0 1");
  std::unordered_set<std::string> expected_moves = {"Kd2d1", "Kd2c1", "Qc3b4", "Rb2c2", "Rb2a2", "f4g5"};
  MoveList moves = pos->GenerateMoves<MovesType::kLegal>();
  std::unordered_set<std::string> actual_moves;
  for (const auto& move: moves.AsSpan()) {
    actual_moves.insert(to_notation(pos->get_piece(move.get_from()), move));
  }
  ASSERT_EQ(expected_moves, actual_moves);
}

TEST(LegalMoves, Position_2) {
  auto pos = fen_manager::Get("2krr3/pppb1pp1/2n2n1p/b2p1P1q/4B2b/1NBQ2N1/PPP4P/R3K2R w KQ - 0 1");
  std::unordered_set<std::string> expected_moves = {"Ke1d2", "Ke1f2", "Ke1f1", "Ke1g1", "Rh1g1", "Rh1f1",
                                                    "h2h3", "Ra1b1", "Ra1c1", "Ra1d1", "a2a3", "a2a4",
                                                    "Bc3b4", "Bc3a5", "Bc3d2", "Qd3e3", "Qd3f3", "Qd3e2",
                                                    "Qd3f1", "Qd3d2", "Qd3d1", "Qd3d4", "Qd3d5", "Qd3c4",
                                                    "Qd3b5", "Qd3a6", "Nb3a5", "Nb3c5", "Nb3d4", "Nb3c1", "Nb3d2"};
  MoveList moves = pos->GenerateMoves<MovesType::kLegal>();
  std::unordered_set<std::string> actual_moves;
  for (const auto& move: moves.AsSpan()) {
    actual_moves.insert(to_notation(pos->get_piece(move.get_from()), move));
  }
  ASSERT_EQ(expected_moves, actual_moves);
}

TEST(LegalMoves, DoubleCheck) {
  auto pos = fen_manager::Get("q6k/8/4K2N/6N1/2B2n2/b5P1/5Q2/4r3 w - - 0 1");
  std::unordered_set<std::string> expected_moves = {"Ke6d7", "Ke6f7", "Ke6f6", "Ke6f5"};
  MoveList moves = pos->GenerateMoves<MovesType::kLegal>();
  std::unordered_set<std::string> actual_moves;
  for (const auto& move: moves.AsSpan()) {
    actual_moves.insert(to_notation(pos->get_piece(move.get_from()), move));
  }
  ASSERT_EQ(expected_moves, actual_moves);
}

TEST(LegalMoves, SingleCheck_1) {
  auto pos = fen_manager::Get("q6k/8/4K2N/5nN1/2B5/b5Pb/5Q2/4r3 w - - 0 1");
  std::unordered_set<std::string> expected_moves = {"Ke6d7", "Ke6f6", "Ke6f7", "Bc4e2", "Qf2e1", "Qf2e2", "Qf2e3",
                                                    "Ng5e4"};
  MoveList moves = pos->GenerateMoves<MovesType::kLegal>();
  std::unordered_set<std::string> actual_moves;
  for (const auto& move: moves.AsSpan()) {
    actual_moves.insert(to_notation(pos->get_piece(move.get_from()), move));
  }
  ASSERT_EQ(expected_moves, actual_moves);
}

TEST(LegalMoves, SingleCheck_2) {
  auto pos = fen_manager::Get("q6k/8/4K2N/5nN1/2B5/b5Pb/b4Q2/4r3 w - - 0 1");
  std::unordered_set<std::string> expected_moves = {"Ke6d7", "Ke6f6", "Ke6f7", "Qf2e1", "Qf2e2", "Qf2e3",
                                                    "Ng5e4"};
  MoveList moves = pos->GenerateMoves<MovesType::kLegal>();
  std::unordered_set<std::string> actual_moves;
  for (const auto& move: moves.AsSpan()) {
    actual_moves.insert(to_notation(pos->get_piece(move.get_from()), move));
  }
  ASSERT_EQ(expected_moves, actual_moves);
}

TEST(LegalMoves, SingleCheck_3) {
  auto pos = fen_manager::Get("3k4/8/8/4Q3/8/8/8/rK6 w - - 0 1");
  std::unordered_set<std::string> expected_moves = {"Kb1a1", "Kb1b2", "Kb1c2", "Qe5a1"};
  MoveList moves = pos->GenerateMoves<MovesType::kLegal>();
  std::unordered_set<std::string> actual_moves;
  for (const auto& move: moves.AsSpan()) {
    actual_moves.insert(to_notation(pos->get_piece(move.get_from()), move));
  }
  ASSERT_EQ(expected_moves, actual_moves);
}

TEST(LegalMoves, Castle_1) {
  auto pos = fen_manager::Get("rnbqk1r1/pppppp1p/5p2/5P2/2BNP2b/8/PPPP2PP/RNBQK2R w KQq - 0 1");
  std::unordered_set<std::string> expected_moves = {"g2g3", "Ke1e2", "Ke1f1"};
  MoveList moves = pos->GenerateMoves<MovesType::kLegal>();
  std::unordered_set<std::string> actual_moves;
  for (const auto& move: moves.AsSpan()) {
    actual_moves.insert(to_notation(pos->get_piece(move.get_from()), move));
  }
  ASSERT_EQ(expected_moves, actual_moves);
}

TEST(LegalMoves, Castle_2) {
  auto pos = fen_manager::Get("rnbqk3/pppppp1p/5p1b/5P2/2BNPr2/8/PPPP2PP/RNBQK2R w KQq - 0 1");
  std::unordered_set<std::string> expected_moves = {"g2g3", "g2g4", "h2h3", "h2h4", "e4e5", "a2a3", "a2a4",
                                                    "b2b3", "b2b4", "c2c3", "d2d3", "Nd4e6", "Nd4f3", "Nd4e2",
                                                    "Nd4b3", "Nd4b5", "Nd4c6", "Bc4a6", "Bc4b5", "Bc4b3", "Bc4d5",
                                                    "Bc4e6", "Bc4f7", "Bc4d3", "Bc4e2", "Bc4f1", "Rh1g1", "Rh1f1",
                                                    "Ke1e2", "Qd1e2", "Qd1f3", "Qd1g4", "Qd1h5", "Nb1c3", "Nb1a3"};
  MoveList moves = pos->GenerateMoves<MovesType::kLegal>();
  std::unordered_set<std::string> actual_moves;
  for (const auto& move: moves.AsSpan()) {
    actual_moves.insert(to_notation(pos->get_piece(move.get_from()), move));
  }
  ASSERT_EQ(expected_moves, actual_moves);
}

TEST(LegalMoves, Castle_3) {
  auto pos = fen_manager::Get("rnbqk3/pppppp1p/5p1b/5Pr1/2BNP3/8/PPPP2PP/RNBQK2R w KQq - 0 1");
  std::unordered_set<std::string> expected_moves = {"g2g3", "g2g4", "h2h3", "h2h4", "e4e5", "a2a3", "a2a4",
                                                    "b2b3", "b2b4", "c2c3", "d2d3", "Nd4e6", "Nd4f3", "Nd4e2",
                                                    "Nd4b3", "Nd4b5", "Nd4c6", "Bc4a6", "Bc4b5", "Bc4b3", "Bc4d5",
                                                    "Bc4e6", "Bc4f7", "Bc4d3", "Bc4e2", "Bc4f1", "Rh1g1", "Rh1f1",
                                                    "Ke1e2", "Qd1e2", "Qd1f3", "Qd1g4", "Qd1h5", "Nb1c3", "Nb1a3",
                                                    "Ke1g1", "Ke1f1", "Ke1f2"};
  MoveList moves = pos->GenerateMoves<MovesType::kLegal>();
  std::unordered_set<std::string> actual_moves;
  for (const auto& move: moves.AsSpan()) {
    actual_moves.insert(to_notation(pos->get_piece(move.get_from()), move));
  }
  ASSERT_EQ(expected_moves, actual_moves);
}

}