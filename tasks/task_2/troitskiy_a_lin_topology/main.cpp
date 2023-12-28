// Copyright 2023 Troitskiy Alexandr

#include <mpi.h>
#include <gtest/gtest.h>
#include "./lin_topology.h"

TEST(LinearTopology, Test_procs_num_2) {
    // assemble
    int rank = 0;
    int world_size = 0;
    int data = 0;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    const int receiving_rank = 1, sending_rank = 0;

    if (rank == sending_rank) {
        data = 50;
    }

    // act
    SendDataLinearTopology(&data, 1, MPI_INT, sending_rank, receiving_rank, 0, MPI_COMM_WORLD);

    // assert
    if (rank == receiving_rank) {
        ASSERT_EQ(50, data);
    }
}

TEST(LinearTopology, Test_descending) {
    // assemble
    int rank = 0;
    int world_size = 0;
    int data = 0;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    const int receiving_rank = 0;
    const int sending_rank = world_size - 1;

    if (rank == sending_rank) {
        data = 50;
    }

    // act
    SendDataLinearTopology(&data, 1, MPI_INT, sending_rank, receiving_rank, 0, MPI_COMM_WORLD);

    // assert
    if (rank == receiving_rank) {
         ASSERT_EQ(50, data);
    }
}

TEST(LinearTopology, Test_mid_descending) {
    // assemble
    int rank = 0;
    int world_size = 0;
    int data = 0;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    const int receiving_rank = 0;
    const int sending_rank = world_size / 2;

    if (rank == sending_rank) {
        data = 50;
    }

    // act
    SendDataLinearTopology(&data, 1, MPI_INT, sending_rank, receiving_rank, 0, MPI_COMM_WORLD);

    // assert
    if (rank == receiving_rank) {
        ASSERT_EQ(50, data);
    }
}

TEST(LinearTopology, Test_ascending) {
    // assemble
    int rank = 0, world_size = 0, data = 0;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    const int receiving_rank = world_size - 1, sending_rank = 0;

    if (rank == sending_rank) {
        data = 50;
    }

    // act
    SendDataLinearTopology(&data, 1, MPI_INT, sending_rank, receiving_rank, 0, MPI_COMM_WORLD);

    // assert
    if (rank == receiving_rank) {
        ASSERT_EQ(50, data);
    }
}

TEST(LinearTopology, Test_mid_ascending) {
    // assemble
    int rank = 0, world_size = 0, data = 0;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    const int receiving_rank = world_size - 1, sending_rank = world_size / 2;

    if (rank == sending_rank) {
        data = 50;
    }

    // act
    SendDataLinearTopology(&data, 1, MPI_INT, sending_rank, receiving_rank, 0, MPI_COMM_WORLD);

    // assert
    if (rank == receiving_rank) {
        ASSERT_EQ(50, data);
    }
}

int main(int argc, char** argv) {
    int result = 0;

    ::testing::InitGoogleTest(&argc, argv);
    ::testing::TestEventListeners& listeners = ::testing::UnitTest::GetInstance()->listeners();

    if (MPI_Init(&argc, &argv) != MPI_SUCCESS) MPI_Abort(MPI_COMM_WORLD, -1);
    result = RUN_ALL_TESTS();
    MPI_Finalize();

    return result;
}

