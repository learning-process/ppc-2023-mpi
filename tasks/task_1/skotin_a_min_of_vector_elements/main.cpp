// Copyright 2023 Skotin Alexander
#include <gtest/gtest.h>
#include "task_1/skotin_a_min_of_vector_elements/min_of_vector_elements.h"
#include <mpi.h>

TEST(VectorMinFinder, RandomVectorTest) {
    VectorMinFinder finder;
    auto vec = VectorMinFinder::generateRandomVector(1000);

    int parallelResult = finder.parallelMin(vec);
    int sequentialResult = VectorMinFinder::sequentialMin(vec);
    ASSERT_EQ(sequentialResult, parallelResult);
}

TEST(VectorMinFinder, FixedVectorTest) {
    VectorMinFinder finder;
    std::vector<int> vec = { 15, 6, -3, 12, 9, -5, 10, 7 };

    int parallelResult = finder.parallelMin(vec);
    int sequentialResult = VectorMinFinder::sequentialMin(vec);
    ASSERT_EQ(sequentialResult, parallelResult);
}

TEST(VectorMinFinder, EmptyVectorTest) {
    VectorMinFinder finder;
    std::vector<int> vec;

    int parallelResult = finder.parallelMin(vec);
    ASSERT_EQ(std::numeric_limits<int>::max(), parallelResult);
}

TEST(VectorMinFinder, SingleElementVectorTest) {
    VectorMinFinder finder;
    std::vector<int> vec = { 42 };

    int parallelResult = finder.parallelMin(vec);
    int sequentialResult = VectorMinFinder::sequentialMin(vec);
    ASSERT_EQ(sequentialResult, parallelResult);
}

TEST(VectorMinFinder, LargeVectorTest) {
    VectorMinFinder finder;
    auto vec = VectorMinFinder::generateRandomVector(1000000);

    int parallelResult = finder.parallelMin(vec);
    int sequentialResult = VectorMinFinder::sequentialMin(vec);
    ASSERT_EQ(sequentialResult, parallelResult);
}

TEST(VectorMinFinder, NegativeValuesVectorTest) {
    VectorMinFinder finder;
    auto vec = VectorMinFinder::generateRandomVector(500, -1000, -1);

    int parallelResult = finder.parallelMin(vec);
    int sequentialResult = VectorMinFinder::sequentialMin(vec);
    ASSERT_EQ(sequentialResult, parallelResult);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    MPI_Init(&argc, &argv);

    int result = RUN_ALL_TESTS();

    MPI_Finalize();
    return result;
}
