#include "MoveGenerator.hpp"
#include "Attacks.hpp"

namespace chess::move_generator {

namespace {

template<ColorType Color>
inline Bitboard GetSinglePawnPush(const Bitboard pawns) {
  if constexpr (Color == ColorType::kWhite) {
    return (pawns << 8);
  }

  return (pawns >> 8);
}

template<ColorType Color>
void GenerateStandardPawnMoves(MoveList& list, Bitboard pawns, const int shift) {
  utils::BitLooping(pawns, [&](uint8_t ind) {
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
  utils::BitLooping(pawns, [&](uint8_t ind) {
    list.push(Move(pawn, ind - shift, ind, knight));
    list.push(Move(pawn, ind - shift, ind, bishop));
    list.push(Move(pawn, ind - shift, ind, rook));
    list.push(Move(pawn, ind - shift, ind, queen));
  });
}

template<ColorType Color>
void GeneratePawnQuietMoves(MoveList& list, const Bitboard pawns, const Bitboard empty_squares) {
  constexpr int board_single_shift = (Color == ColorType::kWhite) ? 8 : -8;
  constexpr Bitboard double_rank_mask = (Color == ColorType::kWhite) ? 0x00000000FF000000ULL
                                                                     : 0x000000FF00000000ULL;
  constexpr Bitboard last_rank_mask = (Color == ColorType::kWhite) ? 0xFF00000000000000ULL
                                                                   : 0x00000000000000FFULL;
  Bitboard single_shift = GetSinglePawnPush<Color>(pawns);
  Bitboard able_to_push = single_shift & empty_squares & ~last_rank_mask;
  GenerateStandardPawnMoves<Color>(list, able_to_push, board_single_shift);
  GenerateStandardPawnMoves<Color>(list, double_rank_mask & GetSinglePawnPush<Color>(able_to_push) &
                                         empty_squares, 2 * board_single_shift);
  GeneratePawnPromotions<Color>(list, single_shift & empty_squares & last_rank_mask, board_single_shift);
}

template<ColorType Color>
void GeneratePawnCaptures(MoveList& list, const Bitboard pawns, const Position& pos) {
  Bitboard enemy_pieces = (Color == ColorType::kWhite) ? pos.get_all_black_pieces()
                                                       : pos.get_all_white_pieces();
  constexpr int capture_left_shift = (Color == ColorType::kWhite) ? 7 : -9;
  constexpr int capture_right_shift = (Color == ColorType::kWhite) ? 9 : -7;
  constexpr Bitboard not_a_file = 0xFEFEFEFEFEFEFEFEULL;
  constexpr Bitboard not_h_file = 0x7F7F7F7F7F7F7F7FULL;
  constexpr Bitboard last_rank_mask = (Color == ColorType::kWhite) ? 0xFF00000000000000ULL
                                                                   : 0x00000000000000FFULL;
  Bitboard left_attacks = (Color == ColorType::kWhite) ? ((pawns & not_a_file) << 7)
                                                       : ((pawns & not_h_file) >> 9);
  Bitboard right_attacks = (Color == ColorType::kWhite) ? ((pawns & not_h_file) << 9)
                                                        : ((pawns & not_a_file) >> 7);
  GenerateStandardPawnMoves<Color>(list, enemy_pieces & left_attacks & ~last_rank_mask, capture_left_shift);
  GenerateStandardPawnMoves<Color>(list, enemy_pieces & right_attacks & ~last_rank_mask, capture_right_shift);
  GeneratePawnPromotions<Color>(list, enemy_pieces & left_attacks & last_rank_mask, capture_left_shift);
  GeneratePawnPromotions<Color>(list, enemy_pieces & right_attacks & last_rank_mask, capture_right_shift);
  if (pos.is_en_passant()) {
    Bitboard en_passant_mask = 1ULL << pos.get_en_passant();
    GenerateStandardPawnMoves<Color>(list, left_attacks & en_passant_mask, capture_left_shift);
    GenerateStandardPawnMoves<Color>(list, right_attacks & en_passant_mask, capture_right_shift);
  }
}

template<ColorType Color>
void GeneratePawnMoves(MoveList& list, const Position& pos) {
  Bitboard pawns = (Color == ColorType::kWhite) ? pos.get_piece_metric(PieceType::kWhitePawn)
                                                : pos.get_piece_metric(PieceType::kBlackPawn);
  GeneratePawnQuietMoves<Color>(list, pawns, ~(pos.get_all_white_pieces() | pos.get_all_black_pieces()));
  GeneratePawnCaptures<Color>(list, pawns, pos);
}

template<ColorType Color>
void GenerateFixedAttackPieces(MoveList& list, const Position& pos,
                               const PieceBase piece_base, const std::array<Bitboard, 64>& attacks) {
  PieceType piece_type = piece_base & Color;
  Bitboard pieces = pos.get_piece_metric(piece_type);
  Bitboard own_pieces = (Color == ColorType::kWhite) ? pos.get_all_white_pieces()
                                                     : pos.get_all_black_pieces();
  utils::BitLooping(pieces, [own_pieces, piece_type, &list, &attacks](uint8_t from) {
    utils::BitLooping(~own_pieces & attacks[from], [from, piece_type, &list](uint8_t to) {
      list.push(Move(piece_type, from, to));
    });
  });
}

template<ColorType Color>
void GenerateKnightMoves(MoveList& list, const Position& pos) {
  GenerateFixedAttackPieces<Color>(list, pos, PieceBase::kKnight, attacks::kKnightAttacks);
}

template<ColorType Color>
void GenerateCastleMoves(MoveList& list, const Position& pos) {
  Bitboard all_pieces = pos.get_all_white_pieces() | pos.get_all_black_pieces();
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
  GenerateFixedAttackPieces<Color>(list, pos, PieceBase::kKing, attacks::kKingAttacks);
  GenerateCastleMoves<Color>(list, pos);
}

template<ColorType Color>
void GenerateBishopMoves(MoveList& list, const Position& pos) {
  Bitboard all_pieces = pos.get_all_white_pieces() | pos.get_all_black_pieces();
}

template<ColorType Color>
void GenerateRookMoves(MoveList& list, const Position& pos) {
  Bitboard all_pieces = pos.get_all_white_pieces() | pos.get_all_black_pieces();
  Bitboard rooks  = (Color == ColorType::kWhite) ? pos.get_piece_metric(PieceType::kWhiteRook)
                                                 : pos.get_piece_metric(PieceType::kBlackRook);
  Bitboard u = rooks;
  constexpr PieceType piece = PieceBase::kRook & Color;
  utils::BitLooping(u, [piece, all_pieces, rooks, &list](uint8_t from) {
    Bitboard attacks = attacks::kRookAttacks[from][(all_pieces * attacks::kRookMagicNumbers[from])
                                                 << (kBoardSize - attacks::kRookShifts[from])];
    utils::BitLooping(attacks, [&piece, from, &list](uint8_t to) {
      list.push(Move(piece, from, to));
    });
  });
  while (rooks > 0) {
    uint8_t t = std::countr_zero(rooks);
    Bitboard num = u * attacks::kRookMagicNumbers[t];
    uint8_t index = (num << (64 - 10));
    rooks &= (rooks - 1);
  }
}

template<ColorType Color>
void GenerateAllMoves(MoveList& list, const Position& pos) {
  GeneratePawnMoves<Color>(list, pos);
  GenerateKnightMoves<Color>(list, pos);
  GenerateBishopMoves<Color>(list, pos);
  GenerateRookMoves<Color>(list, pos);
  //GenerateQueenMoves<Color>(list, pos);
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
  MoveList list = GeneratePseudoMoves(pos);

  return list;
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

// explicit template instantiation
template MoveList GenerateMoves<MovesType::kPseudo>(const Position& pos);
template MoveList GenerateMoves<MovesType::kLegal>(const Position& pos);
template MoveList GenerateMoves<MovesType::kCaptures>(const Position& pos);
template MoveList GenerateMoves<MovesType::kChecks>(const Position& pos);

}// namespace chess::move_generator