
#include <gtest/gtest.h>
#include "AvlTree.h"
#include "AvlNode.h"

using namespace std;

TEST(AvlSequential, testInsertOne) {
    auto t = new AvlTree<int>;
    int tmp = 1;
    t->insertSeq(&tmp, 1);
    auto result = t->find(1);
    ASSERT_TRUE(result);
    EXPECT_EQ(1, result->getValue());
    EXPECT_TRUE(t->isValid());
}

TEST(AvlSequential, testInsertMultiple) {
    auto t = new AvlTree<int>;
    auto items = {1, 3, 5, 8, 6, 7, 2, 4, 1, 5};

    for (const auto &i: items) {
        t->insertSeq(&i, 1);
        auto result = t->find(i);
        ASSERT_TRUE(result);
        EXPECT_EQ(i, result->getValue());
        EXPECT_TRUE(t->isValid()); // The invariants must hold after every insertion
    }
    EXPECT_EQ(items.size(), t->size());
}
TEST(AvlParallel, testInsertOne) {
    auto t = new AvlTree<int>;
    int tmp = 1;
    t->insertPar(&tmp, 1);
    auto result = t->find(1);
    ASSERT_TRUE(result);
    EXPECT_EQ(1, result->getValue());
    EXPECT_TRUE(t->isValid());
}

TEST(AvlParallel, testInsertMultiple) {
    auto t = new AvlTree<int>;
    auto items = {1, 3, 5, 8, 6, 7, 2, 4, 1, 5};

    for (const auto &i: items) {
        t->insertPar(&i, 1);
        auto result = t->find(i);
        ASSERT_TRUE(result);
        EXPECT_EQ(i, result->getValue());
        EXPECT_TRUE(t->isValid()); // The invariants must hold after every insertion
    }
    EXPECT_EQ(items.size(), t->size());
}
int main(int argc, char **argv){
    ::testing::InitGoogleTest(&argc,argv);
    return RUN_ALL_TESTS();
}
