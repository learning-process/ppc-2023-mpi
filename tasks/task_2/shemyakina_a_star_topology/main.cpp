// Copyright 2023 Shemyakina Alesya
#include <gtest/gtest.h>
#include <mpi.h>
#include "task_2/shemyakina_a_star_topology/star_topology.h"

TEST(Star_Topology, Test1_Master) {
    int size, rank;

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int master = 0;
    int source = size / 2;
    int receiver = size - 1;
    int data = 0;

    if (rank == source || size == 1)
        data = 200;

    StarData(MPI_COMM_WORLD, master, source, receiver, 0, &data, MPI_INT);

    if (rank == receiver) {
        ASSERT_EQ(200, data);
    }
}

TEST(Star_Topology, Test2_Master) {
    int size, rank;

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int master = 0;
    int source = master;
    int receiver = size - 1;
    int data = 0;

    if (rank == source || size == 1)
        data = 300;

    StarData(MPI_COMM_WORLD, master, source, receiver, 0, &data, MPI_INT);

    if (rank == receiver) {
        ASSERT_EQ(300, data);
    }
}

TEST(Star_Topology, Test3_Master) {
    int size, rank;

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int master = 0;
    int source = size - 1;
    int receiver = master;
    int data = 0;

    if (rank == source || size == 1)
        data = 200;

    StarData(MPI_COMM_WORLD, master, source, receiver, 0, &data, MPI_INT);

    if (rank == receiver) {
        ASSERT_EQ(200, data);
    }
}

TEST(Star_Topology, Test_Yourself) {
    int size, rank;

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int master = size - 1;
    int source = master;
    int receiver = master;
    int data = 0;

    if (rank == source || size == 1)
        data = 1000;

    StarData(MPI_COMM_WORLD, master, source, receiver, 0, &data, MPI_INT);

    if (rank == receiver) {
        ASSERT_EQ(1000, data);
    }
}

TEST(Star_Topology, Test_Processor) {
    int size, rank;

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int master = 0;
    int source = 0;
    int receiver = 0;
    int data = 0;

    if (rank == source || size == 1)
        data = 200;

    StarData(MPI_COMM_WORLD, master, source, receiver, 0, &data, MPI_INT);

    if (rank == receiver) {
        ASSERT_EQ(200, data);
    }
}

TEST(Star_Topology, Test_Processors) {
    int size, rank;

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int master = 0;
    int source = master;
    int receiver = 1;
    int data = 0;

    if (rank == source || size == 1)
        data = 200;

    StarData(MPI_COMM_WORLD, master, source, receiver, 0, &data, MPI_INT);

    if (rank == receiver) {
        ASSERT_EQ(200, data);
    }
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    ::testing::InitGoogleTest(&argc, argv);
    auto& listeners = ::testing::UnitTest::GetInstance()->listeners();

    if (rank != 0) {
        delete listeners.Release(listeners.default_result_printer());
    }
    int res = RUN_ALL_TESTS();
    MPI_Finalize();

    return res;
}
