// Copyright 2023 Alexseev Danila

#include <gtest/gtest.h>

#include "./count_words.h"


TEST(CountWordsInStringTest, RandomString) {
    std::string globalStr;
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    globalStr = getRandString();

    int globalWordCount = parallelCountWordsInString(globalStr);

    if (rank == 0) {
        int reference_sum = sequentialCountWordsInString(globalStr);
        ASSERT_EQ(reference_sum, globalWordCount);
    }
}

TEST(CountWordsInStringTest, EmptyString) {
    std::string globalStr;
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    globalStr = "";

    int globalWordCount = parallelCountWordsInString(globalStr);

    if (rank == 0) {
        int reference_sum = sequentialCountWordsInString(globalStr);
        ASSERT_EQ(reference_sum, globalWordCount);
    }
}

TEST(CountWordsInStringTest, OneSymbolString) {
    std::string globalStr;
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    globalStr = "f";

    int globalWordCount = parallelCountWordsInString(globalStr);

    if (rank == 0) {
        int reference_sum = sequentialCountWordsInString(globalStr);
        ASSERT_EQ(reference_sum, globalWordCount);
    }
}

TEST(CountWordsInStringTest, FirstSymbolIsSpaceString) {
    std::string globalStr;
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    globalStr = " " + getRandString();

    int globalWordCount = parallelCountWordsInString(globalStr);

    if (rank == 0) {
        int reference_sum = sequentialCountWordsInString(globalStr);
        ASSERT_EQ(reference_sum, globalWordCount);
    }
}

TEST(CountWordsInStringTest, LastSymbolIsSpaceRandomString) {
    std::string globalStr;
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    globalStr = getRandString() + " ";

    int globalWordCount = parallelCountWordsInString(globalStr);

    if (rank == 0) {
        int reference_sum = sequentialCountWordsInString(globalStr);
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
