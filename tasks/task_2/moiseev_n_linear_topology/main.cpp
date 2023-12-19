// Copyright 2023 Moiseev Nikita
#include <gtest/gtest.h>
#include <mpi.h>
#include <iostream>
#include "task_2/moiseev_n_linear_topology/linear_topology.h"

TEST(linear_MPI, Test1) {
    int processRank, worldSize;
    int message = 0;

    MPI_Comm_rank(MPI_COMM_WORLD, &processRank);
    MPI_Comm_size(MPI_COMM_WORLD, &worldSize);

    const int senderRank = 0;
    const int receiverRank = worldSize - 1;

    if (processRank == senderRank) {
        message = 123;
    }

    sendDataLinear(&message, 1, MPI_INT, senderRank, receiverRank, 0, MPI_COMM_WORLD);

    if (processRank == receiverRank) {
        ASSERT_EQ(123, message);
    }
}

TEST(linear_MPI, Test2) {
    int processRank, worldSize;
    int message = 0;

    MPI_Comm_rank(MPI_COMM_WORLD, &processRank);
    MPI_Comm_size(MPI_COMM_WORLD, &worldSize);

    const int senderRank = worldSize - 1;
    const int receiverRank = 0;

    if (processRank == senderRank) {
        message = 123;
    }

    sendDataLinear(&message, 1, MPI_INT, senderRank, receiverRank, 0, MPI_COMM_WORLD);

    if (processRank == receiverRank) {
        ASSERT_EQ(123, message);
    }
}

TEST(linear_MPI, Test3) {
    int processRank, worldSize;
    int message = 0;

    MPI_Comm_rank(MPI_COMM_WORLD, &processRank);
    MPI_Comm_size(MPI_COMM_WORLD, &worldSize);

    const int senderRank = 0;
    const int receiverRank = 1;

    if (processRank == senderRank) {
        message = 123;
    }

    sendDataLinear(&message, 1, MPI_INT, senderRank, receiverRank, 0, MPI_COMM_WORLD);

    if (processRank == receiverRank) {
        ASSERT_EQ(123, message);
    }
}

TEST(linear_MPI, Test4) {
    int processRank, worldSize;
    int message = 0;

    MPI_Comm_rank(MPI_COMM_WORLD, &processRank);
    MPI_Comm_size(MPI_COMM_WORLD, &worldSize);

    const int senderRank = worldSize / 2;
    const int receiverRank = worldSize - 1;

    if (processRank == senderRank) {
        message = 123;
    }

    sendDataLinear(&message, 1, MPI_INT, senderRank, receiverRank, 0, MPI_COMM_WORLD);

    if (processRank == receiverRank) {
        ASSERT_EQ(123, message);
    }
}

TEST(linear_MPI, Test5) {
    int processRank, worldSize;
    int message = 0;

    MPI_Comm_rank(MPI_COMM_WORLD, &processRank);
    MPI_Comm_size(MPI_COMM_WORLD, &worldSize);

    const int senderRank = worldSize / 2;
    const int receiverRank = 0;

    if (processRank == senderRank) {
        message = 123;
    }

    sendDataLinear(&message, 1, MPI_INT, senderRank, receiverRank, 0, MPI_COMM_WORLD);

    if (processRank == receiverRank) {
        ASSERT_EQ(123, message);
    }
}

int main(int argc, char** argv) {
    int result_code = 0;

    ::testing::InitGoogleTest(&argc, argv);
    ::testing::TestEventListeners& listeners =
        ::testing::UnitTest::GetInstance()->listeners();

    if (MPI_Init(&argc, &argv) != MPI_SUCCESS) MPI_Abort(MPI_COMM_WORLD, -1);
    result_code = RUN_ALL_TESTS();
    MPI_Finalize();

    return result_code;
}
