//  Copyright 2023 Bonyuk Petr
#include "task_3/bonyuk_p_radix_bacher/radix_bacher.h"
#include <mpi.h>
#include <gtest/gtest.h>


TEST(RadixSortTest, PositiveTest) {
 std::vector<int> v = { 10, 7, 3, 1, 9, 5, 4, 8 };
 std::vector<int> const expected_v = { 1, 3, 4, 5, 7, 8, 9, 10 };
 radixSort(v);
 EXPECT_EQ(v, expected_v);
}

TEST(RadixSortTest, NegativeTest) {
 std::vector<int> v = { -1, -3, -7, -2, -4, -12, -5, -10};
 std::vector<int> const expected_v = { -12, -10, -7, -5, -4, -3, -2, -1 };
 radixSort(v);
 EXPECT_EQ(v, expected_v);
}

TEST(RadixSortTest, RepeatTest) {
 std::vector<int> v = { 10, 7, 3, 1, 9, 7, 9, 8 };
 std::vector<int> const expected_v = { 1, 3, 7, 7, 8, 9, 9, 10 };
 radixSort(v);
 EXPECT_EQ(v, expected_v);
}

TEST(RadixSortTest, EmptyVectorTest) {
 std::vector<int> v;
 std::vector<int> const expected_v;
 radixSort(v);
 EXPECT_EQ(v, expected_v);
}

TEST(RadixSortTest, MixedPositiveAndNegativeTest) {
 std::vector<int> v = { -2, 3, -1, 5, -4, 7, 6, -8 };
 std::vector<int> const expected_v = { -8, -4, -2, -1, 3, 5, 6, 7 };
 radixSort(v);
 EXPECT_EQ(v, expected_v);
}

int main(int argc, char **argv) {
 MPI_Init(&argc, &argv);

 ::testing::InitGoogleTest(&argc, argv);
 int result = RUN_ALL_TESTS();

 MPI_Finalize();
 return result;
}
