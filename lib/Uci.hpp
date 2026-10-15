#pragma once

#include "types/Bitboard.hpp"
#include "types/Piece.hpp"

#include "Position.hpp"
#include "Move.hpp"

#include <ranges>
#include <string_view>

namespace chess::uci {

enum class ParsePositionError {
  kIllegalPosition,
  kMissingPosition,
  kIllegalArgument,
  kMissingMoves,
  kIllegalMove
};

std::string_view to_string(const ParsePositionError error) {
  switch (error) {
    case ParsePositionError::kIllegalPosition: return "The entered position is illegal";
    case ParsePositionError::kMissingPosition: return "The position wasn't entered";
    case ParsePositionError::kIllegalArgument: return "The unknown argument was found";
    case ParsePositionError::kMissingMoves: return "No moves were entered";
    case ParsePositionError::kIllegalMove: return "The entered moves weren't correct";
  }
  std::unreachable();
}

constexpr std::string_view kFenStr = "fen";

constexpr std::string_view kStartPosStr = "startpos";

constexpr std::string_view kMovesStr = "moves";

namespace internal {

void SkipSpaces(std::string_view& str) {
  str.remove_prefix(std::min(str.find_first_not_of(' '), str.size()));
}

Move UciToMove(std::string_view str, const Position& pos) {
  MoveList list;
  move_generator::GenerateMoves<MovesType::kLegal>(pos, list);
  for (const auto& move: list.AsSpan()) {
    if (move == str) {
      return move;
    }
  }
  
  return Move(); // The move ain't legal
}

std::expected<void, ParsePositionError> ProcessUciMoves(std::string_view request, auto& pos) {
  for (;;) {
    SkipSpaces(request);
    if (request.empty()) {
      break;
    }
    std::size_t move_sz = std::min(request.find(' '), request.size());
    std::string_view move_str = request.substr(0, move_sz);
    request.remove_prefix(move_sz);
    Move move = internal::UciToMove(move_str, *pos);
    if (!move) {
      return std::unexpected(ParsePositionError::kIllegalMove);
    }
    pos->MakeMove(move);
  }

  return {};
}

}// namespace internal

void BotsPlay(Position pos, const int depth) {
  Searcher searcher(pos);
  for (;;) {
    Position& spos = searcher.get_position();
    std::cout << spos << '\n';
    MoveList list;
    move_generator::GenerateMoves<MovesType::kLegal>(spos, list);
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
    move_generator::GenerateMoves<MovesType::kLegal>(spos, list);
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
  ColorType engine_side = pos.side_to_move();
  ColorType side_to_move = pos.side_to_move();
  for (;;) {
    Position& spos = searcher.get_position();
    if (engine_side == ColorType::kWhite) {
      std::cout << spos << '\n';
    } else {
      std::cout << spos << '\n';
    }
    MoveList list;
    move_generator::GenerateMoves<MovesType::kLegal>(spos, list);
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
    move_generator::GenerateMoves<MovesType::kLegal>(spos, list);
    if (list.empty()) {
      if (spos.is_check()) {
        std::cout << "Checkmate!\n";
      } else {
        std::cout << "Stalemate\n";
      }
      break;
    }
    auto move = searcher.IterativeBestMove(time);
    std::cout << "Engine's move: " << move << '\n';
    searcher.MakeMove(move);
  }
}

void ProcessUci() {
  std::cout << "id name Tapuzero" << std::endl;
  std::cout << "id author Mikhail Balakirskii" << std::endl;
}

void ProcessIsReady() {
  std::cout << "readyok" << std::endl;
}

std::expected<void, ParsePositionError> ProcessPosition(Searcher& searcher, std::string_view request) {
  request.remove_prefix(8);// position
  if (request.empty()) {
    return std::unexpected(ParsePositionError::kMissingPosition);
  }
  internal::SkipSpaces(request);
  std::string_view arg = request.substr(0, request.find(' '));
  if (arg == kStartPosStr) {
    searcher.SetPosition(kStartingPosition);
    return {};
  } else if (arg != kFenStr) {
    return std::unexpected(ParsePositionError::kIllegalArgument);
  }
  request.remove_prefix(3);
  if (request.empty()) {
    return std::unexpected(ParsePositionError::kMissingPosition);
  }
  internal::SkipSpaces(request);
  auto pos = fen_manager::Parse(request);
  if (!pos) {
    return std::unexpected(ParsePositionError::kIllegalPosition);
  }
  if (request.empty()) {
    searcher.SetPosition(*pos);
    return {};
  }
  internal::SkipSpaces(request);
  if (!request.starts_with(kMovesStr)) {
    return std::unexpected(ParsePositionError::kIllegalArgument);
  }
  request.remove_prefix(kMovesStr.size());
  if (request.empty()) {
    return std::unexpected(ParsePositionError::kMissingMoves);
  }
  if (auto res = internal::ProcessUciMoves(request, pos); !res) {
    return res;
  }
  searcher.SetPosition(*pos);

  return {};
}

}