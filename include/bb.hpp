#ifndef LATRUNCULI_BITBOARD_H
#define LATRUNCULI_BITBOARD_H

#include <iostream>
#include <vector>

#include "defs.hpp"
#include "magics.hpp"
#include "types.hpp"

namespace BB {

// Defines a set of constants and inline functions for working with bitboards.

constexpr U64 RANK_MASK[NRANKS] = {
    0x00000000000000FF,
    0x000000000000FF00,
    0x0000000000FF0000,
    0x00000000FF000000,
    0x000000FF00000000,
    0x0000FF0000000000,
    0x00FF000000000000,
    0xFF00000000000000,
};

constexpr U64 FILE_MASK[NFILES] = {
    0x0101010101010101,
    0x0202020202020202,
    0x0404040404040404,
    0x0808080808080808,
    0x1010101010101010,
    0x2020202020202020,
    0x4040404040404040,
    0x8080808080808080,
};

constexpr std::array<U64, NSQUARES> BITSET = [] {
    std::array<U64, NSQUARES> arr = {};
    for (auto sq = 0; sq < NSQUARES; ++sq) {
        arr[sq] = 1ull << sq;
    }
    return arr;
}();

constexpr std::array<U64, NSQUARES> BITCLEAR = [] {
    std::array<U64, NSQUARES> arr = {};
    for (auto sq = 0; sq < NSQUARES; ++sq) {
        arr[sq] = ~(1ull << sq);
    }
    return arr;
}();

constexpr U64 computeBitsInline(int sq1, int sq2) {
    U64 mask = 0;
    int rank1 = sq1 / 8, file1 = sq1 % 8;
    int rank2 = sq2 / 8, file2 = sq2 % 8;

    if (rank1 == rank2) {
        mask = RANK_MASK[rank1];
    } else if (file1 == file2) {
        mask = FILE_MASK[file1];
    } else {
        int rankDiff = rank1 - rank2;
        int fileDiff = file1 - file2;

        if (rankDiff == fileDiff) {
            // ↗ diagonal
            for (int i = 0; rank1 + i < 8 && file1 + i < 8; ++i) {
                mask |= (1ULL << ((rank1 + i) * 8 + (file1 + i)));
            }
            // ↙ diagonal
            for (int i = 0; rank1 + i >= 0 && file1 + i >= 0; --i) {
                mask |= (1ULL << ((rank1 + i) * 8 + (file1 + i)));
            }
        } else if (rank1 + file1 == rank2 + file2) {
            // ↘ diagonal
            for (int i = 0; rank1 + i < 8 && file1 - i >= 0; ++i) {
                mask |= (1ULL << ((rank1 + i) * 8 + (file1 - i)));
            }
            // ↖ diagonal
            for (int i = 0; rank1 + i >= 0 && file1 - i < 8; --i) {
                mask |= (1ULL << ((rank1 + i) * 8 + (file1 - i)));
            }
        }
    }

    return mask;
}

constexpr std::array<std::array<U64, NSQUARES>, NSQUARES> BITS_INLINE = [] {
    std::array<std::array<U64, NSQUARES>, NSQUARES> table = {};
    for (auto sq1 = 0; sq1 < NSQUARES; ++sq1) {
        for (int sq2 = 0; sq2 < NSQUARES; ++sq2) {
            table[sq1][sq2] = computeBitsInline(sq1, sq2);
        }
    }
    return table;
}();

constexpr U64 computeBitsBetween(int sq1, int sq2) {
    U64 mask = 0;
    int rank1 = sq1 / 8, file1 = sq1 % 8;
    int rank2 = sq2 / 8, file2 = sq2 % 8;

    if (rank1 == rank2) {
        // Same rank
        int min_file = std::min(file1, file2);
        int max_file = std::max(file1, file2);
        for (int f = min_file + 1; f < max_file; ++f) {
            mask |= (1ULL << (rank1 * 8 + f));
        }
    } else if (file1 == file2) {
        // Same file
        int min_rank = std::min(rank1, rank2);
        int max_rank = std::max(rank1, rank2);
        for (int r = min_rank + 1; r < max_rank; ++r) {
            mask |= (1ULL << (r * 8 + file1));
        }
    } else if ((rank1 - rank2) == (file1 - file2)) {
        // Diagonal
        int min_sq = std::min(sq1, sq2);
        int max_sq = std::max(sq1, sq2);
        for (int s = min_sq + 9; s < max_sq; s += 9) {
            mask |= (1ULL << s);
        }
    } else if ((rank1 + file1) == (rank2 + file2)) {
        // Anti-diagonal
        int min_sq = std::min(sq1, sq2);
        int max_sq = std::max(sq1, sq2);
        for (int s = min_sq + 7; s < max_sq; s += 7) {
            mask |= (1ULL << s);
        }
    }

    return mask;
}

constexpr std::array<std::array<U64, NSQUARES>, NSQUARES> BITS_BETWEEN = [] {
    std::array<std::array<U64, NSQUARES>, NSQUARES> table = {};
    for (auto sq1 = 0; sq1 < NSQUARES; ++sq1) {
        for (int sq2 = 0; sq2 < NSQUARES; ++sq2) {
            table[sq1][sq2] = computeBitsBetween(sq1, sq2);
        }
    }
    return table;
}();

constexpr void addTarget(Square orig, File targetFile, Rank targetRank, std::array<U64, 64>& arr) {
    auto square = Types::getSquare(targetFile, targetRank);
    if (Types::validFile(targetFile) && Types::validRank(targetRank)) {
        arr[orig] |= BITSET[square];
    }
}

constexpr std::array<U64, NSQUARES> KNIGHT_ATTACKS = [] {
    std::array<U64, NSQUARES> arr = {};
    for (auto i = 0; i < arr.size(); ++i) {
        Square sq = static_cast<Square>(i);
        auto file = Types::getFile(sq);
        auto rank = Types::getRank(sq);
        addTarget(sq, file + 2, rank + 1, arr);
        addTarget(sq, file + 2, rank - 1, arr);
        addTarget(sq, file - 2, rank + 1, arr);
        addTarget(sq, file - 2, rank - 1, arr);
        addTarget(sq, file + 1, rank + 2, arr);
        addTarget(sq, file - 1, rank + 2, arr);
        addTarget(sq, file + 1, rank - 2, arr);
        addTarget(sq, file - 1, rank - 2, arr);
    }
    return arr;
}();

constexpr std::array<U64, NSQUARES> KING_ATTACKS = [] {
    std::array<U64, NSQUARES> arr = {};
    for (auto i = 0; i < arr.size(); ++i) {
        Square sq = static_cast<Square>(i);
        auto file = Types::getFile(sq);
        auto rank = Types::getRank(sq);
        addTarget(sq, file - 1, rank - 1, arr);
        addTarget(sq, file - 1, rank + 1, arr);
        addTarget(sq, file + 1, rank - 1, arr);
        addTarget(sq, file + 1, rank + 1, arr);
        addTarget(sq, file, rank - 1, arr);
        addTarget(sq, file, rank + 1, arr);
        addTarget(sq, file - 1, rank, arr);
        addTarget(sq, file + 1, rank, arr);
    }
    return arr;
}();

const U64 WHITESQUARES = 0x55AA55AA55AA55AA;
const U64 BLACKSQUARES = 0xAA55AA55AA55AA55;
const U64 WHITEHOLES = 0x0000003CFFFF0000;
const U64 BLACKHOLES = 0x0000FFFF3C000000;

const U64 CastlePathOO[NCOLORS] = {0x6000000000000000ull, 0x0000000000000060ull};
const U64 CastlePathOOO[NCOLORS] = {0x0E00000000000000ull, 0x000000000000000Eull};
const U64 KingCastlePathOO[NCOLORS] = {0x7000000000000000ull, 0x0000000000000070ull};
const U64 KingCastlePathOOO[NCOLORS] = {0x1C00000000000000ull, 0x000000000000001Cull};

inline U64 rankmask(Rank r, Color c) { return RANK_MASK[G::RANK_BY_COLOR[c][r]]; }
inline U64 filemask(File f, Color c) { return FILE_MASK[G::FILE_BY_COLOR[c][f]]; }

inline constexpr U64 set(const Square sq) {
    // Returns a bitboard with a single bit set corresponding to the given square.
    return BITSET[sq];
}

inline constexpr U64 clear(const Square sq) {
    // Returns a bitboard with a single bit cleared corresponding to the given square.
    return BITCLEAR[sq];
}

inline constexpr U64 bitsInline(const Square from, const Square to) {
    // Returns a bitboard representing all squares in a straight line
    // of the 'from' and 'to' squares.
    return BITS_INLINE[from][to];
}

inline constexpr U64 bitsBtwn(const Square from, const Square to) {
    // Returns a bitboard representing all squares between the 'from'
    // and 'to' squares, exclusive of the endpoints.
    return BITS_BETWEEN[from][to];
}

inline U64 moreThanOneSet(U64 bb) {
    // Returns non-zero if more than one bit is set in bb.
    return bb & (bb - 1);
}

inline int bitCount(U64 bb) {
    // Returns the count of set bits (1's) in bb.
    return __builtin_popcountll(bb);
}

inline Square lsb(U64 bb) {
    // Returns the index of the least significant bit set in bb.
    return static_cast<Square>(__builtin_ctzll(bb));
}

inline Square msb(U64 bb) {
    // Returns the index of the most significant bit set in bb.
    return static_cast<Square>(63 - __builtin_clzll(bb));
}

inline Square (*const advanced_fn[2])(U64) = {lsb, msb};

inline Square advanced(Color c, U64 bb) {
    // Calls the appropriate advanced function (lsb or msb) based on the color.
    return advanced_fn[static_cast<Color>(c)](bb);
}

template <Color c>
inline Square advanced(U64 bb) {
    // Calls the appropriate advanced function (lsb or msb) based on the color.
    return advanced_fn[static_cast<Color>(c)](bb);
}

inline U64 fillNorth(U64 bb) {
    // Fills the squares north of bb by shifting and OR'ing the bits.
    bb |= (bb << 8);
    bb |= (bb << 16);
    bb |= (bb << 32);
    return bb;
}

inline U64 fillSouth(U64 bb) {
    // Fills the squares south of bb by shifting and OR'ing the bits.
    bb |= (bb >> 8);
    bb |= (bb >> 16);
    bb |= (bb >> 32);
    return bb;
}

inline U64 fillFiles(U64 bb) {
    // Fills both north and south for the given bitboard.
    return fillNorth(bb) | fillSouth(bb);
}
inline U64 shiftSouth(U64 bb) {
    // Shifts the bitboard south (down) by 8 squares.
    return bb >> 8;
}
inline U64 shiftNorth(U64 bb) {
    // Shifts the bitboard north (up) by 8 squares.
    return bb << 8;
}

inline U64 shiftEast(U64 bb) {
    // Shifts the bitboard east (right) by 1 square and masks the invalid bits.
    return (bb << 1) & ~BB::FILE_MASK[FILE1];
}

inline U64 shiftNorthEast(U64 bb) {
    // Shifts the bitboard to the northeast (up-right) by 9 squares and masks
    // the invalid bits.
    return (bb << 9) & ~BB::FILE_MASK[FILE1];
}

inline U64 shiftSouthEast(U64 bb) {
    // Shifts the bitboard to the southeast (down-right) by 7 squares and masks
    // the invalid bits.
    return (bb >> 7) & ~BB::FILE_MASK[FILE1];
}

inline U64 shiftWest(U64 bb) {
    // Shifts the bitboard west (left) by 1 square and masks the invalid bits.
    return (bb >> 1) & ~BB::FILE_MASK[FILE8];
}

inline U64 shiftSouthWest(U64 bb) {
    // Shifts the bitboard to the southwest (down-left) by 9 squares and masks
    // the invalid bits.
    return (bb >> 9) & ~BB::FILE_MASK[FILE8];
}

inline U64 shiftNorthWest(U64 bb) {
    // Shifts the bitboard to the northwest (up-left) by 7 squares and masks the
    // invalid bits.
    return (bb << 7) & ~BB::FILE_MASK[FILE8];
}
inline U64 spanNorth(U64 bb) {
    // Fills and shifts the bitboard north to create a span.
    return shiftNorth(fillNorth(bb));
}

inline U64 spanSouth(U64 bb) {
    // Fills and shifts the bitboard south to create a span.
    return shiftSouth(fillSouth(bb));
}

template <Color c>
inline U64 spanFront(U64 bb) {
    // Returns the span in the front direction for the given color.
    return (c == WHITE) ? spanNorth(bb) : spanSouth(bb);
}

template <Color c>
inline U64 spanBack(U64 bb) {
    // Returns the span in the back direction for the given color.
    return (c == WHITE) ? spanSouth(bb) : spanNorth(bb);
}

template <Color c>
inline U64 getFrontAttackSpan(U64 bb) {
    // Returns the front attack span based on the given color's front span.
    U64 spanFrontBB = spanFront<c>(bb);
    return shiftWest(spanFrontBB) | shiftEast(spanFrontBB);
}

template <Color c>
inline U64 getAllFrontSpan(U64 bb) {
    // Returns all front span including attacks based on the given color's front
    // span.
    U64 spanFrontBB = spanFront<c>(bb);
    return shiftWest(spanFrontBB) | shiftEast(spanFrontBB) | spanFrontBB;
}

template <Color c>
inline U64 kingShield(Square sq) {
    U64 bitboard = BB::set(sq);
    if (c) {
        return (BB::shiftNorthWest(bitboard) | BB::shiftNorth(bitboard) | BB::shiftNorthEast(bitboard));
    } else {
        return (BB::shiftSouthWest(bitboard) | BB::shiftSouth(bitboard) | BB::shiftSouthEast(bitboard));
    }
}

template <PawnMove p, Color c>
inline U64 movesByPawns(U64 pawns) {
    if (p == PawnMove::LEFT) {
        pawns &= ~filemask(FILE1, c);
    } else if (p == PawnMove::RIGHT) {
        pawns &= ~filemask(FILE8, c);
    }

    if (c == WHITE) {
        return pawns << static_cast<int>(p);
    } else {
        return pawns >> static_cast<int>(p);
    }
}

template <PawnMove p>
inline U64 movesByPawns(U64 pawns, Color c) {
    if (c == WHITE) {
        return movesByPawns<p, WHITE>(pawns);
    } else {
        return movesByPawns<p, BLACK>(pawns);
    }
};
;

template <Color c>
inline U64 attacksByPawns(U64 pawns) {
    return movesByPawns<PawnMove::LEFT, c>(pawns) | movesByPawns<PawnMove::RIGHT, c>(pawns);
}

inline U64 attacksByPawns(U64 pawns, Color c) {
    return movesByPawns<PawnMove::LEFT>(pawns, c) | movesByPawns<PawnMove::RIGHT>(pawns, c);
}

template <PieceRole p>
inline U64 movesByPiece(Square sq, U64 occupancy) {
    switch (p) {
        case KNIGHT: return KNIGHT_ATTACKS[sq];
        case BISHOP: return Magics::getBishopAttacks(sq, occupancy);
        case ROOK: return Magics::getRookAttacks(sq, occupancy);
        case QUEEN: return Magics::getQueenAttacks(sq, occupancy);
        case KING: return KING_ATTACKS[sq];
    }
}

template <PieceRole p>
inline U64 movesByPiece(Square sq) {
    return movesByPiece<p>(sq, 0);
}

inline U64 movesByPiece(Square sq, PieceRole p, U64 occupancy) {
    switch (p) {
        case KNIGHT: return movesByPiece<KNIGHT>(sq, occupancy);
        case BISHOP: return movesByPiece<BISHOP>(sq, occupancy);
        case ROOK: return movesByPiece<ROOK>(sq, occupancy);
        case QUEEN: return movesByPiece<QUEEN>(sq, occupancy);
        case KING: return movesByPiece<KING>(sq, occupancy);
        default: return 0;
    }
}

inline U64 movesByPiece(Square sq, PieceRole p) { return movesByPiece(sq, p, 0); }

inline void print(std::ostream& os, const U64& bb) {
    // Prints the bitboard in a human-readable chessboard format.
    char output[64];

    // Generate visual representation of the bitboard.
    for (unsigned i = 0; i < 64; i++) {
        output[i] = (bb & BITSET[i]) ? '1' : '.';
    }

    os << "\n  abcdefgh\n";

    // Print each rank of the chessboard.
    for (int rank = 7; rank >= 0; rank--) {
        os << "  ";

        for (int file = 0; file < 8; file++) {
            os << output[Types::getSquare(File(file), Rank(rank))];
        }

        os << " " << (rank + 1) << "\n";
    }

    os << std::endl;
}

}  // namespace BB

#endif