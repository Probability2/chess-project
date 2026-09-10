#include "MoveGenerator.hpp"
#include "types/Bitboard.hpp"
#include "types/Attacks.hpp"
#include "types/Piece.hpp"

namespace chess::move_generator {

namespace {

inline void GenerateStandardPawnMoves(MoveList& list, const Bitboard pawns,
                                      const int shift, const MoveFlag flag) {
  BitLooping(pawns, [&list, shift, flag](const Square to) {
    list.push(Move(to - shift, to, flag));
  });
}

inline void GenerateStandardPawnMoves(MoveList& list, const Bitboard pawns,
                                      const Direction shift, const MoveFlag flag) {
  BitLooping(pawns, [&list, shift, flag](const Square to) {
    list.push(Move(to - shift, to, flag));
  });
}

inline void AddMoves(MoveList& list, const Bitboard mask, const Square from, const Bitboard occupied) {
  BitLooping(mask, [&list, from, occupied](const Square to) {
    list.push(Move(from, to, (occupied & ToBB(to)) ? MoveFlag::kCapture : MoveFlag::kQuiet));
  });
}

void GeneratePawnPromotions(MoveList& list, const Bitboard pawns, const Direction dir, const bool is_capture) {
  MoveFlag knight_flag = (is_capture) ? MoveFlag::kKnightPromoCapture : MoveFlag::kKnightPromotion;
  MoveFlag bishop_flag = (is_capture) ? MoveFlag::kBishopPromoCapture : MoveFlag::kBishopPromotion;
  MoveFlag rook_flag = (is_capture) ? MoveFlag::kRookPromoCapture : MoveFlag::kRookPromotion;
  MoveFlag queen_flag = (is_capture) ? MoveFlag::kQueenPromoCapture : MoveFlag::kQueenPromotion;
  GenerateStandardPawnMoves(list, pawns, dir, knight_flag);
  GenerateStandardPawnMoves(list, pawns, dir, bishop_flag);
  GenerateStandardPawnMoves(list, pawns, dir, rook_flag);
  GenerateStandardPawnMoves(list, pawns, dir, queen_flag);
}

template<ColorType Color>
void GeneratePawnQuietMoves(MoveList& list, const Bitboard pawns,
                            const Bitboard empty_squares, const Bitboard target_squares) {
  constexpr Direction board_single_shift = (Color == ColorType::kWhite) ? Direction::kNorth : Direction::kSouth;
  constexpr Bitboard double_rank_mask = (Color == ColorType::kWhite) ? 0x00000000FF000000ULL : 0x000000FF00000000ULL;
  constexpr Bitboard last_rank_mask = (Color == ColorType::kWhite) ? ~kNot8Rank : ~kNot1Rank;
  const Bitboard single_shift = ShiftDir(pawns, board_single_shift);
  const Bitboard able_to_push = single_shift & empty_squares & ~last_rank_mask;
  GenerateStandardPawnMoves(list, able_to_push & target_squares, board_single_shift, MoveFlag::kQuiet);
  GenerateStandardPawnMoves(list, double_rank_mask & ShiftDir(able_to_push, board_single_shift) & empty_squares &
                            target_squares, 2 * std::to_underlying(board_single_shift), MoveFlag::kDoublePawnPush);
  GeneratePawnPromotions(list, single_shift & empty_squares & last_rank_mask & target_squares,
                                                board_single_shift, false);
}

template<ColorType Color>
void GeneratePawnCaptures(MoveList& list, const Bitboard pawns, const Position& pos, const Bitboard target_squares) {
  constexpr Direction capture_left_shift = (Color == ColorType::kWhite) ? Direction::kNorthWest
                                                                        : Direction::kSouthEast;
  constexpr Direction capture_right_shift = (Color == ColorType::kWhite) ? Direction::kNorthEast
                                                                         : Direction::kSouthWest;
  constexpr Bitboard last_rank_mask = (Color == ColorType::kWhite) ? kNot8Rank : kNot1Rank;
  const Bitboard opponent_pieces = (Color == ColorType::kWhite) ? pos.get_black_pieces() : pos.get_white_pieces();
  Bitboard left_attacks = ShiftDir(pawns, capture_left_shift);
  Bitboard right_attacks = ShiftDir(pawns, capture_right_shift);
  GenerateStandardPawnMoves(list, opponent_pieces & left_attacks & last_rank_mask & target_squares,
                                                             capture_left_shift, MoveFlag::kCapture);
  GenerateStandardPawnMoves(list, opponent_pieces & right_attacks & last_rank_mask & target_squares,
                                                             capture_right_shift, MoveFlag::kCapture);
  GeneratePawnPromotions(list, opponent_pieces & left_attacks & ~last_rank_mask & target_squares,
                                                             capture_left_shift, true);
  GeneratePawnPromotions(list, opponent_pieces & right_attacks & ~last_rank_mask & target_squares,
                                                             capture_right_shift, true);
  if (pos.is_en_passant()) {
    constexpr Direction dir = (Color == ColorType::kWhite) ? Direction::kNorth : Direction::kSouth;
    Bitboard en_passant_mask = ToBB(pos.get_en_passant());
    Bitboard ep_target_squares = ShiftDir(target_squares, dir);
    GenerateStandardPawnMoves(list, left_attacks & en_passant_mask & ep_target_squares,
                                           capture_left_shift, MoveFlag::kEpCapture);
    GenerateStandardPawnMoves(list, right_attacks & en_passant_mask & ep_target_squares,
                                           capture_right_shift, MoveFlag::kEpCapture);
  }
}

template<ColorType Color>
void GeneratePawnMoves(MoveList& list, const Position& pos, const Bitboard target_squares) {
  constexpr PieceType pawn = (PieceBase::kPawn & Color);
  const Bitboard pawns = pos.get_piece_metric(pawn);
  const Bitboard pinned_pieces = pos.get_pinned_pieces();
  GeneratePawnQuietMoves<Color>(list, pawns, ~pos.get_all_pieces(), target_squares);
  GeneratePawnCaptures<Color>(list, pawns, pos, target_squares);
}

template<ColorType Color, PieceBase Piece>
void GenerateFixedAttackPieces(MoveList& list, const Position& pos, const Bitboard target_squares) {
  constexpr PieceType piece = (Piece & Color);
  Bitboard opponent_pieces = (Color == ColorType::kWhite) ? pos.get_black_pieces() : pos.get_white_pieces();
  Bitboard own_pieces = (Color == ColorType::kWhite) ? pos.get_white_pieces() : pos.get_black_pieces();
  BitLooping(pos.get_piece_metric(piece), [opponent_pieces, own_pieces, target_squares, &list]
                                                                           (const Square from) {
    AddMoves(list, ~own_pieces & attacks::kAttacks<Piece>[from] & target_squares, from, opponent_pieces);
  });
}

template<ColorType Color>
void GenerateKnightMoves(MoveList& list, const Position& pos, const Bitboard target_squares) {
  GenerateFixedAttackPieces<Color, PieceBase::kKnight>(list, pos, target_squares);
}

template<ColorType Color>
void GenerateCastleMoves(MoveList& list, const Position& pos) {
  constexpr std::array<Bitboard, kMxCastles> kEmptySquareCastles = {
    0x0000000000000060ULL, 0x000000000000000EULL, 0x6000000000000000ULL, 0x0E00000000000000ULL
  };
  constexpr Square start_pos = (Color == ColorType::kWhite) ? Square::E1 : Square::E8;
  constexpr uint8_t ind = (Color == ColorType::kWhite) ? 0 : 2;
  constexpr std::array<MoveFlag, 2> castle_flags = {MoveFlag::kKingCastle, MoveFlag::kQueenCastle};
  const Bitboard all_pieces = pos.get_all_pieces();
  for (uint8_t j = 0; j < 2; ++j) {
    if (((pos.get_castles() >> (ind + j)) & 1) && ((all_pieces & kEmptySquareCastles[ind + j]) == 0)) {
      list.push(Move(start_pos, start_pos + kCastleShifts[j], castle_flags[j]));
    }
  }
}

template<ColorType Color>
void GenerateKingMoves(MoveList& list, const Position& pos) {
  GenerateFixedAttackPieces<Color, PieceBase::kKing>(list, pos, kAllSquares);
  GenerateCastleMoves<Color>(list, pos);
}

template<ColorType Color, PieceBase Base>
void GenerateSlidingMoves(MoveList& list, const Position& pos, const Bitboard target_squares) {
  constexpr PieceType piece = Base & Color;
  const Bitboard all_pieces = pos.get_all_pieces();
  const Bitboard own_pieces = (Color == ColorType::kWhite) ? pos.get_white_pieces() : pos.get_black_pieces();
  const Bitboard opponent_pieces = all_pieces ^ own_pieces;
  BitLooping(pos.get_piece_metric(piece), [all_pieces, own_pieces, opponent_pieces, target_squares, &list]
                                                                                       (const Square from) {
    AddMoves(list, ~own_pieces & attacks::SlidingAttacks<Base>(from, all_pieces) & target_squares,
                                                                            from, opponent_pieces);
  });
}

template<ColorType Color>
void GenerateBishopMoves(MoveList& list, const Position& pos, const Bitboard target_squares) {
  GenerateSlidingMoves<Color, PieceBase::kBishop>(list, pos, target_squares);
}

template<ColorType Color>
void GenerateRookMoves(MoveList& list, const Position& pos, const Bitboard target_squares) {
  GenerateSlidingMoves<Color, PieceBase::kRook>(list, pos, target_squares);
}

template<ColorType Color>
void GenerateQueenMoves(MoveList& list, const Position& pos, const Bitboard target_squares) {
  GenerateSlidingMoves<Color, PieceBase::kQueen>(list, pos, target_squares);
}

template<ColorType Color>
void GeneratePseudoMoves(MoveList& list, const Position& pos) {
  GeneratePawnMoves<Color>(list, pos, kAllSquares);
  GenerateKnightMoves<Color>(list, pos, kAllSquares);
  GenerateBishopMoves<Color>(list, pos, kAllSquares);
  GenerateRookMoves<Color>(list, pos, kAllSquares);
  GenerateQueenMoves<Color>(list, pos, kAllSquares);
  GenerateKingMoves<Color>(list, pos);
}

Bitboard GetAttackerLines(const Position& pos, const Square from) {
  const Bitboard attackers = pos.get_king_attackers();
  Bitboard attacker_lines = 0;
  BitLooping(attackers, [&pos, &attacker_lines, from](const Square sq) {
    if (IsSliding(pos.PieceOn(sq))) {
      attacker_lines |= kLines[from, sq];
      attacker_lines ^= ToBB(sq);
    }
  });

  return attacker_lines;
}

template<ColorType Color>
void GenerateKingEvasions(MoveList& list, const Position& pos) {
  constexpr PieceType king = PieceBase::kKing & Color;
  const Bitboard own_pieces = (Color == ColorType::kWhite) ? pos.get_white_pieces() : pos.get_black_pieces();
  const Bitboard opponent_pieces = (Color == ColorType::kWhite) ? pos.get_black_pieces() : pos.get_white_pieces();
  const Square from = GetLSB(pos.get_piece_metric(king));
  AddMoves(list, ~own_pieces & attacks::kAttacks<PieceBase::kKing>[from] & ~GetAttackerLines(pos, from),
                                                                                 from, opponent_pieces);
}

template<ColorType Color>
void GenerateEvasions(MoveList& list, const Position& pos) {
  GenerateKingEvasions<Color>(list, pos);
  if (pos.is_double_check()) [[unlikely]] {
    return;
  }
  constexpr PieceType king = PieceBase::kKing & Color;
  const Bitboard king_attackers = pos.get_king_attackers();
  const Bitboard target_squares = kBetween[GetLSB(king_attackers),
                                           GetLSB(pos.get_piece_metric(king))] |
                                           king_attackers;
  GeneratePawnMoves<Color>(list, pos, target_squares);
  GenerateKnightMoves<Color>(list, pos, target_squares);
  GenerateBishopMoves<Color>(list, pos, target_squares);
  GenerateRookMoves<Color>(list, pos, target_squares);
  GenerateQueenMoves<Color>(list, pos, target_squares);
}

template<ColorType Color>
void GenerateLegalMoves(MoveList& list, const Position& pos) {
  if (pos.is_check()) [[unlikely]] {
    GenerateEvasions<Color>(list, pos);
    // GeneratePseudoMoves<Color>(list, pos);
  }  else [[likely]] {
    GeneratePseudoMoves<Color>(list, pos);
  }
  std::size_t ind = 0;
  constexpr PieceType king = (PieceBase::kKing & Color);
  const Square king_sq = GetLSB(pos.get_piece_metric(king));
  while (ind < list.size()) {
    Move move = list[ind];
    Square from = move.get_from();
    const bool is_pinned = pos.IsPinned(from);
    if (!move.is_en_passant() && (!is_pinned && from != king_sq) ||
       (is_pinned && (ToBB(move.get_to()) & kLines[king_sq, from])) ||
        IsLegal<Color>(pos, move)) {
      ind++;
      continue;
    }
    list[ind] = list.back();
    list.pop_back();
  }
}

}// unnamed namespace

template<MovesType Type, ColorType Color>
MoveList GenerateMoves(const Position& pos, MoveList& list) {
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
void GenerateMoves(const Position& pos, MoveList& list) {
  if (pos.is_white_move()) {
    GenerateMoves<Type, ColorType::kWhite>(pos, list);
    return;
  }
  GenerateMoves<Type, ColorType::kBlack>(pos, list);
}

template<ColorType Color>
bool IsLegalEP(const Position& pos, const Square from, const Square to, const Square king_sq) {
  Bitboard bb_from = ToBB(from);
  if ((bb_from & pos.get_pinned_pieces()) && !(ToBB(to) & kLines[king_sq, from])) {
    return false;
  }
  const Direction shift = (Color == ColorType::kWhite) ? Direction::kSouth : Direction::kNorth;
  return pos.GetSquareAttackers(king_sq, bb_from | ToBB(to + shift), ToBB(to)) == 0;
}

template<ColorType Color>
bool IsLegal(const Position& pos, const Move& move) {
  constexpr PieceType king = PieceBase::kKing & Color;
  constexpr uint8_t color_ind = std::to_underlying(Color);
  const Square from = move.get_from();
  const Bitboard bb_from = ToBB(from);
  const Square to = move.get_to();
  if (pos.PieceOn(from) == king) {
    if (pos.GetSquareAttackers(to, bb_from, 0) != 0) {
      return false;
    }
    if (move.is_castle()) [[unlikely]] {
      const std::size_t ind = (move.is_king_castle()) ? 0 : 1;
      return (!pos.is_check() && (pos.GetSquareAttackers(kCastleInterSq[ind][color_ind], bb_from, 0) == 0));
    }
    return true;
  }
  // if (pos.is_double_check()) [[unlikely]] {
  //   return false;
  // }
  return IsLegalEP<Color>(pos, from, to, GetLSB(pos.get_piece_metric(king)));
  // const Bitboard pinned_pieces = pos.get_pinned_pieces();
  // const Square king_sq = GetLSB(pos.get_piece_metric(king));
  // const Bitboard bb_to = ToBB(to);
  // if (pos.is_single_check()) {
  //   if (move.is_en_passant()) [[unlikely]] {
  //     return IsLegalEP<Color>(pos, from, to, king_sq);
  //   }
  //   if (pinned_pieces & bb_from) {
  //     return false;
  //   }
  //   const Square attacker_sq = GetLSB(pos.get_king_attackers());
  //   return (bb_to & (kBetween[king_sq, attacker_sq] | ToBB(attacker_sq))) != 0;
  // }
  // if (move.is_en_passant()) [[unlikely]] {
  //   return IsLegalEP<Color>(pos, from, to, king_sq);
  // }
  // if ((bb_from & pinned_pieces) == 0) {
  //   return true;
  // }
  // return (bb_to & kLines[GetLSB(pos.get_piece_metric(king)), from]) != 0;
}

// explicit template instantiation
template void GenerateMoves<MovesType::kPseudo>(const Position& pos, MoveList& list);
template void GenerateMoves<MovesType::kLegal>(const Position& pos, MoveList& list);
template void GenerateMoves<MovesType::kCaptures>(const Position& pos, MoveList& list);
template void GenerateMoves<MovesType::kChecks>(const Position& pos, MoveList& list);
template void GenerateMoves<MovesType::kEvasions>(const Position& pos, MoveList& list);

}// namespace chess::move_generator