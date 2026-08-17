#include "Piece.hpp"

namespace chess {

enum class MovesType: uint8_t {
  kPseudo, kLegal, kCaptures, kChecks
};

struct Move {
  Move() = default;
  Move(const PieceType piece, const uint8_t from, const uint8_t to);
  Move(const PieceType piece, const uint8_t from, const uint8_t to, const PieceType promoted_piece);
  bool operator==(const Move& other) const = default;
  bool has_promoted_piece() const;
  bool is_pawn() const;// for tests only
  bool is_knight() const;// for tests only
  bool is_bishop() const;// for tests only
  bool is_queen() const;// for tests only
  bool is_king() const;// for tests only
  PieceType piece_; // the piece that has been moved
  uint8_t from_;// a-h files, 1-8 ranks
  uint8_t to_;// the same thing
  PieceType promoted_piece_ = PieceType::kNone;
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

}