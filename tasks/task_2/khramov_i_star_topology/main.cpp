// Copyright 2023 Khramov Ivan
#include <gtest/gtest.h>
#include <mpi.h>
#include "./star_topology.h"

TEST(Star_Topology_MPI, Test_Send_Through_Master) {
    int world_rank, world_size;

    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    int master = 0;
    int source = world_size / 2;
    int receiver = world_size - 1;
    int data = 0;

    if (world_rank == source || world_size == 1)
        data = 200;

    sendDataStar(MPI_COMM_WORLD, master, source, receiver, 0, &data, MPI_INT);

    if (world_rank == receiver) {
        ASSERT_EQ(200, data);
    }
}

TEST(Star_Topology_MPI, Test_Send_From_Master) {
    int world_rank, world_size;

    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    int master = 0;
    int source = master;
    int receiver = world_size - 1;
    int data = 0;

    if (world_rank == source || world_size == 1)
        data = 300;

    sendDataStar(MPI_COMM_WORLD, master, source, receiver, 0, &data, MPI_INT);

    if (world_rank == receiver) {
        ASSERT_EQ(300, data);
    }
}

TEST(Star_Topology_MPI, Test_Send_To_Master) {
    int world_rank, world_size;

    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    int master = 0;
    int source = world_size - 1;
    int receiver = master;
    int data = 0;

    if (world_rank == source || world_size == 1)
        data = 200;

    sendDataStar(MPI_COMM_WORLD, master, source, receiver, 0, &data, MPI_INT);

    if (world_rank == receiver) {
        ASSERT_EQ(200, data);
    }
}


TEST(Star_Topology_MPI, Test_Send_To_Yourself) {
    int world_rank, world_size;

    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    int master = world_size - 1;
    int source = master;
    int receiver = master;
    int data = 0;

    if (world_rank == source || world_size == 1)
        data = 1000;

    sendDataStar(MPI_COMM_WORLD, master, source, receiver, 0, &data, MPI_INT);

    if (world_rank == receiver) {
        ASSERT_EQ(1000, data);
    }
}

TEST(Star_Topology_MPI, Test_For_One_Processor) {
    int world_rank, world_size;

    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    int master = 0;
    int source = 0;
    int receiver = 0;
    int data = 0;

    if (world_rank == source || world_size == 1)
        data = 200;

    sendDataStar(MPI_COMM_WORLD, master, source, receiver, 0, &data, MPI_INT);

    if (world_rank == receiver) {
        ASSERT_EQ(200, data);
    }
}

TEST(Star_Topology_MPI, Test_For_Two_Processors) {
    int world_rank, world_size;

    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    int master = 0;
    int source = master;
    int receiver = 1;
    int data = 0;

    if (world_rank == source || world_size == 1)
        data = 200;

    sendDataStar(MPI_COMM_WORLD, master, source, receiver, 0, &data, MPI_INT);

    if (world_rank == receiver) {
        ASSERT_EQ(200, data);
    }
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    ::testing::InitGoogleTest(&argc, argv);
    auto& listeners = ::testing::UnitTest::GetInstance()->listeners();

    if (world_rank != 0) {
        delete listeners.Release(listeners.default_result_printer());
    }
    int result = RUN_ALL_TESTS();
    MPI_Finalize();

    return result;
}
