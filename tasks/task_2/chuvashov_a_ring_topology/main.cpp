// Copyright 2023 Chuvashov Andrey

#include <gtest/gtest.h>
#include "../tasks/task_2/chuvashov_a_ring_topology/ring_topology.h"

TEST(RingTopologyTests, FullCicle) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int sender = 0, reciver = 0, data;

    if (rank == sender) {
        data = 1234;
    }

    if (size >= 3) {
        SendDataByRing(&data, 1, MPI_INT, sender, reciver, 0, MPI_COMM_WORLD);
    }

    if (rank == reciver) {
        ASSERT_EQ(1234, data);
    }
}

TEST(RingTopologyTests, FromOneToRandomAnother) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    std::srand(static_cast<int>(time(0)));
    int sender = 0, reciver = std::rand() % (size - 1) + 1, data;

    if (rank == sender) {
        data = 1234;
    }

    if (size >= 3) {
        SendDataByRing(&data, 1, MPI_INT, sender, reciver, 0, MPI_COMM_WORLD);
    }

    if (rank == reciver) {
        ASSERT_EQ(1234, data);
    }
}

TEST(RingTopologyTests, FromLastToFirst) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int sender = size - 1, reciver = 0, data;

    if (rank == sender) {
        data = 1234;
    }

    if (size >= 3) {
        SendDataByRing(&data, 1, MPI_INT, sender, reciver, 0, MPI_COMM_WORLD);
    }

    if (rank == reciver) {
        ASSERT_EQ(1234, data);
    }
}

TEST(RingTopologyTests, FromFirstToLast) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int sender = 0, reciver = size - 1, data;

    if (rank == sender) {
        data = 1234;
    }

    if (size >= 3) {
        SendDataByRing(&data, 1, MPI_INT, sender, reciver, 0, MPI_COMM_WORLD);
    }

    if (rank == reciver) {
        ASSERT_EQ(1234, data);
    }
}

TEST(RingTopologyTests, FromOneMiddleToAnotherMiddle) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    std::srand(static_cast<int>(time(0)));

    int sender = std::rand() % (size / 2) + size / 2 - 1,
        reciver = std::rand() % (size - size / 2) + size / 2 - 1, data;

    if (rank == sender) {
        data = 1234;
    }

    if (size >= 3) {
        SendDataByRing(&data, 1, MPI_INT, sender, reciver, 0, MPI_COMM_WORLD);
    }

    if (rank == reciver) {
        ASSERT_EQ(1234, data);
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
