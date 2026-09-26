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
  if (str.length() < 4) {
    return Move();// Incorrect input!!
  }
  int file_from = str[0] - 'a';
  int rank_from = str[1] - '1';
  int file_to = str[2] - 'a';
  int rank_to = str[3] - '1';
  if (file_from < 0 || file_from > 8 || rank_from < 0 || rank_from > 8 ||
      file_to < 0 || file_to > 8 || rank_to < 0 || rank_to > 8) {
    return Move();// Incorrect input!!
  }
  Square from = coord(rank_from, file_from);
  Square to = coord(rank_to, file_to);
  PieceType promoted = PieceType::kNone;
  ColorType side = pos.side_to_move();
  auto& arr = kPromotedPieces[std::to_underlying(side)];
  if (str.length() > 4 && std::ranges::find(arr, str[4]) != arr.end()) {
    promoted = static_cast<PieceType>(std::to_underlying(PieceBase::kKnight & side) +
                                      std::ranges::distance(arr.begin(), std::ranges::find(arr, str[4])));
    std::cout << std::to_underlying(PieceBase::kKnight & side) +
                                      std::ranges::distance(arr.begin(), std::ranges::find(arr, str[4])) << '\n';
    std::cout << static_cast<int>(std::to_underlying(promoted)) << " promoted\n";
  }
  MoveList list;
  pos.GenerateMoves<MovesType::kLegal>(list);
  for (const auto& move: list.AsSpan()) {
    if (move.get_from() == from && move.get_to() == to &&
       (promoted == PieceType::kNone || GetPieceBase(promoted) == move.get_promoted_base())) {
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