// Copyright 2023 Chuvashov Andrey

#include <gtest/gtest.h>
#include "../tasks/task_1/chuvashov_a_count_num_of_char_in_string/count_num_of_char_in_string.h"

TEST(LettersCountingTests, EmptyString) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::string currentStr;
    const int currentSize = 0;
    if (rank == 0) {
        currentStr = GenerateRandomString(currentSize);
    }
    int parallelResult = ParallelLettersCount(currentStr);
    if (rank == 0) {
        int sequentialResult = SequentialLettersCount(currentStr);
        ASSERT_EQ(sequentialResult, parallelResult);
    }
}

TEST(LettersCountingTests, StringWithSpace) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::string currentStr;
    if (rank == 0) {
        currentStr = " ";
    }
    int parallelResult = ParallelLettersCount(currentStr);
    if (rank == 0) {
        int sequentialResult = SequentialLettersCount(currentStr);
        ASSERT_EQ(sequentialResult, parallelResult);
    }
}

TEST(LettersCountingTests, StringWithoutLetters) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::string currentStr;
    if (rank == 0) {
        currentStr = "324%^(*)(&$#@124^&21";
    }
    int parallelResult = ParallelLettersCount(currentStr);
    if (rank == 0) {
        int sequentialResult = SequentialLettersCount(currentStr);
        ASSERT_EQ(sequentialResult, parallelResult);
    }
}

TEST(LettersCountingTests, RandomGeneratedString) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::string currentStr;
    std::srand(static_cast<int>(time(0)));
    const int currentSize = std::rand() % 21 + 10;
    if (rank == 0) {
        currentStr = GenerateRandomString(currentSize);
    }
    int parallelResult = ParallelLettersCount(currentStr);
    if (rank == 0) {
        int sequentialResult = SequentialLettersCount(currentStr);
        ASSERT_EQ(sequentialResult, parallelResult);
    }
}

TEST(LettersCountingTests, OnlyLettersString) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::string currentStr;
    std::srand(static_cast<int>(time(0)));
    const int currentSize = std::rand() % 21 + 10;
    if (rank == 0) {
        currentStr = GenerateRandomOnlyLettersString(currentSize);
    }
    int parallelResult = ParallelLettersCount(currentStr);
    if (rank == 0) {
        int sequentialResult = SequentialLettersCount(currentStr);
        ASSERT_EQ(sequentialResult, parallelResult);
    }
}

int main(int argc, char** argv) {
    int result = 0;
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::TestEventListeners& listeners =
        ::testing::UnitTest::GetInstance()->listeners();
    MPI_Init(&argc, &argv);
    result = RUN_ALL_TESTS();
    MPI_Finalize();
    return result;
}
