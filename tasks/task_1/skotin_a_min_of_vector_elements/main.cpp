// Copyright 2023 Skotin Alexander
#include <gtest/gtest.h>
#include <mpi.h>
#include <vector>
#include <algorithm>
#include "task_1/skotin_a_min_of_vector_elements/min_of_vector_elements.h"

class MinVectorElementsTest : public ::testing::Test {
 protected:
    void SetUp() override {
        MPI_Comm_rank(MPI_COMM_WORLD, &worldRank);
    }

    int worldRank;
};

TEST_F(MinVectorElementsTest, TestRandom) {
    std::vector<int> vec;
    if (worldRank == 0) {
        vec = generateRandomVector(100);
    }

    int globalMin = findMinElementMPI(vec, vec.size());

    if (worldRank == 0) {
        int expectedMin = *std::min_element(vec.begin(), vec.end());
        ASSERT_EQ(expectedMin, globalMin);
    }
}

TEST_F(MinVectorElementsTest, TestPositiveOnly) {
    std::vector<int> vec;
    if (worldRank == 0) {
        vec = generateRandomVector(100, 1, 1000);
    }

    int globalMin = findMinElementMPI(vec, vec.size());

    if (worldRank == 0) {
        int expectedMin = *std::min_element(vec.begin(), vec.end());
        ASSERT_EQ(expectedMin, globalMin);
    }
}

TEST_F(MinVectorElementsTest, TestNegativeOnly) {
    std::vector<int> vec;
    if (worldRank == 0) {
        vec = generateRandomVector(100, -1000, -1);
    }

    int globalMin = findMinElementMPI(vec, vec.size());

    if (worldRank == 0) {
        int expectedMin = *std::min_element(vec.begin(), vec.end());
        ASSERT_EQ(expectedMin, globalMin);
    }
}

TEST_F(MinVectorElementsTest, TestSingleElement) {
    std::vector<int> vec;
    if (worldRank == 0) {
        vec = {42};
    }

    int globalMin = findMinElementMPI(vec, vec.size());

    if (worldRank == 0) {
        ASSERT_EQ(42, globalMin);
    }
}

TEST_F(MinVectorElementsTest, TestIdenticalElements) {
    std::vector<int> vec;
    if (worldRank == 0) {
        vec.assign(100, 5);
    }

    int globalMin = findMinElementMPI(vec, vec.size());

    if (worldRank == 0) {
        ASSERT_EQ(5, globalMin);
    }
}

TEST_F(MinVectorElementsTest, TestEmptyVector) {
    std::vector<int> vec;

    int globalMin = findMinElementMPI(vec, vec.size());

    if (worldRank == 0) {
        ASSERT_EQ(INT_MAX, globalMin);
    }
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    ::testing::InitGoogleTest(&argc, argv);
    int result = RUN_ALL_TESTS();
    MPI_Finalize();
    return result;
}
