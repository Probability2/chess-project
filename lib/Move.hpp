#include "types/Piece.hpp"

namespace chess {

enum class MovesType: uint8_t {
  kPseudo, kLegal, kCaptures, kChecks, kEvasions
};

enum class MoveFlag: uint8_t {
  kQuiet,//              0
  kDoublePawnPush,//     1
  kKingCastle,//         2
  kQueenCastle,//        3
  kCapture,//            4
  kEpCapture,//          5
  kKnightPromotion,//    8
  kBishopPromotion,//    9
  kRookPromotion,//      10
  kQueenPromotion,//     11
  kKnightPromoCapture,// 12
  kBishopPromoCapture,// 13
  kRookPromoCapture,//   14
  kQueenPromoCapture//   15
};

//16 bit representation to preserve memory
class Move {
public:
  Move() = default;
  Move(const uint8_t from, const uint8_t to);
  Move(const uint8_t from, const uint8_t to, const MoveFlag flag);
  bool operator==(const Move& other) const = default;
  uint8_t get_from() const;
  uint8_t get_to() const;
  PieceBase get_promoted_piece() const;
  MoveFlag get_flag() const;
  bool has_promoted_piece() const;
  bool is_en_passant() const;
  bool is_castle() const;

private:
  uint16_t move_val_ = 0;
};

class MoveList {
public:
  MoveList() = default;
  std::size_t size() const;
  void push(const Move& move);
  std::span<const Move> AsSpan() const;
  bool contains(const Move& move) const;

private:
  std::array<Move, kMaxMoves> moves_;
  std::size_t size_ = 0;
};

}// namespace chess