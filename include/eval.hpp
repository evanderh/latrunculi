#ifndef LATRUNCULI_EVAL_H
#define LATRUNCULI_EVAL_H

#include "bb.hpp"
#include "types.hpp"

namespace Eval {

const int MobilityScaling[2][6] = {
    {0, 6, 2, 0, 0, 0},
    {2, 3, 1, 1, 1, 1}
};

const int PassedPawnBonus[2]    = {  30,  200 };
const int DoublePawnPenalty[2]  = { -30, -100 };
const int TriplePawnPenalty[2]  = { -45, -100 };
const int IsoPawnPenalty[2]     = { -30,  -40 };
const int OpenFileBonus[2]      = {  20,   10 };
const int HalfOpenFileBonus[2]  = {  10,    0 };
const int BishopPairBonus[2]    = {  20,   60 };

const int TEMPO_BONUS                 = 25;
const int KNIGHT_PENALTY_PER_PAWN     = -2;
const int ROOK_BONUS_PER_PAWN         = 2;
const int CONNECTED_ROOK_BONUS        = 15;
const int ROOK_ON_SEVENTH_BONUS       = 20;
const int BACK_RANK_MINOR_PENALTY     = -6;
const int MINOR_OUTPOST_BONUS         = 10;
const int STRONG_KING_SHIELD_BONUS    = 10;
const int WEAK_KING_SHIELD_BONUS      = 5;

const int PieceValues[6][2] = {
    { -PAWNSCORE,   PAWNSCORE },
    { -KNIGHTSCORE, KNIGHTSCORE },
    { -BISHOPSCORE, BISHOPSCORE },
    { -ROOKSCORE,   ROOKSCORE },
    { -QUEENSCORE,  QUEENSCORE },
    { -KINGSCORE,   KINGSCORE }
};

const int KNIGHT_TROPISM[8] = {
    0, 5, 4, 2, 0, 0, -1, -3
};
const int BISHOP_TROPISM[8] = {
    0, 5, 4, 3, 2, 1, 0, 0
};
const int ROOK_TROPISM[8] = {
    0, 6, 5, 3, 2, 1, 0, 0
};
const int QUEEN_TROPISM[8] = {
    0, 12, 10, 6, 4, 2, 0, -2
};
const int QUEEN_EARLY_DEV_PENALTY[4] = {
    0, -2, -8, -24
};

const Square ColorSq[2][64] = {
    {
        A1, B1, C1, D1, E1, F1, G1, H1,
        A2, B2, C2, D2, E2, F2, G2, H2,
        A3, B3, C3, D3, E3, F3, G3, H3,
        A4, B4, C4, D4, E4, F4, G4, H4,
        A5, B5, C5, D5, E5, F5, G5, H5,
        A6, B6, C6, D6, E6, F6, G6, H6,
        A7, B7, C7, D7, E7, F7, G7, H7,
        A8, B8, C8, D8, E8, F8, G8, H8,
    }, {
        H8, G8, F8, E8, D8, C8, B8, A8,
        H7, G7, F7, E7, D7, C7, B7, A7,
        H6, G6, F6, E6, D6, C6, B6, A6,
        H5, G5, F5, E5, D5, C5, B5, A5,
        H4, G4, F4, E4, D4, C4, B4, A4,
        H3, G3, F3, E3, D3, C3, B3, A3,
        H2, G2, F2, E2, D2, C2, B2, A2,
        H1, G1, F1, E1, D1, C1, B1, A1,
    }
};

const std::array<std::array<int, 64>, 2> PawnSqValues = {{
    {
        0,  0,  0,  0,  0,  0,  0,  0,
        50, 50, 50, 50, 50, 50, 50, 50,
        10, 10, 20, 30, 30, 20, 10, 10,
        5,  5, 10, 25, 25, 10,  5,  5,
        0,  0,  0, 20, 20,  0,  0,  0,
        5, -5,-10,  0,  0,-10, -5,  5,
        5, 10, 10,-20,-20, 10, 10,  5,
        0,  0,  0,  0,  0,  0,  0,  0
    }, {
        0,  0,  0,  0,  0,  0,  0,  0,
        115,125,125,125,125,125,125,125,
        85, 95, 95,105,105, 95, 95, 85,
        75, 85, 90,100,100, 90, 85, 65,
        65, 80, 80, 95, 95, 80, 80, 65,
        55, 75, 75, 75, 75, 75, 75, 55,
        50, 70, 70, 70, 70, 70, 70, 50,
        0,  0,  0,  0,  0,  0,  0,  0
    }
}};

const std::array<std::array<int, 64>, 2> KnightSqValues = {{
    {
        -50,-40,-30,-30,-30,-30,-40,-50,
        -40,-20,  0,  0,  0,  0,-20,-40,
        -30,  0, 10, 15, 15, 10,  0,-30,
        -30,  5, 15, 20, 20, 15,  5,-30,
        -30,  0, 15, 20, 20, 15,  0,-30,
        -30,  5, 10, 15, 15, 10,  5,-30,
        -40,-20,  0,  5,  5,  0,-20,-40,
        -50,-40,-30,-30,-30,-30,-40,-50,
    }, {
        -50,-40,-30,-30,-30,-30,-40,-50,
        -40,-20,  0,  0,  0,  0,-20,-40,
        -30,  0, 10, 15, 15, 10,  0,-30,
        -30,  5, 15, 20, 20, 15,  5,-30,
        -30,  0, 15, 20, 20, 15,  0,-30,
        -30,  5, 10, 15, 15, 10,  5,-30,
        -40,-20,  0,  5,  5,  0,-20,-40,
        -50,-40,-30,-30,-30,-30,-40,-50,
    }
}};

const std::array<std::array<int, 64>, 2> BishopSqValues = {{
    {
        -20,-10,-10,-10,-10,-10,-10,-20,
        -10,  0,  0,  0,  0,  0,  0,-10,
        -10,  0,  5, 10, 10,  5,  0,-10,
        -10,  5,  5, 10, 10,  5,  5,-10,
        -10,  0, 10, 10, 10, 10,  0,-10,
        -10, 10, 10, 10, 10, 10, 10,-10,
        -10, 10,  0,  0,  0,  0, 10,-10,
        -20,-10,-10,-10,-10,-10,-10,-20
    }, {
        -20,-10,-10,-10,-10,-10,-10,-20,
        -10,  0,  0,  0,  0,  0,  0,-10,
        -10,  0,  5, 10, 10,  5,  0,-10,
        -10,  5,  5, 10, 10,  5,  5,-10,
        -10,  0, 10, 10, 10, 10,  0,-10,
        -10, 10, 10, 10, 10, 10, 10,-10,
        -10,  5,  0,  0,  0,  0,  5,-10,
        -20,-10,-10,-10,-10,-10,-10,-20
    }
}};

const std::array<std::array<int, 64>, 2> RookSqValues = {{
    {
        0,  0,  0,  0,  0,  0,  0,  0,
        5, 10, 10, 10, 10, 10, 10,  5,
        -5,  0,  0,  0,  0,  0,  0, -5,
        -5,  0,  0,  0,  0,  0,  0, -5,
        -5,  0,  0,  0,  0,  0,  0, -5,
        -5,  0,  0,  0,  0,  0,  0, -5,
        -5,  0,  0,  0,  0,  0,  0, -5,
        0,  0,  0,  5,  5,  0,  0,  0
    }, {
        0,  0,  0,  0,  0,  0,  0,  0,
        5, 10, 10, 10, 10, 10, 10,  5,
        -5,  0,  0,  0,  0,  0,  0, -5,
        -5,  0,  0,  0,  0,  0,  0, -5,
        -5,  0,  0,  0,  0,  0,  0, -5,
        -5,  0,  0,  0,  0,  0,  0, -5,
        -5,  0,  0,  0,  0,  0,  0, -5,
        0,  0,  0,  5,  5,  0,  0,  0
    }
}};

const std::array<std::array<int, 64>, 2> QueenSqValues = {{
    {
        -20,-10,-10, -5, -5,-10,-10,-20,
        -10,  0,  0,  0,  0,  0,  0,-10,
        -10,  0,  5,  5,  5,  5,  0,-10,
        -5,  0,  5,  5,  5,  5,  0, -5,
        0,  0,  5,  5,  5,  5,  0, -5,
        -10,  5,  5,  5,  5,  5,  0,-10,
        -10,  0,  5,  0,  0,  0,  0,-10,
        -20,-10,-10, -5, -5,-10,-10,-20
    }, {
        -20,-10,-10, -5, -5,-10,-10,-20,
        -10,  0,  0,  0,  0,  0,  0,-10,
        -10,  0,  5,  5,  5,  5,  0,-10,
        -5,  0,  5,  5,  5,  5,  0, -5,
        0,  0,  5,  5,  5,  5,  0, -5,
        -10,  5,  5,  5,  5,  5,  0,-10,
        -10,  0,  5,  0,  0,  0,  0,-10,
        -20,-10,-10, -5, -5,-10,-10,-20
    }
}};

const std::array<std::array<int, 64>, 2> KingSqValues = {{
    {
        30,-40,-40,-50,-50,-40,-40,-30,
        -30,-40,-40,-50,-50,-40,-40,-30,
        -30,-40,-40,-50,-50,-40,-40,-30,
        -30,-40,-40,-50,-50,-40,-40,-30,
        -20,-30,-30,-40,-40,-30,-30,-20,
        -10,-20,-20,-20,-20,-20,-20,-10,
        20, 20,  0,  0,  0,  0, 20, 20,
        20, 30, 10,  0,  0, 10, 30, 20
    }, {
        -50,-40,-30,-20,-20,-30,-40,-50,
        -30,-20,-10,  0,  0,-10,-20,-30,
        -30,-10, 20, 30, 30, 20,-10,-30,
        -30,-10, 30, 40, 40, 30,-10,-30,
        -30,-10, 30, 40, 40, 30,-10,-30,
        -30,-10, 20, 30, 30, 20,-10,-30,
        -30,-30,  0,  0,  0,  0,-30,-30,
        -50,-30,-30,-30,-30,-30,-30,-50
    }
}};

const std::array<std::array<std::array<int, 64>, 2>, 6> PieceSqValues = {{
    PawnSqValues,
    KnightSqValues,
    BishopSqValues,
    RookSqValues,
    QueenSqValues,
    KingSqValues,
}};

inline int psqv(Color c, PieceRole p, int phase, Square sq) {
    // Get the piece square value for color c
    int score = PieceSqValues[p - 1][phase][ColorSq[c][sq]];
    return (2 * c * score) - score;
}

}

#endif