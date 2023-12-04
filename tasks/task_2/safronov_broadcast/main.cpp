// Copyright 2023 Safronov Mikhail
#include <gtest/gtest.h>
#include <iostream>
#include <mpi.h>
#include "./broadcast.h"
#include <vector>
#include <numeric>

TEST(broadcast, Test_MPI_INT) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int root = 0, value = 0;
    if(rank == root) {
        value = 1;
    }
    bcast(&value, 1, MPI_INT, root, MPI_COMM_WORLD);

    ASSERT_EQ(value, 1);
}

TEST(broadcast, Test_MPI_FLOAT) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int root = 0; float value = 0.0;
    if(rank == root) {
        value = 1.0;
    }
    bcast(&value, 1, MPI_FLOAT, root, MPI_COMM_WORLD);

    ASSERT_EQ(value, 1.0);
}

TEST(broadcast, Test_MPI_DOUBLE) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int root = 0; double value = 0.0;
    if(rank == root) {
        value = 1.0;
    }
    bcast(&value, 1, MPI_DOUBLE, root, MPI_COMM_WORLD);

    ASSERT_EQ(value, 1.0);
}

TEST(broadcast, test_sum_el_int) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<int> v;

    if (rank == 0)
        v = {1, 2, 3, 4, 5};
    
    MPI_Bcast(v.data(), v.size(), MPI_INT, 0, MPI_COMM_WORLD);

    int local_sum = std::accumulate(v.begin(), v.end(), 0);
    int global_sum;

    MPI_Allreduce(&local_sum, &global_sum, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);

    ASSERT_EQ(global_sum, 15);
}

int main(int argc, char** argv) {
    int resultCode = 0;
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::TestEventListeners& listeners =
        ::testing::UnitTest::GetInstance()->listeners();

    if (MPI_Init(&argc, &argv) != MPI_SUCCESS)
        MPI_Abort(MPI_COMM_WORLD, -1);
    resultCode = RUN_ALL_TESTS();
    MPI_Finalize();

    return resultCode;
}
