cmake_minimum_required(VERSION 2.6)
 
# Locate GTest
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})
 
# Link runTests with what we want to test and the GTest and pthread library
add_executable(runTests montecarlo_test.cpp Montecarlo.h MontecarloTesting.cpp)
target_link_libraries(runTests ${GTEST_LIBRARIES} pthread -std=c++17 -Wall -fopenmp -O3 -lgtest)
