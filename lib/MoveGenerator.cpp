#include "MoveGenerator.hpp"
#include "types/Bitboard.hpp"
#include "types/Attacks.hpp"
#include "types/Piece.hpp"

namespace chess::move_generator {

namespace {

template<ColorType Color>
void GenerateStandardPawnMoves(MoveList& list, const Bitboard pawns, const int shift, const MoveFlag flag) {
  BitLooping(pawns, [&list, shift, flag](const Square ind) {
    list.push(Move(ind - shift, ind, flag));
  });
}

template<ColorType Color>
void GeneratePawnPromotions(MoveList& list, Bitboard pawns, const int shift, const bool is_capture) {
  MoveFlag knight_flag = (is_capture) ? MoveFlag::kKnightPromoCapture : MoveFlag::kKnightPromotion;
  MoveFlag bishop_flag = (is_capture) ? MoveFlag::kBishopPromoCapture : MoveFlag::kBishopPromotion;
  MoveFlag rook_flag = (is_capture) ? MoveFlag::kRookPromoCapture : MoveFlag::kRookPromotion;
  MoveFlag queen_flag = (is_capture) ? MoveFlag::kQueenPromoCapture : MoveFlag::kQueenPromotion;
  BitLooping(pawns, [&](const Square ind) {
    list.push(Move(ind - shift, ind, knight_flag));
    list.push(Move(ind - shift, ind, bishop_flag));
    list.push(Move(ind - shift, ind, rook_flag));
    list.push(Move(ind - shift, ind, queen_flag));
  });
}

template<ColorType Color>
void GeneratePawnQuietMoves(MoveList& list, const Bitboard pawns, const Bitboard empty_squares) {
  constexpr int board_single_shift = (Color == ColorType::kWhite) ? 8 : -8;
  constexpr Bitboard double_rank_mask = (Color == ColorType::kWhite) ? 0x00000000FF000000ULL : 0x000000FF00000000ULL;
  constexpr Bitboard last_rank_mask = (Color == ColorType::kWhite) ? ~kNot8Rank : ~kNot1Rank;
  Bitboard single_shift = ShiftDir(pawns, board_single_shift);
  Bitboard able_to_push = single_shift & empty_squares & ~last_rank_mask;
  GenerateStandardPawnMoves<Color>(list, able_to_push, board_single_shift, MoveFlag::kQuiet);
  GenerateStandardPawnMoves<Color>(list, double_rank_mask & ShiftDir(able_to_push, board_single_shift) & empty_squares,
                                   2 * board_single_shift, MoveFlag::kQuiet);// double pawn pushes
  GeneratePawnPromotions<Color>(list, single_shift & empty_squares & last_rank_mask, board_single_shift, false);
}

template<ColorType Color>
void GeneratePawnCaptures(MoveList& list, const Bitboard pawns, const Position& pos) {
  const Bitboard opponent_pieces = (Color == ColorType::kWhite) ? pos.get_black_pieces() : pos.get_white_pieces();
  constexpr int capture_left_shift = (Color == ColorType::kWhite) ? 7 : -9;
  constexpr int capture_right_shift = (Color == ColorType::kWhite) ? 9 : -7;
  constexpr Bitboard last_rank_mask = (Color == ColorType::kWhite) ? ~kNot8Rank : ~kNot1Rank;
  Bitboard left_attacks = ShiftDir(pawns, capture_left_shift);
  Bitboard right_attacks = ShiftDir(pawns, capture_right_shift);
  GenerateStandardPawnMoves<Color>(list, opponent_pieces & left_attacks & ~last_rank_mask, capture_left_shift,
                                                                                            MoveFlag::kCapture);
  GenerateStandardPawnMoves<Color>(list, opponent_pieces & right_attacks & ~last_rank_mask, capture_right_shift,
                                                                                            MoveFlag::kCapture);
  GeneratePawnPromotions<Color>(list, opponent_pieces & left_attacks & last_rank_mask, capture_left_shift, true);
  GeneratePawnPromotions<Color>(list, opponent_pieces & right_attacks & last_rank_mask, capture_right_shift, true);
  if (pos.is_en_passant()) {
    Bitboard en_passant_mask = ToBB(pos.get_en_passant());
    GenerateStandardPawnMoves<Color>(list, left_attacks & en_passant_mask, capture_left_shift, MoveFlag::kEpCapture);
    GenerateStandardPawnMoves<Color>(list, right_attacks & en_passant_mask, capture_right_shift, MoveFlag::kEpCapture);
  }
}

template<ColorType Color>
void GeneratePawnMoves(MoveList& list, const Position& pos) {
  Bitboard pawns = pos.get_piece_metric(PieceBase::kPawn & Color);
  GeneratePawnQuietMoves<Color>(list, pawns, ~pos.get_all_pieces());
  GeneratePawnCaptures<Color>(list, pawns, pos);
}

inline void AddMoves(const Bitboard mask, const Bitboard opponent_pieces, const Square from, MoveList& list) {
  BitLooping(mask, [opponent_pieces, from, &list](const Square to) {
    list.push(Move(from, to, (opponent_pieces & ToBB(to)) ? MoveFlag::kCapture : MoveFlag::kQuiet));
  });
}

template<ColorType Color, PieceBase Piece>
void GenerateFixedAttackPieces(MoveList& list, const Position& pos) {
  Bitboard opponent_pieces = (Color == ColorType::kWhite) ? pos.get_black_pieces() : pos.get_white_pieces();
  Bitboard own_pieces = (Color == ColorType::kWhite) ? pos.get_white_pieces() : pos.get_black_pieces();
  BitLooping(pos.get_piece_metric(Piece & Color), [opponent_pieces, own_pieces, &list](const Square from) {
    AddMoves(~own_pieces & attacks::kAttacks<Piece>[from], opponent_pieces, from, list);
  });
}

template<ColorType Color>
void GenerateKnightMoves(MoveList& list, const Position& pos) {
  GenerateFixedAttackPieces<Color, PieceBase::kKnight>(list, pos);
}

template<ColorType Color>
void GenerateCastleMoves(MoveList& list, const Position& pos) {
  const Bitboard all_pieces = pos.get_all_pieces();
  constexpr std::array<Bitboard, kMxCastles> kEmptySquareCastles = {
    0x0000000000000060ULL, 0x000000000000000EULL, 0x6000000000000000ULL, 0x0E00000000000000ULL
  };
  constexpr std::array<int, 2> diffs = {2, -2};
  constexpr uint8_t start_pos = (Color == ColorType::kWhite) ? 4 : 60;
  constexpr uint8_t ind = (Color == ColorType::kWhite) ? 3 : 1;
  constexpr std::array<MoveFlag, 2> castle_flags = {MoveFlag::kKingCastle, MoveFlag::kQueenCastle};
  for (uint8_t j = 0; j < 2; ++j) {
    if (((pos.get_castles() >> (ind - j)) & 1) && ((all_pieces & kEmptySquareCastles[3 - ind + j]) == 0)) {
      list.push(Move(start_pos, start_pos + diffs[j], castle_flags[j]));
    }
  }
}

template<ColorType Color>
void GenerateKingMoves(MoveList& list, const Position& pos) {
  GenerateFixedAttackPieces<Color, PieceBase::kKing>(list, pos);
  GenerateCastleMoves<Color>(list, pos);
}

template<ColorType Color, PieceBase Base, PieceType piece_type = Base & Color>
void GenerateSlidingMoves(MoveList& list, const Position& pos) {
  const Bitboard all_pieces = pos.get_all_pieces();
  const Bitboard own_pieces = (Color == ColorType::kWhite) ? pos.get_white_pieces() : pos.get_black_pieces();
  const Bitboard opponent_pieces = all_pieces ^ own_pieces;
  BitLooping(pos.get_piece_metric(piece_type), [own_pieces, opponent_pieces, all_pieces, &list](const Square from) {
    AddMoves(~own_pieces & attacks::SlidingAttacks<Base>(from, all_pieces), opponent_pieces, from, list);
  });
}

template<ColorType Color>
void GenerateBishopMoves(MoveList& list, const Position& pos) {
  GenerateSlidingMoves<Color, PieceBase::kBishop>(list, pos);
}

template<ColorType Color>
void GenerateRookMoves(MoveList& list, const Position& pos) {
  GenerateSlidingMoves<Color, PieceBase::kRook>(list, pos);
}

template<ColorType Color>
void GenerateQueenMoves(MoveList& list, const Position& pos) {
  GenerateSlidingMoves<Color, PieceBase::kRook, PieceBase::kQueen & Color>(list, pos);
  GenerateSlidingMoves<Color, PieceBase::kBishop, PieceBase::kQueen & Color>(list, pos);
}

template<ColorType Color>
void GeneratePseudoMoves(MoveList& list, const Position& pos) {
  GeneratePawnMoves<Color>(list, pos);
  GenerateKnightMoves<Color>(list, pos);
  GenerateBishopMoves<Color>(list, pos);
  GenerateRookMoves<Color>(list, pos);
  GenerateQueenMoves<Color>(list, pos);
  GenerateKingMoves<Color>(list, pos);
}

Bitboard GetAttackerLines(const Position& pos, const Square from) {
  const Bitboard attackers = pos.get_king_attackers();
  Bitboard attacker_lines = 0;
  BitLooping(attackers, [&](const Square sq) {
    if (is_sliding(pos.get_piece(sq))) {
      attacker_lines |= kLines[from][sq];
      attacker_lines ^= ToBB(sq);
    }
  });

  return attacker_lines;
}

template<ColorType Color>
void GenerateKingEvasions(MoveList& list, const Position& pos) {
  const Bitboard own_pieces = (Color == ColorType::kWhite) ? pos.get_white_pieces() : pos.get_black_pieces();
  const Bitboard opponent_pieces = (Color == ColorType::kWhite) ? pos.get_black_pieces() : pos.get_white_pieces();
  const Square from = std::countr_zero(pos.get_piece_metric(PieceBase::kKing & Color));
  AddMoves(~own_pieces & attacks::kAttacks<PieceBase::kKing>[from] & ~GetAttackerLines(pos, from),
                                                                      opponent_pieces, from, list);
}

template<ColorType Color, PieceBase Base>
inline void GenerateSquareEvasions(MoveList& list, const Position& pos,
                                   const Bitboard opponent_pieces, const Square from) {
  const Bitboard attacks = [from]{//iife
    if constexpr (Base == PieceBase::kPawn) {
      return attacks::kAttacks<Base>[!Color, from];
    } else if constexpr (Base == PieceBase::kQueen) {
      return attacks::kAttacks<PieceBase::kRook>[from] | attacks::kAttacks<PieceBase::kBishop>[from];
    } else {
      return attacks::kAttacks<Base>[from];
    }
  }();
  BitLooping(pos.get_piece_metric(Base & Color) & attacks, [&](const Square to) {
    list.push(Move(to, from, (opponent_pieces & ToBB(to)) ? MoveFlag::kCapture : MoveFlag::kQuiet));
  });
}

template<ColorType Color>
void GeneratePieceEvasions(MoveList& list, const Position& pos) {
  const Bitboard opponent_pieces = (Color == ColorType::kWhite) ? pos.get_black_pieces() : pos.get_white_pieces();
  const uint8_t attacker = std::countr_zero(pos.get_king_attackers());
  const uint8_t king_sq = std::countr_zero(pos.get_piece_metric(PieceBase::kKing & Color));
  BitLooping(kBetween[attacker][king_sq] | ToBB(attacker),
            [&list, &pos, opponent_pieces](const uint8_t from) {
    GenerateSquareEvasions<Color, PieceBase::kPawn>(list, pos, opponent_pieces, from);
    GenerateSquareEvasions<Color, PieceBase::kKnight>(list, pos, opponent_pieces, from);
    GenerateSquareEvasions<Color, PieceBase::kBishop>(list, pos, opponent_pieces, from);
    GenerateSquareEvasions<Color, PieceBase::kRook>(list, pos, opponent_pieces, from);
    GenerateSquareEvasions<Color, PieceBase::kQueen>(list, pos, opponent_pieces, from);
  });
}

template<ColorType Color>
void GenerateEvasions(MoveList& list, const Position& pos) {
  GenerateKingEvasions<Color>(list, pos);
  if (pos.is_double_check()) [[unlikely]] {
    return;
  }
  GeneratePieceEvasions<Color>(list, pos);
}

template<ColorType Color>
void GenerateLegalMoves(MoveList& list, const Position& pos) {
  if (pos.is_check()) [[unlikely]] {
    // GenerateEvasions<Color>(list, pos);
    GeneratePseudoMoves<Color>(list, pos);
  }  else [[likely]] {
    GeneratePseudoMoves<Color>(list, pos);
  }
  std::size_t ind = 0;
  while (ind < list.size()) {
    Move move = list[ind];
    if (IsLegal<Color>(pos, move)) {
      ind++;
      continue;
    }
    list[ind] = list.back();
    list.pop_back();
  }
}

}// unnamed namespace

template<MovesType Type, ColorType Color>
MoveList GenerateMoves(const Position& pos) {
  MoveList list;
  if constexpr (Type == MovesType::kPseudo) {
    GeneratePseudoMoves<Color>(list, pos);
  } else if constexpr (Type == MovesType::kEvasions) {
    GenerateEvasions<Color>(list, pos);
  } else {
    GenerateLegalMoves<Color>(list, pos);
  }

  return list;
}

template<MovesType Type>
MoveList GenerateMoves(const Position& pos) {
  if (pos.is_white_move()) {
    return GenerateMoves<Type, ColorType::kWhite>(pos);
  }
  return GenerateMoves<Type, ColorType::kBlack>(pos);
}

template<ColorType Color>
bool IsLegalEnPassant(const Position& pos, const Move& move) {
  return false;
}

template<ColorType Color>
bool IsLegal(const Position& pos, const Move& move) {
  const Square from = move.get_from();
  const Square to = move.get_to();
  const MoveFlag flag = move.get_flag();
  const uint8_t king_sq = std::countr_zero(pos.get_piece_metric(PieceBase::kKing & Color));
  if (pos.get_piece(from) == (PieceBase::kKing & Color)) {
    const Bitboard attackers = pos.GetSquareAttackers(to, from);
    if (attackers != 0) {
      return false;
    }
    if (flag == MoveFlag::kKingCastle || flag == MoveFlag::kQueenCastle) {
      const std::size_t ind = (flag == MoveFlag::kKingCastle) ? 0 : 1;
      return !pos.is_check() && (pos.GetSquareAttackers(kCastleInterSq[ind][std::to_underlying(Color)],
                                                                                     kBoardSize) == 0);
    }
    return true;
  }
  if (pos.is_double_check()) {
    return false;
  }
  const Bitboard pinned_pieces = pos.get_pinned_pieces();
  if (pos.is_single_check()) {
    if (pinned_pieces & ToBB(from)) {
      return false;
    }
    const uint8_t attacker_sq = std::countr_zero(pos.get_king_attackers());
    return (ToBB(to) & (kBetween[king_sq][attacker_sq] | ToBB(attacker_sq))) != 0;
  }
  if ((ToBB(from) & pinned_pieces) == 0) {
    return true;
  }
  if (move.is_en_passant()) [[unlikely]] {
    if (ToBB(from) & pinned_pieces) {
      return false;
    }

    return IsLegalEnPassant<Color>(pos, move);
  }
  return (ToBB(to) & kLines[std::countr_zero(pos.get_piece_metric(PieceBase::kKing & Color))][from]) != 0;
}

// explicit template instantiation
template MoveList GenerateMoves<MovesType::kPseudo>(const Position& pos);
template MoveList GenerateMoves<MovesType::kLegal>(const Position& pos);
template MoveList GenerateMoves<MovesType::kCaptures>(const Position& pos);
template MoveList GenerateMoves<MovesType::kChecks>(const Position& pos);
template MoveList GenerateMoves<MovesType::kEvasions>(const Position& pos);

}// namespace chess::move_generator