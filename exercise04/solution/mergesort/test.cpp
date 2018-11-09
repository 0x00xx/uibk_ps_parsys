#include "MergeSort.h"
#include <gtest/gtest.h>

TEST(mergeSortSeqTest, RevertedArray) { 
    int test_array[] = {9,8,7,6,5,4,3,2,1,0};
    mergeSortSeq(test_array, 10);
    for (int i = 0; i < 10; ++i) {
        ASSERT_EQ(i, test_array[i]);
    }
}

TEST(mergeSortParTest, RevertedArray) { 
    int test_array[] = {9,8,7,6,5,4,3,2,1,0};
    mergeSortPar(test_array, 10);
    for (int i = 0; i < 10; ++i) {
        ASSERT_EQ(i, test_array[i]);
    }
}

TEST(mergeSortSeqTest, MixedArrayNeg) {
	int test_array[] = {-2, 7, 83, 90, 72, -39, -33, 53, 25, -4};
	int comparison_array[] = {-39, -33, -4, -2, 7, 25, 53, 72, 83, 90};
	mergeSortSeq(test_array, 10);
	for (int i = 0; i < 10; ++i) {
        ASSERT_EQ(comparison_array[i], test_array[i]);
    }	
}

TEST(mergeSortParTest, MixedArrayNeg) {
	int test_array[] = {-2, 7, 83, 90, 72, -39, -33, 53, 25, -4};
	int comparison_array[] = {-39, -33, -4, -2, 7, 25, 53, 72, 83, 90};
	mergeSortPar(test_array, 10);
	for (int i = 0; i < 10; ++i) {
        ASSERT_EQ(comparison_array[i], test_array[i]);
    }	
}

TEST(mergeSortSeqTest, ArrayNeg) {
	int test_array[] = {-16, -42, -13, -37, -45, -70, -77, -39, -42, -44};
	int comparison_array[] = {-77, -70, -45, -44, -42, -42, -39, -37, -16, -13};
	mergeSortSeq(test_array, 10);
	for (int i = 0; i < 10; ++i) {
        ASSERT_EQ(comparison_array[i], test_array[i]);
    }	
}

TEST(mergeSortParTest, ArrayNeg) {
	int test_array[] = {-16, -42, -13, -37, -45, -70, -77, -39, -42, -44};
	int comparison_array[] = {-77, -70, -45, -44, -42, -42, -39, -37, -16, -13};
	mergeSortPar(test_array, 10);
	for (int i = 0; i < 10; ++i) {
        ASSERT_EQ(comparison_array[i], test_array[i]);
    }	
}

TEST(mergeSortSeqTest, ArrayPos) {
	int test_array[] = {88, 51, 74, 54, 42, 78, 74, 44, 33, 73};
	int comparison_array[] = {33, 42, 44, 51, 54, 73, 74, 74, 78, 88};
	mergeSortSeq(test_array, 10);
	for (int i = 0; i < 10; ++i) {
        ASSERT_EQ(comparison_array[i], test_array[i]);
    }	
}

TEST(mergeSortParTest, ArrayPos) {
	int test_array[] = {88, 51, 74, 54, 42, 78, 74, 44, 33, 73};
	int comparison_array[] = {33, 42, 44, 51, 54, 73, 74, 74, 78, 88};
	mergeSortPar(test_array, 10);
	for (int i = 0; i < 10; ++i) {
        ASSERT_EQ(comparison_array[i], test_array[i]);
    }	
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
