// Copyright 2023 Yurin Andrey
#include <gtest/gtest.h>
#include "task_2/yurin_a_readers_writers/readers_writers.h"

TEST(Parallel_Operations_MPI, Only_Controller) {
    int rank = 0;
    int size_world = 0;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size_world);
    ReadersWriters(0, 0);

    if (rank == 0) {
        ASSERT_NO_THROW();
    }
}

TEST(Parallel_Operations_MPI, Controller_And_Reader) {
    int rank = 0;
    int size_world = 0;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size_world);

    ReadersWriters(1, 0);

    if (rank == 0) {
        ASSERT_NO_THROW();
    }
}

TEST(Parallel_Operations_MPI,  Controller_And_Writer) {
    int rank = 0;
    int size_world = 0;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size_world);

    ReadersWriters(0, 1);

    if (rank == 0) {
        ASSERT_NO_THROW();
    }
}

TEST(Parallel_Operations_MPI, Controller_Reader_Writer) {
    int rank = 0;
    int size_world = 0;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size_world);

    ReadersWriters(1, 1);

    if (rank == 0) {
        ASSERT_NO_THROW();
    }
}

TEST(Parallel_Operations_MPI, Controller_Two_Reader_One_Writer) {
    int rank = 0;
    int size_world = 0;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size_world);

    ReadersWriters(2, 1);

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
