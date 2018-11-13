#include "montecarlo_par.cpp"
#include "gtest/gtest.h"
#include <map>

std::map<long long, double> piResults = {{10000, 3.2192},{100000, 3.14496},{1000000,3.145792},{10000000,3.1407488},{100000000,3.14174976},{1000000000,3.141554976}};
long long n_values[] = {10000,100000,1000000,10000000,100000000,1000000000};

TEST (MonteCarloTest, VerifyResult) { 
    ASSERT_DOUBLE_EQ (piResults.find(n_values[0])->second, getDouble(n_values[0]));
    ASSERT_DOUBLE_EQ (piResults.find(n_values[1])->second, getDouble(n_values[1]));
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
