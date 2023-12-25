// Copyright 2023 Vinokurov Ivan
#include <gtest/gtest.h>
#include <mpi.h>
#include <iostream>
#include <vector>
#include <utility>
#include "task_1/vinokurov_i_nearest_vector_neighbor_elements/nearest_vector_neighbor_elements.h"

TEST(TESTS, CanRunFunction) {
    std::vector<int> testData = { 90, 3, 8, 4, 20, 6 };

    int worldRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &worldRank);

    std::pair<int, int> res;

    ASSERT_NO_THROW(res = funcFindClosestNeighborsMPI(testData));
}

TEST(TESTS, CanCheckBasic) {
    std::vector<int> testData = { 90, 3, 8, 4, 5, 7 };

    int worldRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &worldRank);

    std::pair<int, int> res = funcFindClosestNeighborsMPI(testData);

    EXPECT_EQ(res.first, 3);
    EXPECT_EQ(res.second, 4);
}

TEST(TESTS, CanCheckEmptyVector) {
    std::vector<int> testData;

    int worldRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &worldRank);

    std::pair<int, int> res = funcFindClosestNeighborsMPI(testData);

    EXPECT_EQ(res.first, -1);
    EXPECT_EQ(res.second, -1);
}

TEST(TESTS, CanCheckNegative) {
    std::vector<int> testData = { -1, -3, -9, -4, -20, -6 };

    int worldRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &worldRank);

    std::pair<int, int> res = funcFindClosestNeighborsMPI(testData);

    EXPECT_EQ(res.first, 0);
    EXPECT_EQ(res.second, 1);
}

TEST(TESTS, CanCheckLargeNumbers) {
    std::vector<int> testData = { 999999, 350000, 505050, 111111, 100000, 543211 };

    int worldRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &worldRank);

    std::pair<int, int> res = funcFindClosestNeighborsMPI(testData);

    EXPECT_EQ(res.first, 3);
    EXPECT_EQ(res.second, 4);
}


TEST(TESTS, CanCheckEquality) {
    std::vector<int> testData = { 3, 3, 3, 3, 3, 3, 3, 3, 3 };

    int worldRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &worldRank);

    std::pair<int, int> res = funcFindClosestNeighborsMPI(testData);

    EXPECT_EQ(res.first, 0);
    EXPECT_EQ(res.second, 1);
}

int main(int argc, char** argv) {
  MPI_Init(&argc, &argv);
  ::testing::InitGoogleTest(&argc, argv);
  int result = RUN_ALL_TESTS();
  MPI_Finalize();
  return result;
}
