#include "Montecarlo.h"
#include "gtest/gtest.h"
#include <math.h>

TEST(MontecarloTestSeq, VerifyResult1) { 
    long s = 1000;
    double precision = 0.5
    EXPECT_NEAR(M_PI, montecarloSeq(s), precision);
}

TEST(MontecarloTestSeq, VerifyResult2) { 
    long s = 10000;
    double precision = 0.1
    EXPECT_NEAR(M_PI, montecarloSeq(s), precision);
}

TEST(MontecarloTestSeq, VerifyResult3) { 
    long s = 100000;
    double precision = 0.05
    EXPECT_NEAR(M_PI, montecarloSeq(s), precision);
}

TEST(MontecarloTestSeq, VerifyResult4) { 
    long s = 1000000;
    double precision = 0.01
    EXPECT_NEAR(M_PI, montecarloSeq(s), precision);
}

TEST(MontecarloTestSeq, VerifyResult5) { 
    long s = 10000000;
    double precision = 0.005
    EXPECT_NEAR(M_PI, montecarloSeq(s), precision);
}

TEST(MontecarloTestPar, VerifyResult1) { 
    long s = 1000;
    double precision = 0.5
    EXPECT_NEAR(M_PI, montecarloPar(s), precision);
}

TEST(MontecarloTestPar, VerifyResult2) { 
    long s = 10000;
    double precision = 0.1
    EXPECT_NEAR(M_PI, montecarloPar(s), precision);
}

TEST(MontecarloTestPar, VerifyResult3) { 
    long s = 100000;
    double precision = 0.05
    EXPECT_NEAR(M_PI, montecarloPar(s), precision);
}

TEST(MontecarloTestPar, VerifyResult4) { 
    long s = 1000000;
    double precision = 0.01
    EXPECT_NEAR(M_PI, montecarloPar(s), precision);
}

TEST(MontecarloTestPar, VerifyResult5) { 
    long s = 10000000;
    double precision = 0.005
    EXPECT_NEAR(M_PI, montecarloPar(s), precision);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}