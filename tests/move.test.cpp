#include "move.hpp"

#include <gtest/gtest.h>

#include "types.hpp"

TEST(MoveTest, DefaultConstructor) {
    Move move;
    EXPECT_TRUE(move.isNullMove()) << "should create null move by default";
}

TEST(MoveTest, Constructor) {
    Square from = A2;
    Square to = A3;

    Move move(from, to);

    EXPECT_EQ(move.from(), from) << "should create move with from square";
    EXPECT_EQ(move.to(), to) << "should create move with to square";
    EXPECT_EQ(move.type(), NORMAL) << "should create normal move";
    EXPECT_FALSE(move.isNullMove()) << "should not create null move";
}

TEST(MoveTest, EqualityOperator) {
    Move move1(A2, A3);
    Move move2(A2, A3);
    Move move3(A2, A4);

    EXPECT_EQ(move1, move2) << "should be equal when squares are equal";
    EXPECT_NE(move1, move3) << "should not be equal when squares not equal";
}

TEST(MoveTest, LessThanOperator) {
    Move move1(A2, A3);
    move1.score = 10;
    Move move2(A2, A3);
    move2.score = 20;

    EXPECT_TRUE(move1 < move2) << "should be less than when score is <";
    EXPECT_FALSE(move2 < move1) << "should not be less than when score >";
}

TEST(MoveTest, PromotionMove) {
    Move move(A7, A8, PROMOTION, QUEEN);
    EXPECT_EQ(move.type(), PROMOTION) << "should create promotion move";
    EXPECT_EQ(move.promoPiece(), QUEEN) << "should handle promotion piece";
    EXPECT_FALSE(move.isNullMove()) << "should not be null move";
}

TEST(MoveTest, OutputStreamOperator) {
    Move move(A7, A8, PROMOTION, ROOK);
    EXPECT_EQ(move.DebugString(), "a7a8r") << "should output move string";
}
