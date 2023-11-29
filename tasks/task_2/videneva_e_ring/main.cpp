// Copyright 2023 Videneva Ekaterina
#include <gtest/gtest.h>
#include <mpi.h>
#include <iostream>
#include <vector>
#include "./ring.h"


TEST(RingTest, CheckGenerateRandomNumbers) {
    int size = 0;
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int rank = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int min = 1, max = 10;
    int randomNumber = generateRandomNumbers(min, max);

    ASSERT_TRUE(randomNumber >= min && randomNumber <= max);
}

TEST(RingTest, CheckInitAndFinalize) {
    MPI_Init(NULL, NULL);
    MPI_Finalize();
    ASSERT_TRUE(true);
}

TEST(RingTest, CheckRankAndSize) {
    int size = 0;
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int rank = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    ASSERT_TRUE(rank >= 0 && rank < size);
}

TEST(RingTest, CheckSendMessage) {
    int size = 0;
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int rank = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int sourceRank = 0, receiverRank = size - 1;
    int message = generateRandomNumbers(1, 10);

    if (rank == sourceRank) {
        sendMessage(&message, sourceRank, receiverRank);
    } else if (rank == receiverRank) {
        sendMessage(&message, sourceRank, receiverRank);
        ASSERT_EQ(message, generateRandomNumbers(1, 10));
    }
}

TEST(RingTest, CheckSendMessageInClockwiseDirection) {
    int size = 0;
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int rank = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int sourceRank = 0, receiverRank = size - 1;
    int message = generateRandomNumbers(1, 10);

    if (rank == sourceRank) {
        sendMessage(&message, sourceRank, receiverRank);
    } else if (rank == receiverRank) {
        sendMessage(&message, sourceRank, receiverRank);
        ASSERT_EQ(message, generateRandomNumbers(1, 10));
    }
}

TEST(RingTest, CheckSameProcess) {
    int size = 0;
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int rank = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int min = 1, max = 10;
    int randomNumber = generateRandomNumbers(min, max);

    int sourceRank = 0, receiverRank = size - 1;
    int message = randomNumber;

    if (rank == sourceRank) {
        sendMessage(&message, sourceRank, receiverRank);
    } else if (rank == receiverRank) {
        sendMessage(&message, sourceRank, receiverRank);
        ASSERT_EQ(message, randomNumber);
    }
}

int main(int argc, char** argv) {
    int res = 0;
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::TestEventListeners& listeners = ::testing::UnitTest::GetInstance()->listeners();
    MPI_Init(&argc, &argv);
    res = RUN_ALL_TESTS();
    MPI_Finalize();
    return res;
}