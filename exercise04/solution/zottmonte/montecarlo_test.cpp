#include "montecarlo_par.cpp"
#include "gtest/gtest.h"
#include <map>

map<int, double> piResults = {{10000, 3.2192}};

TEST (MonteCarloTest, VerifyResult) { 
	long long x = 10000;	
    ASSERT_EQ (piResults.find(x)->second, getDouble(x));
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
