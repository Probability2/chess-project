#include "MoveGenerator.hpp"

namespace chess::move_generator {

namespace {

template<ColorType Color>
PieceType GetPawnsType() {
  if (Color == ColorType::kWhite) {
    return PieceType::kWhitePawn;
  }

  return PieceType::kBlackPawn;
}

template<ColorType Color>
Bitboard GetSinglePawnPush(const Bitboard pawns) {
  if (Color == ColorType::kWhite) {
    return (pawns << 8);
  }

  return (pawns >> 8);
}

template<ColorType Color>
void GenerateStandardPawnMoves(MoveList& list, Bitboard pawns, const int shift) {
  while (pawns > 0) {
    uint8_t ind = std::countr_zero(pawns);
    list.push(Move(GetPawnsType<Color>(), ind - shift, ind));
    pawns &= (pawns - 1); //Brian Kernighan's algorithm
  }
}

template<ColorType Color>
void GeneratePawnPromotions(MoveList& list, Bitboard pawns, const int shift) {
  while (pawns > 0) {
    uint8_t ind = std::countr_zero(pawns);
    PieceType knight = (Color == ColorType::kWhite) ? PieceType::kWhiteKnight : PieceType::kBlackKnight;
    PieceType bishop = (Color == ColorType::kWhite) ? PieceType::kWhiteBishop : PieceType::kBlackBishop;
    PieceType rook = (Color == ColorType::kWhite) ? PieceType::kWhiteRook : PieceType::kBlackRook;
    PieceType queen = (Color == ColorType::kWhite) ? PieceType::kWhiteQueen : PieceType::kBlackQueen;
    list.push(Move(GetPawnsType<Color>(), ind - shift, ind, knight));
    list.push(Move(GetPawnsType<Color>(), ind - shift, ind, bishop));
    list.push(Move(GetPawnsType<Color>(), ind - shift, ind, rook));
    list.push(Move(GetPawnsType<Color>(), ind - shift, ind, queen));
    pawns &= (pawns - 1); //Brian Kernighan's algorithm
  }
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

// template<ColorType Color>
// void GenerateEnPassant(MoveList& list, )

template<ColorType Color>
void GeneratePawnMoves(MoveList& list, const Position& pos) {
  Bitboard enemy_pieces = (Color == ColorType::kWhite) ? pos.get_all_black_pieces()
                                                       : pos.get_all_white_pieces();
  Bitboard pawns = (Color == ColorType::kWhite) ? pos.get_piece_metric(PieceType::kWhitePawn)
                                                : pos.get_piece_metric(PieceType::kBlackPawn);
  GeneratePawnQuietMoves<Color>(list, pawns, ~(pos.get_all_white_pieces() | pos.get_all_black_pieces()));
  Bitboard left_shift = (Color == ColorType::kWhite) ? (pawns << 7) : (pawns >> 7);
  Bitboard right_shift = (Color == ColorType::kWhite) ? (pawns << 9) : (pawns >> 9);
  constexpr int capture_left_shift = (Color == ColorType::kWhite) ? 7 : -7;
  constexpr int capture_right_shift = (Color == ColorType::kWhite) ? 9 : -9;
  constexpr Bitboard not_a_file = 0xFEFEFEFEFEFEFEFEULL;
  constexpr Bitboard not_h_file = 0x7F7F7F7F7F7F7F7FULL;
  constexpr Bitboard last_rank_mask = (Color == ColorType::kWhite) ? 0xFF00000000000000ULL
                                                                   : 0x00000000000000FFULL;
  Bitboard left_attacks = (Color == ColorType::kWhite) ? ((pawns & not_a_file) << 7)
                                                                       : ((pawns & not_h_file) >> 7);
  Bitboard right_attacks = (Color == ColorType::kWhite) ? ((pawns & not_h_file) << 9)
                                                                        : ((pawns & not_a_file) >> 9);
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

MoveList GeneratePseudoMoves(const Position& pos) {
  MoveList list;
  // utils::PrintBitboard(pos.get_piece_metric(PieceType::kWhitePawn));
  if (pos.is_white_move()) {
    GeneratePawnMoves<ColorType::kWhite>(list, pos);
  } else {
    GeneratePawnMoves<ColorType::kBlack>(list, pos);
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

}