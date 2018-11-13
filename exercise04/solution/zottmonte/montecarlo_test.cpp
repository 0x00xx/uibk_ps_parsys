#include "Montecarlo.h"
#include "gtest/gtest.h"

TEST(MontecarloTestSeq, VerifyResult1) { 
    long long s = 10000;
    double result = getDoubleSeq(s);
    ASSERT_DOUBLE_EQ (result, 3.1388);
}

TEST(MontecarloTestSeq, VerifyResult2) { 
    long long s = 100000;
    double result = getDoubleSeq(s);
    ASSERT_DOUBLE_EQ (result, 3.13928);
}

TEST(MontecarloTestSeq, VerifyResult3) { 
    long long s = 1000000;
    double result = getDoubleSeq(s);
    ASSERT_DOUBLE_EQ (result, 3.142896);
}

TEST(MontecarloTestSeq, VerifyResult4) { 
    long long s = 10000000;
    double result = getDoubleSeq(s);
    ASSERT_DOUBLE_EQ (result, 3.1421912);
}

/*TEST(MontecarloTestPar, VerifyResult1) { 
    long long s = 100000;
    double result = getDoublePar(s);
    ASSERT_DOUBLE_EQ (result, 3.1438);
}*/

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}