// Copyright 2023 Makhinya Danil
#include <gtest/gtest.h>
#include <vector>
#include <cmath>
#include "./readers_writers.h"

TEST(Parallel_Readers_Writes_MPI, Test_no_throw1) {
    int rank = 0;
    int size_world = 0;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size_world);

    run_problem_readers_writers(2, 2);

    if (rank == 0) {
        ASSERT_NO_THROW();
    }
}

TEST(Parallel_Readers_Writes_MPI, Test_no_throw2) {
    int rank = 0;
    int size_world = 0;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size_world);

    run_problem_readers_writers(1, 2);

    if (rank == 0) {
        ASSERT_NO_THROW();
    }
}

TEST(Parallel_Readers_Writes_MPI, Test_no_throw3) {
    int rank = 0;
    int size_world = 0;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size_world);

    run_problem_readers_writers(2, 1);

    if (rank == 0) {
        ASSERT_NO_THROW();
    }
}

TEST(Parallel_Readers_Writes_MPI, Test_no_throw4) {
    int rank = 0;
    int size_world = 0;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    run_problem_readers_writers(1, 1);

    if (rank == 0) {
        ASSERT_NO_THROW();
    }
}

TEST(Parallel_Readers_Writes_MPI, Test_no_throw5) {
    int rank = 0;
    int size_world = 0;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size_world);

    run_problem_readers_writers(0, 0);

    if (rank == 0) {
        ASSERT_NO_THROW();
    }
}

int main(int argc, char** argv) {
    int result_code = 0;

    ::testing::InitGoogleTest(&argc, argv);
    ::testing::TestEventListeners &listeners =
            ::testing::UnitTest::GetInstance()->listeners();

    if (MPI_Init(&argc, &argv) != MPI_SUCCESS)
        MPI_Abort(MPI_COMM_WORLD, -1);
    result_code = RUN_ALL_TESTS();
    MPI_Finalize();

    return result_code;
}
