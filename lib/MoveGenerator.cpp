#include "MoveGenerator.hpp"
#include "types/Bitboard.hpp"
#include "types/Attacks.hpp"
#include "types/Piece.hpp"

namespace chess::move_generator {

namespace {

template<ColorType Color>
void GenerateStandardPawnMoves(MoveList& list, Bitboard pawns, const int shift) {
  BitLooping(pawns, [&](const uint8_t ind) {
    list.push(Move(PieceBase::kPawn & Color, ind - shift, ind));
  });
}

template<ColorType Color>
void GeneratePawnPromotions(MoveList& list, Bitboard pawns, const int shift) {
  constexpr PieceType pawn = PieceBase::kPawn & Color;
  constexpr PieceType knight = PieceBase::kKnight & Color;
  constexpr PieceType bishop = PieceBase::kBishop & Color;
  constexpr PieceType rook = PieceBase::kRook & Color;
  constexpr PieceType queen = PieceBase::kQueen & Color;
  BitLooping(pawns, [&](const uint8_t ind) {
    list.push(Move(pawn, ind - shift, ind, knight));
    list.push(Move(pawn, ind - shift, ind, bishop));
    list.push(Move(pawn, ind - shift, ind, rook));
    list.push(Move(pawn, ind - shift, ind, queen));
  });
}

template<ColorType Color>
void GeneratePawnQuietMoves(MoveList& list, const Bitboard pawns, const Bitboard empty_squares) {
  constexpr int board_single_shift = (Color == ColorType::kWhite) ? 8 : -8;
  constexpr Bitboard double_rank_mask = (Color == ColorType::kWhite) ? 0x00000000FF000000ULL : 0x000000FF00000000ULL;
  constexpr Bitboard last_rank_mask = (Color == ColorType::kWhite) ? ~kNot8Rank : ~kNot1Rank;
  Bitboard single_shift = ShiftDir(pawns, board_single_shift);
  Bitboard able_to_push = single_shift & empty_squares & ~last_rank_mask;
  GenerateStandardPawnMoves<Color>(list, able_to_push, board_single_shift);
  GenerateStandardPawnMoves<Color>(list, double_rank_mask & ShiftDir(able_to_push, board_single_shift) & empty_squares,
                                                                   2 * board_single_shift);// double pawn pushes
  GeneratePawnPromotions<Color>(list, single_shift & empty_squares & last_rank_mask, board_single_shift);
}

template<ColorType Color>
void GeneratePawnCaptures(MoveList& list, const Bitboard pawns, const Position& pos) {
  const Bitboard opponent_pieces = (Color == ColorType::kWhite) ? pos.get_all_black_pieces()
                                                                : pos.get_all_white_pieces();
  constexpr int capture_left_shift = (Color == ColorType::kWhite) ? 7 : -9;
  constexpr int capture_right_shift = (Color == ColorType::kWhite) ? 9 : -7;
  constexpr Bitboard last_rank_mask = (Color == ColorType::kWhite) ? ~kNot8Rank : ~kNot1Rank;
  Bitboard left_attacks = ShiftDir(pawns, capture_left_shift);
  Bitboard right_attacks = ShiftDir(pawns, capture_right_shift);
  GenerateStandardPawnMoves<Color>(list, opponent_pieces & left_attacks & ~last_rank_mask, capture_left_shift);
  GenerateStandardPawnMoves<Color>(list, opponent_pieces & right_attacks & ~last_rank_mask, capture_right_shift);
  GeneratePawnPromotions<Color>(list, opponent_pieces & left_attacks & last_rank_mask, capture_left_shift);
  GeneratePawnPromotions<Color>(list, opponent_pieces & right_attacks & last_rank_mask, capture_right_shift);
  if (pos.is_en_passant()) {
    Bitboard en_passant_mask = 1ULL << pos.get_en_passant();
    GenerateStandardPawnMoves<Color>(list, left_attacks & en_passant_mask, capture_left_shift);
    GenerateStandardPawnMoves<Color>(list, right_attacks & en_passant_mask, capture_right_shift);
  }
}

template<ColorType Color>
void GeneratePawnMoves(MoveList& list, const Position& pos) {
  Bitboard pawns = pos.get_piece_metric(PieceBase::kPawn & Color);
  GeneratePawnQuietMoves<Color>(list, pawns, ~pos.get_all_pieces());
  GeneratePawnCaptures<Color>(list, pawns, pos);
}

template<ColorType Color, PieceBase piece_base>
void GenerateFixedAttackPieces(MoveList& list, const Position& pos) {
  PieceType piece_type = piece_base & Color;
  Bitboard pieces = pos.get_piece_metric(piece_type);
  Bitboard own_pieces = (Color == ColorType::kWhite) ? pos.get_all_white_pieces()
                                                     : pos.get_all_black_pieces();
  BitLooping(pieces, [own_pieces, piece_type, &list](uint8_t from) {
    BitLooping(~own_pieces & attacks::kAttacks<piece_base>[from], [from, piece_type, &list](uint8_t to) {
      list.push(Move(piece_type, from, to));
    });
  });
}

template<ColorType Color>
void GenerateKnightMoves(MoveList& list, const Position& pos) {
  GenerateFixedAttackPieces<Color, PieceBase::kKnight>(list, pos);
}

template<ColorType Color>
void GenerateCastleMoves(MoveList& list, const Position& pos) {
  const Bitboard all_pieces = pos.get_all_pieces();
  constexpr PieceType piece = PieceBase::kKing & Color;
  constexpr std::array<Bitboard, kMxCastles> kEmptySquareCastles = {
    0x0000000000000060ULL, 0x000000000000000EULL, 0x6000000000000000ULL, 0x0E00000000000000ULL
  };
  constexpr std::array<int, 2> diffs = {2, -2};
  constexpr uint8_t start_pos = (Color == ColorType::kWhite) ? 4 : 60;
  constexpr uint8_t ind = (Color == ColorType::kWhite) ? 3 : 1;
  for (uint8_t j = 0; j < 2; ++j) {
    if (((pos.get_castles() >> (ind - j)) & 1) && ((all_pieces & kEmptySquareCastles[3 - ind + j]) == 0)) {
      list.push(Move(piece, start_pos, start_pos + diffs[j]));
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
  const Bitboard own_pieces = (Color == ColorType::kWhite) ? pos.get_all_white_pieces() : pos.get_all_black_pieces();
  Bitboard pieces  = pos.get_piece_metric(piece_type);
  BitLooping(pieces, [own_pieces, all_pieces, &list](const uint8_t from) {
    Bitboard attacks = ~own_pieces & attacks::SlidingAttacks<Base>(from, all_pieces);
    BitLooping(attacks, [from, &list](uint8_t to) {
      list.push(Move(piece_type, from, to));
    });
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
void GenerateAllMoves(MoveList& list, const Position& pos) {
  GeneratePawnMoves<Color>(list, pos);
  GenerateKnightMoves<Color>(list, pos);
  GenerateBishopMoves<Color>(list, pos);
  GenerateRookMoves<Color>(list, pos);
  GenerateQueenMoves<Color>(list, pos);
  GenerateKingMoves<Color>(list, pos);
}

MoveList GeneratePseudoMoves(const Position& pos) {
  MoveList list;
  if (pos.is_white_move()) {
    GenerateAllMoves<ColorType::kWhite>(list, pos);
  } else {
    GenerateAllMoves<ColorType::kBlack>(list, pos);
  }

  return list;
}

MoveList GenerateLegalMoves(const Position& pos) {
  ColorType color = ColorType::kWhite;
  if (!pos.is_white_move()) {
    color = ColorType::kBlack;
  }
  MoveList legal_list;
  MoveList pseudo_list = GeneratePseudoMoves(pos);
  for (const auto& move: pseudo_list.AsSpan()) {
    if (pos.is_white_move() && !IsLegal<ColorType::kWhite>(pos, move) ||
        !pos.is_white_move() && IsLegal<ColorType::kBlack>(pos, move)) {
      continue;
    }
    legal_list.push(move);
  }

  return legal_list;
}

}// unnamed namespace

template<MovesType Type>
MoveList GenerateMoves(const Position& pos) {
  if constexpr (Type == MovesType::kPseudo) {
    return GeneratePseudoMoves(pos);
  } else {
    return GenerateLegalMoves(pos);
  }
}

template<ColorType Color>
Bitboard GetKingAttackers(const Position& pos) {
  Bitboard attackers = 0;
  const Bitboard all_pieces = pos.get_all_pieces();
  const Bitboard own_pieces = (Color == ColorType::kWhite) ? pos.get_all_white_pieces() : pos.get_all_black_pieces();
  uint8_t king_square = std::countr_zero(pos.get_piece_metric(PieceBase::kKing & Color));
  Bitboard king_rook_attacks = ~own_pieces & attacks::SlidingAttacks<PieceBase::kRook>(king_square, all_pieces);
  Bitboard king_bishop_attacks = ~own_pieces & attacks::SlidingAttacks<PieceBase::kBishop>(king_square, all_pieces);
  attackers |= (attacks::kAttacks<PieceBase::kPawn>[std::to_underlying(Color)][king_square] &
                pos.get_piece_metric(PieceBase::kPawn & !Color));// pawn attacks
  attackers |= (attacks::kAttacks<PieceBase::kKnight>[king_square] &
                pos.get_piece_metric(PieceBase::kKnight & !Color));// knight attacks
  attackers |= (king_rook_attacks & pos.get_piece_metric(PieceBase::kRook & !Color));// rook attacks
  attackers |= (king_bishop_attacks & pos.get_piece_metric(PieceBase::kBishop & !Color));// bishop attacks
  attackers |= (king_rook_attacks & pos.get_piece_metric(PieceBase::kQueen & !Color));// queen attacks
  attackers |= (king_bishop_attacks & pos.get_piece_metric(PieceBase::kQueen & !Color));

  return attackers;
}

template<PieceBase Base>
Bitboard GetPinsBySlidingPiece(const uint8_t king_sq, const Bitboard own_pieces, const Bitboard all_pieces,
                                                                                     const Bitboard pieces) {
  Bitboard pinned_pieces = 0;
  BitLooping(pieces, [&pinned_pieces, own_pieces, all_pieces, king_sq](const uint8_t sq) {
    Bitboard attacks = ~own_pieces & attacks::SlidingAttacks<Base>(sq, all_pieces);
    Bitboard blockers = own_pieces & attacks;
    if (blockers == 0) {
      return;
    }
    if ((attacks ^ (~own_pieces & attacks::SlidingAttacks<Base>(sq, all_pieces ^ blockers))) & (1ULL << king_sq)) {
      pinned_pieces |= ((~own_pieces & attacks::SlidingAttacks<Base>(king_sq, all_pieces)) & attacks);
    }
  });

  return pinned_pieces;
}

template<ColorType Color>
Bitboard GetPinnedPieces(const Position& pos) {
  Bitboard pinned_pieces = 0;
  const Bitboard all_pieces = pos.get_all_pieces();
  const Bitboard own_pieces = (Color == ColorType::kWhite) ? pos.get_all_white_pieces() : pos.get_all_black_pieces();
  uint8_t king_sq = pos.get_piece_metric(PieceBase::kKing & Color);
  pinned_pieces |= GetPinsBySlidingPiece<PieceBase::kRook>(king_sq, own_pieces, all_pieces,
                                           pos.get_piece_metric(PieceBase::kRook & !Color));
  pinned_pieces |= GetPinsBySlidingPiece<PieceBase::kBishop>(king_sq, own_pieces, all_pieces,
                                           pos.get_piece_metric(PieceBase::kBishop & !Color));
  pinned_pieces |= GetPinsBySlidingPiece<PieceBase::kRook>(king_sq, own_pieces, all_pieces,
                                           pos.get_piece_metric(PieceBase::kQueen & !Color));
  pinned_pieces |= GetPinsBySlidingPiece<PieceBase::kBishop>(king_sq, own_pieces, all_pieces,
                                           pos.get_piece_metric(PieceBase::kQueen & !Color));

  return pinned_pieces;
}

template<ColorType Color>
bool IsLegal(const Position& pos, const Move& move) {
  Bitboard attackers = GetKingAttackers<Color>(pos);
  Bitboard pinned_pieces = GetPinnedPieces<Color>(pos);
  if (move.get_piece() == PieceType::kWhiteKing || move.get_piece() == PieceType::kBlackKing) {
    return !(attackers & (1ULL << move.get_to())) && (!move.is_castle() ||
           !(attackers & (1ULL << (move.get_from() + move.get_to()) / 2)));
  }
  if (!move.is_en_passant()) {
    // if (move.get_from() & pinned_pieces == 0 || move.get_to() & (pinned_pieces | (1ULL << ))) {
    //   return true;
    // }
    return false;
    // if (move.get_to() & )
  }

  return true;
}

// explicit template instantiation
template MoveList GenerateMoves<MovesType::kPseudo>(const Position& pos);
template MoveList GenerateMoves<MovesType::kLegal>(const Position& pos);
template MoveList GenerateMoves<MovesType::kCaptures>(const Position& pos);
template MoveList GenerateMoves<MovesType::kChecks>(const Position& pos);
template MoveList GenerateMoves<MovesType::kEvasions>(const Position& pos);

}// namespace chess::move_generator