#pragma once

#include "types/Bitboard.hpp"
#include "types/Piece.hpp"

#include "Position.hpp"
#include "Move.hpp"

#include <ranges>
#include <string_view>

namespace chess::uci {

namespace internal {

Move UciToMove(std::string_view str, const Position& pos) {
  MoveList list;
  pos.GenerateMoves<MovesType::kLegal>(list);
  for (const auto& move: list.AsSpan()) {
    if (move == str) {
      return move;
    }
  }
  
  return Move(); // The move ain't legal
}

}// namespace internal

void BotsPlay(Position pos, const int depth) {
  Searcher searcher(pos);
  for (;;) {
    Position& spos = searcher.get_position();
    std::cout << spos << '\n';
    MoveList list;
    spos.GenerateMoves<MovesType::kLegal>(list);
    if (list.empty()) {
      if (spos.is_check()) {
        std::cout << "Checkmate!\n";
      } else {
        std::cout << "Stalemate\n";
      }
      break;
    }
    chess::Move move = searcher.GetBestMove(depth);
    std::cout << "Engine's move: " << move << '\n';
    searcher.MakeMove(move);
  }
}

void PlayWithBot(Position pos, const int depth) {
  Searcher searcher(pos);
  ColorType engine_side = !pos.side_to_move();
  ColorType side_to_move = pos.side_to_move();
  for (;;) {
    Position& spos = searcher.get_position();
    if (engine_side == ColorType::kWhite) {
      std::cout << flipped(spos) << '\n';
    } else {
      std::cout << spos << '\n';
    }
    MoveList list;
    spos.GenerateMoves<MovesType::kLegal>(list);
    if (list.empty()) {
      if (spos.is_check()) {
        std::cout << "Checkmate!\n";
      } else {
        std::cout << "Stalemate\n";
      }
      break;
    }
    chess::Move move;
    if (side_to_move == engine_side) {
      move = searcher.GetBestMove(depth);
      std::cout << "Engine's move is: " << move << '\n';
    } else {
      std::cout << "Your move: ";
      while (!move) {
        std::string str;
        if (!std::getline(std::cin, str)) {
          return;
        }
        move = internal::UciToMove(str, spos);
        std::cout << move << '\n';
      }
    }
    searcher.MakeMove(move);
    side_to_move = !side_to_move;
  }
}

void PlayWithIterativeBot(Position pos, const Ms time) {
  Searcher searcher(pos);
  ColorType engine_side = !pos.side_to_move();
  ColorType side_to_move = pos.side_to_move();
  for (;;) {
    Position& spos = searcher.get_position();
    if (engine_side == ColorType::kWhite) {
      std::cout << flipped(spos) << '\n';
    } else {
      std::cout << spos << '\n';
    }
    MoveList list;
    spos.GenerateMoves<MovesType::kLegal>(list);
    if (list.empty()) {
      if (spos.is_check()) {
        std::cout << "Checkmate!\n";
      } else {
        std::cout << "Stalemate\n";
      }
      break;
    }
    chess::Move move;
    if (side_to_move == engine_side) {
      move = searcher.IterativeBestMove(time);
      std::cout << "Engine's move is: " << move << '\n';
    } else {
      std::cout << "Your move: ";
      while (!move) {
        std::string str;
        if (!std::getline(std::cin, str)) {
          return;
        }
        move = internal::UciToMove(str, spos);
        std::cout << move << '\n';
      }
    }
    searcher.MakeMove(move);
    side_to_move = !side_to_move;
  }
}

void BotsIterativePlay(Position pos, const Ms time) {
  Searcher searcher(pos);
  for (;;) {
    Position& spos = searcher.get_position();
    std::cout << spos << '\n';
    MoveList list;
    spos.GenerateMoves<MovesType::kLegal>(list);
    if (list.empty()) {
      if (spos.is_check()) {
        std::cout << "Checkmate!\n";
      } else {
        std::cout << "Stalemate\n";
      }
      break;
    }
    chess::Move move = searcher.IterativeBestMove(time);
    std::cout << "Engine's move: " << move << '\n';
    searcher.MakeMove(move);
  }
}

}