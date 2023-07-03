#ifndef LABA6_GTESTS_H
#define LABA6_GTESTS_H

#include <gtest/gtest.h>

TEST(PolynomTest, PositivePositive) {
    EXPECT_EQ((Polynom<1, 2>{}.GetAt(1)), 3);
    EXPECT_EQ((Polynom<1, 2, 3>{}.GetAt(2)), 11);
    EXPECT_EQ((Polynom<1, 2, 3, 4>{}.GetAt(3)), 58);
    EXPECT_EQ((Polynom<1, 2, 3, 4, 5>{}.GetAt(50)), 6507705);
}

TEST(PolynomTest, PositiveNegative) {
    EXPECT_EQ((Polynom<1, 2>{}.GetAt(-1)), 1);
    EXPECT_EQ((Polynom<1, 2, 3>{}.GetAt(-2)), 3);
    EXPECT_EQ((Polynom<1, 2, 3, 4>{}.GetAt(-3)), -14);
    EXPECT_EQ((Polynom<1, 2, 3, 4, 5>{}.GetAt(-50)), 6007305);
}

TEST(PolynomTest, NegativePositive) {
    EXPECT_EQ((Polynom<-1, -2>{}.GetAt(1)), -3);
    EXPECT_EQ((Polynom<-1, -2, -3>{}.GetAt(2)), -11);
    EXPECT_EQ((Polynom<-1, -2, -3, -4>{}.GetAt(3)), -58);
    EXPECT_EQ((Polynom<-1, -2, -3, -4, -5>{}.GetAt(50)), -6507705);
}

TEST(PolynomTest, NegativeNegative) {
    EXPECT_EQ((Polynom<-1, -2>{}.GetAt(-1)), -1);
    EXPECT_EQ((Polynom<-1, -2, -3>{}.GetAt(-2)), -3);
    EXPECT_EQ((Polynom<-1, -2, -3, -4>{}.GetAt(-3)), 14);
    EXPECT_EQ((Polynom<-1, -2, -3, -4, -5>{}.GetAt(-50)), -6007305);
}

TEST(PolynomTest, Zeros) {
    EXPECT_EQ((Polynom<1, 0>{}.GetAt(-1)), -1);
    EXPECT_EQ((Polynom<1, 0, 3>{}.GetAt(2)), 7);
    EXPECT_EQ((Polynom<1, 3, 2, 1>{}.GetAt(0)), 1);
    EXPECT_EQ((Polynom<-1, -2, 0, -4, -5>{}.GetAt(0)), -5);
}



#endif //LABA6_GTESTS_H
