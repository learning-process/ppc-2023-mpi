// Copyright 2023 Kostin Artem
#include <gtest/gtest.h>
#include <mpi.h>
#include <vector>
#include <numeric>
#include "./average_of_vector_elements.h"

TEST(Parallel_Vector_Average, Empty_Vector) {
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    std::vector<int> v(0);
    double av = getParallelAverage(v);
    if (world_rank == 0) {
        ASSERT_EQ(av, 0);
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
        double av2 = (static_cast<double> (std::reduce(v.begin(), v.end()))) /
                (static_cast<double> (v.size()));
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
        v = getRandomVector(20);
    }
    double av1 = getParallelAverage(v);
    if (world_rank == 0) {
        double av2 = (static_cast<double> (std::reduce(v.begin(), v.end()))) /
                     (static_cast<double> (v.size()));
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
    std::vector<int> v(20);
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
