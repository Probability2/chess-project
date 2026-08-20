#include "Piece.hpp"
#include <algorithm>
#include <bit>
#include <concepts>
#include "../core/utils.hpp"

namespace chess::attacks {
  
namespace internal {

template<PieceBase base> // unknown piece
const std::size_t kMaxConfigurations;

template<>
const std::size_t kMaxConfigurations<PieceBase::kRook> = 4096;

template<>
const std::size_t kMaxConfigurations<PieceBase::kBishop> = 512;

inline constexpr Bitboard GetRookParams(const int square) {
  Bitboard mask = 0;
  uint8_t file = square % 8;
  uint8_t rank = square / 8;
  for (int f = 1; f < 7; ++f) {
    if (f == file) {
      continue;
    }
    mask |= (1ULL << utils::coord(rank, f));
  }
  for (int r = 1; r < 7; ++r) {
    if (r == rank) {
      continue;
    }
    mask |= (1ULL << utils::coord(r, file));
  }
  
  return mask;
}

inline constexpr Bitboard GetBishopParams(const int square) {
  Bitboard mask = 0;
  int file = square % 8;
  int rank = square / 8;
  for (int diff = -8; diff < 8; ++diff) {
    if (diff == 0 || rank + diff < 1 || rank + diff > 6 || file + diff < 1 || file + diff > 6) {
      continue;
    }
    mask |= (1ULL << utils::coord(rank + diff, file + diff));
  }
  for (int diff = -8; diff < 8; ++diff) {
    if (diff == 0 || rank + diff < 1 || rank + diff > 6 || file - diff < 1 || file - diff > 6) {
      continue;
    }
    mask |= (1ULL << utils::coord(rank + diff, file - diff));
  }
  
  return mask;
}

Bitboard GetSlidingPiecesOcuppied(const int blocker, Bitboard mask) {
  Bitboard res = 0;
  uint8_t sz = 0;
  utils::BitLooping(mask, [blocker, &sz, &res](uint8_t ind) {
    if (blocker & (1 << sz)) {
      res |= (1ULL << ind);
    }
    sz++;
  });

  return res;
}

template<PieceBase Base>
Bitboard GetAttackMask(const uint8_t square, const Bitboard occupied);

template<>
Bitboard GetAttackMask<PieceBase::kRook>(const uint8_t square, const Bitboard occupied) {
  Bitboard mask = 0;
  uint8_t file = square % 8;
  uint8_t rank = square / 8;
  for (int i = file - 1; i >= 0; --i) {
    Bitboard bb = (1ULL << utils::coord(rank, i));
    mask |= bb;
    if (occupied & bb) {
      break;
    }
  }
  for (int i = file + 1; i < 8; ++i) {
    Bitboard bb = (1ULL << utils::coord(rank, i));
    mask |= bb;
    if (occupied & bb) {
      break;
    }
  }
  for (int i = rank - 1; i >= 0; --i) {
    Bitboard bb = (1ULL << utils::coord(i, file));
    mask |= bb;
    if (occupied & bb) {
      break;
    }
  }
  for (int i = rank + 1; i < 8; ++i) {
    Bitboard bb = (1ULL << utils::coord(i, file));
    mask |= bb;
    if (occupied & bb) {
      break;
    }
  }

  return mask;
}

template<>
Bitboard GetAttackMask<PieceBase::kBishop>(const uint8_t square, const Bitboard occupied) {
  Bitboard mask = 0;
  int file = square % 8;
  int rank = square / 8;
  for (int diff = 1; diff < std::min(8 - rank, 8 - file); ++diff) {
    Bitboard bb = (1ULL << utils::coord(rank + diff, file + diff));
    mask |= bb;
    if (occupied & bb) {
      break;
    }
  }
  for (int diff = -1; diff >= std::max(-rank, -file); --diff) {
    Bitboard bb = (1ULL << utils::coord(rank + diff, file + diff));
    mask |= bb;
    if (occupied & bb) {
      break;
    }
  }
  for (int diff = 1; diff < std::min(8 - rank, file + 1); ++diff) {
    Bitboard bb = (1ULL << utils::coord(rank + diff, file - diff));
    mask |= bb;
    if (occupied & bb) {
      break;
    }
  }
  for (int diff = -1; diff > std::max(-rank - 1, file - 8); --diff) {
    Bitboard bb = (1ULL << utils::coord(rank + diff, file - diff));
    mask |= bb;
    if (occupied & bb) {
      break;
    }
  }

  return mask;
}


}// namespace internal

constexpr Bitboard kNotAFile = 0xFEFEFEFEFEFEFEFEULL;
constexpr Bitboard kNotHFile = 0x7F7F7F7F7F7F7F7FULL;
constexpr Bitboard kNotABFile = 0xFCFCFCFCFCFCFCFCULL;
constexpr Bitboard kNotGHFile = 0x3F3F3F3F3F3F3F3FULL;
constexpr Bitboard kNot1Rank = 0xFFFFFFFFFFFFFF00ULL;
constexpr Bitboard kNot12Rank = 0xFFFFFFFFFFFF0000ULL;
constexpr Bitboard kNot8Rank = 0x00FFFFFFFFFFFFFFULL;
constexpr Bitboard kNot78Rank = 0x0000FFFFFFFFFFFFULL;

template<PieceBase Piece>
inline constexpr std::array<Bitboard, kBoardSize> kAttacks;//unknown piece

template<>
inline constexpr std::array<std::array<Bitboard, kBoardSize>, 2> kAttacks<PieceBase::kPawn> = []() {
  std::array<std::array<Bitboard, kBoardSize>, 2> attacks{};
  for (std::size_t i = 0; i < kBoardSize; ++i) {
    Bitboard pawn_coord = 1ULL << i;
    attacks[0][i] = ((pawn_coord & kNotAFile) << 7) | ((pawn_coord & kNotHFile) << 9);//white
    attacks[1][i] = ((pawn_coord & kNotHFile) >> 7) | ((pawn_coord & kNotAFile) >> 9);//black
  }

  return attacks;
}();

template<>
inline constexpr std::array<Bitboard, kBoardSize> kAttacks<PieceBase::kKnight> = []() {
  std::array<Bitboard, kBoardSize> attacks{};
  for (std::size_t i = 0; i < kBoardSize; ++i) {
    Bitboard knight_coord = 1ULL << i;
    attacks[i] = (knight_coord & kNotAFile & kNot78Rank) << 15; //e4->d6
    attacks[i] |= (knight_coord & kNotABFile & kNot8Rank) << 6; //e4->c5
    attacks[i] |= (knight_coord & kNotABFile & kNot1Rank) >> 10;//e4->c3
    attacks[i] |= (knight_coord & kNotAFile & kNot12Rank) >> 17;//e4->d2
    attacks[i] |= (knight_coord & kNotHFile & kNot12Rank) >> 15;//e4->f2
    attacks[i] |= (knight_coord & kNotGHFile & kNot1Rank) >> 6; //e4->g3
    attacks[i] |= (knight_coord & kNotGHFile & kNot8Rank) << 10;//e4->g5
    attacks[i] |= (knight_coord & kNotHFile & kNot78Rank) << 17;//e4->f6
  }

  return attacks;
}();

template<>
std::array<Bitboard, kBoardSize> kAttacks<PieceBase::kKing> = []() {
  std::array<Bitboard, kBoardSize> attacks{};
  for (std::size_t i = 0; i < kBoardSize; ++i) {
    Bitboard king_coord = 1ULL << i;
    attacks[i] = (king_coord & kNotAFile) >> 1;             //e4->d4
    attacks[i] |= (king_coord & kNotAFile & kNot1Rank) >> 9;//e4->d3
    attacks[i] |= (king_coord & kNot1Rank) >> 8;            //e4->e3
    attacks[i] |= (king_coord & kNotHFile & kNot1Rank) >> 7;//e4->f3
    attacks[i] |= (king_coord & kNotHFile) << 1;            //e4->f4
    attacks[i] |= (king_coord & kNotHFile & kNot8Rank) << 9;//e4->f5
    attacks[i] |= (king_coord & kNot8Rank) << 8;            //e4->e5
    attacks[i] |= (king_coord & kNotAFile & kNot8Rank) << 7;//e4->d5
  }
  
  return attacks;
}();

template<>
inline constexpr std::array<Bitboard, kBoardSize> kAttacks<PieceBase::kRook> = [](){
  std::array<Bitboard, kBoardSize> attacks{};
  for (int sq = 0; sq < kBoardSize; ++sq) {
    attacks[sq] = internal::GetRookParams(sq);
  }
  
  return attacks;
}();

template<>
inline constexpr std::array<Bitboard, kBoardSize> kAttacks<PieceBase::kBishop> = [](){
  std::array<Bitboard, kBoardSize> attacks{};
  for (int sq = 0; sq < kBoardSize; ++sq) {
    attacks[sq] = internal::GetBishopParams(sq);
  }
  
  return attacks;
}();

template<PieceBase Piece>
inline constexpr std::array<uint8_t, kBoardSize> kShifts;// uknown piece

template<>
inline constexpr std::array<uint8_t, kBoardSize> kShifts<PieceBase::kRook> = [](){
  std::array<uint8_t, kBoardSize> attacks{};
  for (int sq = 0; sq < kBoardSize; ++sq) {
    attacks[sq] = static_cast<uint8_t>(std::popcount(kAttacks<PieceBase::kRook>[sq]));
  }
  
  return attacks;
}();

template<>
inline constexpr std::array<uint8_t, kBoardSize> kShifts<PieceBase::kBishop> = [](){
  std::array<uint8_t, kBoardSize> attacks{};
  for (int sq = 0; sq < kBoardSize; ++sq) {
    attacks[sq] = static_cast<uint8_t>(std::popcount(kAttacks<PieceBase::kBishop>[sq]));
  }
  
  return attacks;
}();


template<PieceBase base>
inline constexpr std::array<Bitboard, kBoardSize> kMagicBitboards;// unknown piece

template<>
inline constexpr std::array<Bitboard, kBoardSize> kMagicBitboards<PieceBase::kRook> = {
  0xA080002150400089ULL, 0x5900235181004001ULL, 0xAD00290041600010ULL, 0x1080080086100080ULL, //A1-D1
  0x2A00082064500200ULL, 0x1500040026190048ULL, 0x1800E0016801500ULL,  0x100084C84210006ULL,  //E1-H1
  0x2C80218001C000ULL,   0x4068400AA0100042ULL, 0x4060022005480C0ULL,  0xA8150023004A9000ULL, //A2-D2
  0x7810008000D0010ULL,  0x7D66001002001548ULL, 0x4C2009A004C0108ULL,  0x8442000212440285ULL, //E2-H2
  0xC4C8318000400184ULL, 0x6EA480804006A008ULL, 0x8A00828020011000ULL, 0x2080A0020114200ULL,  //A3-D3
  0x805801000C090010ULL, 0x610100060C0058ULL,   0x812040092280510ULL,  0x4A58060009418409ULL, //E3-H3
  0x4000988000C120ULL,   0x21A0400900210288ULL, 0x100080C200220212ULL, 0x175620200124028ULL,  //A4-D4
  0x546880080640080ULL,  0x400401801201004ULL,  0x100106400083A01ULL,  0x700A954600010094ULL, //E4-H4
  0x40008020800044ULL,   0xCA0A0083C2002300ULL, 0x3211804252002204ULL, 0x600C082101001004ULL, //A5-D5
  0x318200A006001830ULL, 0x20A004C42005008ULL,  0x7825080244000150ULL, 0x2A16640982002045ULL, //E5-H5
  0x920286C00063800CULL, 0x2100040A0074002ULL,  0x5C11100A0050040ULL,  0x8050989200420020ULL, //A6-D6
  0xA104000800818004ULL, 0x238904020180104ULL,  0x1300032608040010ULL, 0x18E1010281520004ULL, //E6-H6
  0x401102C200248A00ULL, 0x5009400100208100ULL, 0x3052422000190100ULL, 0x420500468008280ULL,  //A7-D7
  0x6120540080580080ULL, 0x48600D014482E00ULL,  0xC8C4500802218C00ULL, 0x398124510C008200ULL, //E7-H7
  0x8301022A40800031ULL, 0x1281460100851062ULL, 0x18130402000E905ULL,  0x6081205001490065ULL, //A8-D8
  0x28F6002C08211056ULL, 0x1115000A34008801ULL, 0x60863084E100194ULL,  0x1181C3850C074026ULL, //E8-H8
};

template<>
inline constexpr std::array<Bitboard, kBoardSize> kMagicBitboards<PieceBase::kBishop> = {
  0xB084282248042101ULL, 0xC02104040894A19EULL, 0x2C8109400828200ULL, 0xD404040C84000109ULL,  //A1-D1
  0xC8A1104000005832ULL, 0x402080C251004C0ULL,  0x4129014720200231ULL, 0x280A008411961026ULL, //E1-H1
  0xC0020204031CULL,     0x403D284381E8100ULL,  0xC4500807D4088133ULL, 0xF800082046C90042ULL, //A2-D2
  0x420A69104012A428ULL, 0x2024230520108000ULL, 0x6809A1C338201088ULL, 0x217800240C220809ULL, //E2-H2
  0x4205026008902109ULL, 0x860600A004011E15ULL, 0x202D00C0044A9ULL, 0x321800648A00C030ULL,    //A3-D3
  0x72820A402080014ULL,  0x1206041101090500ULL, 0x2C00415A021010ULL, 0xA75821B05880592ULL,    //E3-H3
  0x810103141252B02ULL,  0x1A20204A0840408ULL,  0x81C1E60010028208ULL, 0xC288021080200A0ULL,  //A4-D4
  0x4E13007005004000ULL, 0x9458820041180600ULL, 0xAA160E0D0C008210ULL, 0xA4E1020199007108ULL, //E4-H4
  0x481C4040CC1810ULL,   0x152021091200100ULL,  0x9082919008020404ULL, 0xC85760080280080ULL,  //A5-D5
  0x9810040020C106ULL,   0xA1602E4080170082ULL, 0x48E6108C21010408ULL, 0x2250A40100618180ULL, //E5-H5
  0x1443016840486001ULL, 0x501C040403101442ULL, 0xE48A029404003204ULL, 0x200802C601820800ULL, //A6-D6
  0x2D40404101000611ULL, 0x9191011005800100ULL, 0x8088089800C10281ULL, 0x410448600829040ULL,  //E6-H6
  0x5682480410280075ULL, 0x9010D0801540804ULL,  0x250D006184108803ULL, 0x8D00288B094800ACULL, //A7-D7
  0x104C450C10440400ULL, 0x8D240888101C2800ULL, 0x4190102508048026ULL, 0x9428C1C14024D01ULL,  //E7-H7
  0x1325040503080A00ULL, 0x854C15D308901000ULL, 0x801606704039210ULL, 0x20428E223A08842ULL,   //A8-D8
  0xA000872061A24400ULL, 0x5C4854C0D2C40106ULL, 0x26014088453C0080ULL, 0x1010100129040094ULL  //E8-H8
};

template<PieceBase Base>
concept SlidingPiece = (Base == PieceBase::kBishop || Base == PieceBase::kRook);

template<PieceBase Piece> requires SlidingPiece<Piece>
inline const std::array<std::array<Bitboard, internal::kMaxConfigurations<Piece>>,
kBoardSize> kSlidingAttacks = []() {
  std::array<std::array<Bitboard, internal::kMaxConfigurations<Piece>>, kBoardSize> attacks{};
  for (std::size_t sq = 0; sq < kBoardSize; ++sq) {
    for (std::size_t b = 0, config = (1 << kShifts<Piece>[sq]); b < config; ++b) {
      Bitboard occupied = internal::GetSlidingPiecesOcuppied(b, kAttacks<Piece>[sq]);
      std::size_t index = ((occupied * kMagicBitboards<Piece>[sq]) >> (kBoardSize - kShifts<Piece>[sq]));
      attacks[sq][index] = internal::GetAttackMask<Piece>(sq, occupied);
    }
  }

  return attacks;
}();

}// namespace chess::attacks