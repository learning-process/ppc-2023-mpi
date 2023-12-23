// Copyright 2023 Safarov Nurlan
#include <gtest/gtest.h>

#include "task_2/safarov_n_image_smoothing/image_smoothing.h"

TEST(Image_Smoothing, Test1_3x3) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    Image source(3, 3);
    source.FillRandom();

    Image resultParallel = SmoothingParallel(source);

    if (rank == 0) {
        Image resultSequential = SmoothingSequential(source);
        ASSERT_EQ(resultParallel, resultSequential);
    }
}

TEST(Image_Smoothing, Test2_10x10) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    Image source(10, 10);
    source.FillRandom();

    Image resultParallel = SmoothingParallel(source);

    if (rank == 0) {
        Image resultSequential = SmoothingSequential(source);
        ASSERT_EQ(resultParallel, resultSequential);
    }
}

TEST(Image_Smoothing, Test3_40x150) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    Image source(40, 150);
    source.FillRandom();

    Image resultParallel = SmoothingParallel(source);

    if (rank == 0) {
        Image resultSequential = SmoothingSequential(source);
        ASSERT_EQ(resultParallel, resultSequential);
    }
}

TEST(Image_Smoothing, Test4_100x50) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    Image source(40, 150);
    source.FillRandom();

    Image resultParallel = SmoothingParallel(source);

    if (rank == 0) {
        Image resultSequential = SmoothingSequential(source);
        ASSERT_EQ(resultParallel, resultSequential);
    }
}

TEST(Image_Smoothing, Test5_400x400) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    Image source(400, 400);
    source.FillRandom();

    Image resultParallel = SmoothingParallel(source);

    if (rank == 0) {
        Image resultSequential = SmoothingSequential(source);
        ASSERT_EQ(resultParallel, resultSequential);
    }
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::TestEventListeners& listeners =
        ::testing::UnitTest::GetInstance()->listeners();

    if (MPI_Init(&argc, &argv) != MPI_SUCCESS) MPI_Abort(MPI_COMM_WORLD, -1);

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank != 0) {
        delete listeners.Release(listeners.default_result_printer());
    }
    int exec_code = RUN_ALL_TESTS();
    MPI_Finalize();
    return exec_code;
}
