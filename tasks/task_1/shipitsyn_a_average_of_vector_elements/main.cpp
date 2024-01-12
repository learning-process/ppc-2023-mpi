// Copyright 2023 Shipitsyn Alexey
#include <mpi.h>
#include <gtest/gtest.h>
#include <vector>
#include <numeric>

#include <boost/mpi/environment.hpp>
#include <boost/mpi/communicator.hpp>

#include "task_1/shipitsyn_a_average_of_vector_elements/average_of_vector_elements.h"


TEST(Parallel_Vector_Average, Vector) {
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    std::vector<int> v = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    double av1 = getParallelAverage(v);
    if (world_rank == 0) {
        double av2 = getSequentialAverage(v);
        ASSERT_EQ(av1, 5.5);
        ASSERT_EQ(av2, 5.5);
    }
}

TEST(Parallel_Vector_Average, Random_Values) {
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    std::vector<int> v(20);
    if (world_rank == 0) {
        v = getRandomVector(20);
    }

    double av1 = getParallelAverage(v);
    if (world_rank == 0) {
        double av2 = getSequentialAverage(v);
        ASSERT_EQ(av1, av2);
    }
}

TEST(Parallel_Vector_Average, Random_Values_And_Size) {
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    int sz = 1 + (std::rand() % 100);
    std::vector<int> v(sz);
    if (world_rank == 0) {
        v = getRandomVector(sz);
    }
    double av1 = getParallelAverage(v);
    if (world_rank == 0) {
        double av2 = getSequentialAverage(v);
        ASSERT_EQ(av1, av2);
    }
}

TEST(Parallel_Vector_Average, one) {
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    std::vector<int> v(20, 1);
    double av = getParallelAverage(v);
    if (world_rank == 0) {
        ASSERT_EQ(av, 1);
    }
}

TEST(Parallel_Vector_Average, zeros) {
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    std::vector<int> v(20, 0);
    double av = getParallelAverage(v);
    if (world_rank == 0) {
        ASSERT_EQ(av, 0);
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
