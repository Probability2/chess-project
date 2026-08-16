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

void BitLooping(Bitboard pawns, std::invocable<uint8_t> auto&& f) {
  while (pawns > 0) {
    uint8_t ind = std::countr_zero(pawns);
    std::forward<decltype(f)>(f)(ind);
    pawns &= (pawns - 1); //Brian Kernighan's algorithm
  }
}

template<ColorType Color>
void GenerateStandardPawnMoves(MoveList& list, Bitboard pawns, const int shift) {
  BitLooping(pawns, [&](uint8_t ind) {
    list.push(Move(MakePiece(PieceBase::kPawn, Color), ind - shift, ind));
  });
}

template<ColorType Color>
void GeneratePawnPromotions(MoveList& list, Bitboard pawns, const int shift) {
  constexpr PieceType pawn = MakePiece(PieceBase::kPawn, Color);
  constexpr PieceType knight = MakePiece(PieceBase::kKnight, Color);
  constexpr PieceType bishop = MakePiece(PieceBase::kBishop, Color);
  constexpr PieceType rook = MakePiece(PieceBase::kRook, Color);
  constexpr PieceType queen = MakePiece(PieceBase::kQueen, Color);
  BitLooping(pawns, [&](uint8_t ind) {
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
  Bitboard able_to_push_twice = double_rank_mask & GetSinglePawnPush<Color>(able_to_push) & empty_squares;
  GenerateStandardPawnMoves<Color>(list, able_to_push, board_single_shift);
  GenerateStandardPawnMoves<Color>(list, able_to_push_twice, 2 * board_single_shift);
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
  PieceType piece_type = MakePiece(piece_base, Color);
  Bitboard pieces = pos.get_piece_metric(piece_type);
  Bitboard own_pieces = (Color == ColorType::kWhite) ? pos.get_all_white_pieces()
                                                     : pos.get_all_black_pieces();
  BitLooping(pieces, [own_pieces, piece_type, &list, &attacks](uint8_t from) {
    BitLooping(~own_pieces & attacks[from], [from, piece_type, &list](uint8_t to) {
      list.push(Move(piece_type, from, to));
    });
  });
}

template<ColorType Color>
void GenerateKnightMoves(MoveList& list, const Position& pos) {
  GenerateFixedAttackPieces<Color>(list, pos, PieceBase::kKnight, attacks::kKnightAttacks);
}

template<ColorType Color>
void GenerateKingMoves(MoveList& list, const Position& pos) {
  GenerateFixedAttackPieces<Color>(list, pos, PieceBase::kKing, attacks::kKingAttacks);
}

template<ColorType Color>
void GenerateAllMoves(MoveList& list, const Position& pos) {
  GeneratePawnMoves<Color>(list, pos);
  GenerateKnightMoves<Color>(list, pos);
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