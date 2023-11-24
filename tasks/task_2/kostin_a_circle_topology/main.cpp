// Copyright 2023 Kostin Artem
#include <gtest/gtest.h>
#include <mpi.h>
#include <vector>
#include "./circle_topology.h"

TEST(Circle_Topology, Test_1) {
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    int from = 1, to = 3;
    int data = 0;
    if (world_size > 1){
        if (world_rank == from)
            data = 20;
        send_data(&data, from, to);
        if (world_rank == to)
            ASSERT_EQ(data, 20);
    }
}

TEST(Circle_Topology, From_Is_To) {
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    int from = 1, to = 1;
    int data = 0;
    if (world_size > 1){
        if (world_rank == from)
        data = 20;
        send_data(&data, from, to);
        if (world_rank == to)
            ASSERT_EQ(data, 20);
    }
}

TEST(Circle_Topology, Random) {
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    int from = std::rand() % world_size;
    int to = std::rand() % world_size;
    int data = 0;
    if (world_size > 1){
        if (world_rank == from)
            data = 20;
        send_data(&data, from, to);
        if (world_rank == to)
            ASSERT_EQ(data, 20);
    }
}

TEST(Circle_Topology, From_Begin_To_End) {
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    int from = 0;
    int to = world_size - 1;
    int data = 0;
    if (world_size > 1){
        if (world_rank == from)
            data = 20;
        send_data(&data, from, to);
        if (world_rank == to)
            ASSERT_EQ(data, 20);
    }
}

TEST(Circle_Topology, From_End_To_Begin) {
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    int from = world_size - 1;
    int to = 0;
    int data = 0;
    if (world_size > 1){
        if (world_rank == from)
            data = 20;
        send_data(&data, from, to);
        if (world_rank == to)
            ASSERT_EQ(data, 20);
    }
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::TestEventListeners& listeners = ::testing::UnitTest::GetInstance()->listeners();
    if (MPI_Init(&argc, &argv) != MPI_SUCCESS)
        MPI_Abort(MPI_COMM_WORLD, -1);
    int rcode = RUN_ALL_TESTS();
    MPI_Finalize();
    return rcode;
}
