// Copyright 2023 Akopyan Zal

#include <gtest/gtest.h>

#include "./symbols_count.h"


TEST(CountSymbolsInStringTest, RandomString) {
    std::string globalStr;
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    globalStr = getRandString();

    int globalWordCount = parallelCountSymbolsInString(globalStr);

    if (rank == 0) {
        int reference_sum = sequentialCountSymbolsInString(globalStr);
        ASSERT_EQ(reference_sum, globalWordCount);
    }
}

TEST(CountSymbolsInStringTest, EmptyString) {
    std::string globalStr;
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    globalStr = "";

    int globalWordCount = parallelCountSymbolsInString(globalStr);

    if (rank == 0) {
        int reference_sum = sequentialCountSymbolsInString(globalStr);
        ASSERT_EQ(reference_sum, globalWordCount);
    }
}

TEST(CountSymbolsInStringTest, OneSymbolString) {
    std::string globalStr;
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    globalStr = "f";

    int globalWordCount = parallelCountSymbolsInString(globalStr);

    if (rank == 0) {
        int reference_sum = sequentialCountSymbolsInString(globalStr);
        ASSERT_EQ(reference_sum, globalWordCount);
    }
}

TEST(CountSymbolsInStringTest, someString1) {
    std::string globalStr;
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    globalStr = " iyi gwrg wr grw g 675sfs6c5 s5c56s5566 ";

    int globalWordCount = parallelCountSymbolsInString(globalStr);

    if (rank == 0) {
        int reference_sum = sequentialCountSymbolsInString(globalStr);
        ASSERT_EQ(reference_sum, globalWordCount);
    }
}

TEST(CountSymbolsInStringTest, someString2) {
    std::string globalStr;
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    globalStr = "04 t384mt834tm083t/..';';'; cdk jw kehjw ";

    int globalWordCount = parallelCountSymbolsInString(globalStr);

    if (rank == 0) {
        int reference_sum = sequentialCountSymbolsInString(globalStr);
        ASSERT_EQ(reference_sum, globalWordCount);
    }
}

int main(int argc, char** argv) {
    int result = 0;
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::TestEventListeners& listeners = ::testing::UnitTest::GetInstance()->listeners();
    MPI_Init(&argc, &argv);
    result = RUN_ALL_TESTS();
    MPI_Finalize();
    return result;
}
