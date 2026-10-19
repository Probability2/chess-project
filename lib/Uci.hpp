#pragma once

#include "types/Bitboard.hpp"
#include "types/Piece.hpp"

#include "Perft.hpp"
#include "Position.hpp"
#include "Move.hpp"

#include <charconv>
#include <chrono>
#include <iostream>
#include <ranges>
#include <string_view>

namespace chess::uci {

enum class ParsePositionError: uint8_t {
  kIllegalPosition,
  kMissingPosition,
  kIllegalArgument,
  kMissingMoves,
  kIllegalMove
};

enum class GoParseError: uint8_t {
  kMissingCommand,
  kIllegalNumber,
  kPositionNotSet,
  kIllegalArgument
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

std::string_view to_string(const GoParseError error) {
  switch (error) {
    case GoParseError::kIllegalArgument: return "The unknown argument was found";
    case GoParseError::kIllegalNumber: return "The entered number isn't valid";
    case GoParseError::kPositionNotSet: return "The position in the searcher hasn't been set";
    case GoParseError::kMissingCommand: return "The command wasn't entered";
  }
  std::unreachable();
}

constexpr std::string_view kFenStr = "fen";
constexpr std::string_view kStartPosStr = "startpos";
constexpr std::string_view kMovesStr = "moves";
constexpr std::string_view kMoveTimeStr = "movetime";
constexpr std::string_view kNodesStr = "nodes";
constexpr std::string_view kDepthStr = "depth";
constexpr std::string_view kInfStr = "infinite";
constexpr std::string_view kPerftStr = "perft";

namespace internal {

void SkipSpaces(std::string_view& str) {
  str.remove_prefix(std::min(str.find_first_not_of(' '), str.size()));
}

std::expected<Position, ParsePositionError> ParsePosition(std::string_view& request) {
  std::string_view arg = request.substr(0, request.find(' '));
  if (arg == kStartPosStr) {
    request.remove_prefix(kStartPosStr.size());
    return kStartingPosition;
  } else if (arg != kFenStr) {
    return std::unexpected(ParsePositionError::kIllegalArgument);
  }
  request.remove_prefix(kFenStr.size());
  internal::SkipSpaces(request);
  if (request.empty()) {
    return std::unexpected(ParsePositionError::kMissingPosition);
  }
  return fen_manager::Parse(request).transform_error([]([[maybe_unused]] auto)
                             { return ParsePositionError::kIllegalPosition; });
}

Move UciToMove(std::string_view str, const Position& pos) {
  MoveList list;
  move_generator::GenerateMoves<MovesType::kLegal>(pos, list);
  auto moves = list.AsSpan();
  auto move = std::ranges::find(moves, str);
  if (move != moves.end()) {
    return *move;
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

std::expected<std::size_t, GoParseError> ParseNumber(std::string_view& request, std::string_view arg) {
  request.remove_prefix(arg.size());
  internal::SkipSpaces(request);
  std::size_t res = 0;
  auto [_, ec] = std::from_chars(request.data(), request.data() + request.size(), res);
  if (ec != std::errc()) {
    return std::unexpected(GoParseError::kIllegalNumber);
  }

  return res;
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

void PlayWithIterativeBot(Position pos, const Ms move_time) {
  Searcher searcher(pos);
  ColorType engine_side = pos.side_to_move();
  ColorType side_to_move = pos.side_to_move();
  searcher.set_limits({.move_time_ = move_time});
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
      move = searcher.IterativeBestMove();
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

void BotsIterativePlay(Position pos, const Ms move_time) {
  Searcher searcher(pos);
  searcher.set_limits({.move_time_ = move_time});
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
    auto move = searcher.IterativeBestMove();
    std::cout << "Engine's move: " << move << '\n';
    searcher.MakeMove(move);
  }
}

void ProcessUci() {
  std::cout << "uciok" << std::endl;
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
  auto pos = internal::ParsePosition(request);
  if (!pos) {
    return std::unexpected(pos.error());
  }
  internal::SkipSpaces(request);
  if (!request.empty()) {
    if (!request.starts_with(kMovesStr)) {
      return std::unexpected(ParsePositionError::kIllegalArgument);
    }
    request.remove_prefix(kMovesStr.size());
    internal::SkipSpaces(request);
    if (request.empty()) {
      return std::unexpected(ParsePositionError::kMissingMoves);
    }
    if (auto res = internal::ProcessUciMoves(request, pos); !res) {
      return res;
    }
  }
  searcher.SetPosition(*pos);

  return {};
}

std::expected<void, GoParseError> ProcessGo(Searcher& searcher, std::string_view request) {
  request.remove_prefix(3);
  internal::SkipSpaces(request);
  if (request.empty()) {
    return std::unexpected(GoParseError::kMissingCommand);
  }
  std::expected<Move, GoParseError> best_move = std::unexpected(GoParseError::kIllegalArgument);
  if (request.starts_with(kMoveTimeStr)) {
    best_move = internal::ParseNumber(request, kMoveTimeStr).transform([&searcher](const auto move_time) {
      if (move_time == 0) {
        return std::unexpected(GoParseError::kIllegalArgument);
      }
      searcher.set_limits({.move_time_ = std::chrono::milliseconds(move_time)});
      return searcher.IterativeBestMove();
    });
  } else if (request.starts_with(kNodesStr)) {
    best_move = internal::ParseNumber(request, kNodesStr).transform([&searcher](const auto nodes) {
      searcher.set_limits({.move_time_ = Ms::zero(), .depth_ = kMaxDepth, .mx_nodes_ = nodes});
      return searcher.IterativeBestMove();
    });
  } else if (request.starts_with(kDepthStr)) {
    best_move = internal::ParseNumber(request, kDepthStr).transform([&searcher](const auto depth) {
      searcher.set_limits({.move_time_ = Ms::zero(), .depth_ = depth, .mx_nodes_ = kMxNodes});
      return searcher.IterativeBestMove();
    });
  } else if (request.starts_with(kInfStr)) {
    searcher.set_limits({.depth_ = kMaxDepth});
    best_move = searcher.IterativeBestMove();
  } else if (request.starts_with(kPerftStr)) {
    return internal::ParseNumber(request, kPerftStr).transform([&searcher](const auto depth) {
      PrintPerft(searcher.get_position(), depth);
    });
  }
  if (!best_move) {
    return std::unexpected(best_move.error());
  }
  std::cout << "bestmove " << *best_move << std::endl;

  return {};
}

void Run() {
  Searcher searcher;
  std::string request;
  for (;;) {
    if (!std::getline(std::cin, request)) {
      break;
    }
    if (request == "uci") {
      ProcessUci();
    } else if (request == "quit") {
      break;
    } else if (request == "isready") {
      ProcessIsReady();
    } else if (request == "ucinewgame") {
      searcher.clear();
    } else if (request == "d") {
      std::cout << searcher.get_position() << std::endl;
    } else if (request == "help") {
      std::cout << "See: https://github.com/Probability2/chess-project" << std::endl;
    } else if (request.starts_with("position")) {
      if (auto res = ProcessPosition(searcher, request); !res) {
        std::cerr << to_string(res.error()) << std::endl;
      }
    } else if (request.starts_with("go")) {
      if (auto res = ProcessGo(searcher, request); !res) {
        std::cerr << to_string(res.error()) << std::endl;
      }
    } else if (request.starts_with("eval")) {
      std::cout << eval::Evaluate(searcher.get_position());
    }
  }
}

}