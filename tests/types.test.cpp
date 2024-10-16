#include "types.hpp"

#include <gtest/gtest.h>

TEST(TypesTest, Color) { EXPECT_EQ(WHITE, ~BLACK); }

TEST(TypesTest, Square) {
    EXPECT_EQ(A1 + 1, B1);
    EXPECT_EQ(A1, B1 - 1);
    EXPECT_EQ(A1 + 8, A2);
    EXPECT_EQ(A1, A2 - 8);
}

TEST(TypesTest, File) {
    EXPECT_EQ(FILE1 + 1, FILE2);
    EXPECT_EQ(FILE1, FILE2 - 1);
}

TEST(TypesTest, Rank) {
    EXPECT_EQ(RANK1 + 1, RANK2);
    EXPECT_EQ(RANK1, RANK2 - 1);
}

TEST(TypesTest, CastleRights) {
    EXPECT_EQ(ALL_CASTLE & WHITE_CASTLE, WHITE_CASTLE);
    EXPECT_EQ(ALL_CASTLE ^ WHITE_CASTLE, BLACK_CASTLE);
    EXPECT_EQ(ALL_CASTLE ^ WHITE_OOO, BLACK_CASTLE | WHITE_OO);
    EXPECT_EQ(ALL_CASTLE ^ WHITE_OO, BLACK_CASTLE | WHITE_OOO);
    EXPECT_EQ(ALL_CASTLE & BLACK_CASTLE, BLACK_CASTLE);
    EXPECT_EQ(ALL_CASTLE ^ BLACK_CASTLE, WHITE_CASTLE);
    EXPECT_EQ(ALL_CASTLE ^ BLACK_OOO, WHITE_CASTLE | BLACK_OO);
    EXPECT_EQ(ALL_CASTLE ^ BLACK_OO, WHITE_CASTLE | BLACK_OOO);
}

TEST(TypesTest, Score) {
    EXPECT_EQ(PAWNSCORE + PAWNSCORE, 2 * PAWNSCORE);
    EXPECT_EQ(PAWNSCORE + 0, PAWNSCORE - 0);
}

TEST(TypesTest, getSquare) {
    EXPECT_EQ(Types::getSquare(FILE1, RANK1), A1);
    EXPECT_EQ(Types::getSquare(FILE1, RANK8), A8);
    EXPECT_EQ(Types::getSquare(FILE8, RANK1), H1);
    EXPECT_EQ(Types::getSquare(FILE8, RANK8), H8);
    EXPECT_EQ(Types::getSquare(FILE4, RANK4), D4);
}

TEST(TypesTest, getSquareFromStr) {
    EXPECT_EQ(Types::getSquareFromStr("a1"), A1);
    EXPECT_EQ(Types::getSquareFromStr("a8"), A8);
    EXPECT_EQ(Types::getSquareFromStr("h1"), H1);
    EXPECT_EQ(Types::getSquareFromStr("h8"), H8);
    EXPECT_EQ(Types::getSquareFromStr("d4"), D4);
}

TEST(TypesTest, getRank) {
    EXPECT_EQ(Types::getRank(A1), RANK1);
    EXPECT_EQ(Types::getRank(H1), RANK1);
    EXPECT_EQ(Types::getRank(A4), RANK4);
    EXPECT_EQ(Types::getRank(H4), RANK4);
    EXPECT_EQ(Types::getRank(A8), RANK8);
    EXPECT_EQ(Types::getRank(H8), RANK8);
}

TEST(TypesTest, getFile) {
    EXPECT_EQ(Types::getFile(A1), FILE1);
    EXPECT_EQ(Types::getFile(A8), FILE1);
    EXPECT_EQ(Types::getFile(D1), FILE4);
    EXPECT_EQ(Types::getFile(D8), FILE4);
    EXPECT_EQ(Types::getFile(H1), FILE8);
    EXPECT_EQ(Types::getFile(H8), FILE8);
}

TEST(TypesTest, makePiece) {
    EXPECT_EQ(Types::makePiece(WHITE, PAWN), W_PAWN);
    EXPECT_EQ(Types::makePiece(WHITE, KNIGHT), W_KNIGHT);
    EXPECT_EQ(Types::makePiece(WHITE, BISHOP), W_BISHOP);
    EXPECT_EQ(Types::makePiece(WHITE, ROOK), W_ROOK);
    EXPECT_EQ(Types::makePiece(WHITE, QUEEN), W_QUEEN);
    EXPECT_EQ(Types::makePiece(WHITE, KING), W_KING);
    EXPECT_EQ(Types::makePiece(BLACK, PAWN), B_PAWN);
    EXPECT_EQ(Types::makePiece(BLACK, KNIGHT), B_KNIGHT);
    EXPECT_EQ(Types::makePiece(BLACK, BISHOP), B_BISHOP);
    EXPECT_EQ(Types::makePiece(BLACK, ROOK), B_ROOK);
    EXPECT_EQ(Types::makePiece(BLACK, QUEEN), B_QUEEN);
    EXPECT_EQ(Types::makePiece(BLACK, KING), B_KING);
}

TEST(TypesTest, getPieceRole) {
    EXPECT_EQ(Types::getPieceRole(W_PAWN), PAWN);
    EXPECT_EQ(Types::getPieceRole(W_KNIGHT), KNIGHT);
    EXPECT_EQ(Types::getPieceRole(W_BISHOP), BISHOP);
    EXPECT_EQ(Types::getPieceRole(W_ROOK), ROOK);
    EXPECT_EQ(Types::getPieceRole(W_QUEEN), QUEEN);
    EXPECT_EQ(Types::getPieceRole(W_KING), KING);
    EXPECT_EQ(Types::getPieceRole(B_PAWN), PAWN);
    EXPECT_EQ(Types::getPieceRole(B_KNIGHT), KNIGHT);
    EXPECT_EQ(Types::getPieceRole(B_BISHOP), BISHOP);
    EXPECT_EQ(Types::getPieceRole(B_ROOK), ROOK);
    EXPECT_EQ(Types::getPieceRole(B_QUEEN), QUEEN);
    EXPECT_EQ(Types::getPieceRole(B_KING), KING);
}

TEST(TypesTest, getPieceColor) {
    EXPECT_EQ(Types::getPieceColor(W_PAWN), WHITE);
    EXPECT_EQ(Types::getPieceColor(W_KNIGHT), WHITE);
    EXPECT_EQ(Types::getPieceColor(W_BISHOP), WHITE);
    EXPECT_EQ(Types::getPieceColor(W_ROOK), WHITE);
    EXPECT_EQ(Types::getPieceColor(W_QUEEN), WHITE);
    EXPECT_EQ(Types::getPieceColor(W_KING), WHITE);
    EXPECT_EQ(Types::getPieceColor(B_PAWN), BLACK);
    EXPECT_EQ(Types::getPieceColor(B_KNIGHT), BLACK);
    EXPECT_EQ(Types::getPieceColor(B_BISHOP), BLACK);
    EXPECT_EQ(Types::getPieceColor(B_ROOK), BLACK);
    EXPECT_EQ(Types::getPieceColor(B_QUEEN), BLACK);
    EXPECT_EQ(Types::getPieceColor(B_KING), BLACK);
}

TEST(TypesTest, pawnMovePush) {
    Square sq = Types::pawnMove<WHITE, PawnMove::PUSH, true>(E4);
    EXPECT_EQ(sq, E5);
    sq = Types::pawnMove<WHITE, PawnMove::PUSH, false>(E5);
    EXPECT_EQ(sq, E4);

    sq = Types::pawnMove<BLACK, PawnMove::PUSH, true>(E5);
    EXPECT_EQ(sq, E4);
    sq = Types::pawnMove<BLACK, PawnMove::PUSH, false>(E4);
    EXPECT_EQ(sq, E5);
}

TEST(TypesTest, pawnMoveRight) {
    Square sq = Types::pawnMove<WHITE, PawnMove::RIGHT, true>(D4);
    EXPECT_EQ(sq, E5);
    sq = Types::pawnMove<WHITE, PawnMove::RIGHT, false>(E5);
    EXPECT_EQ(sq, D4);

    sq = Types::pawnMove<BLACK, PawnMove::RIGHT, true>(E5);
    EXPECT_EQ(sq, D4);
    sq = Types::pawnMove<BLACK, PawnMove::RIGHT, false>(D4);
    EXPECT_EQ(sq, E5);
}

TEST(TypesTest, pawnMoveLeft) {
    Square sq = Types::pawnMove<WHITE, PawnMove::LEFT, true>(E4);
    EXPECT_EQ(sq, D5);
    sq = Types::pawnMove<WHITE, PawnMove::LEFT, false>(D5);
    EXPECT_EQ(sq, E4);

    sq = Types::pawnMove<BLACK, PawnMove::LEFT, true>(D5);
    EXPECT_EQ(sq, E4);
    sq = Types::pawnMove<BLACK, PawnMove::LEFT, false>(E4);
    EXPECT_EQ(sq, D5);
}
