// Copyright 2023 Simonyan Suren
#include <gtest/gtest.h>
#include <math.h>
#include "task_2/simonyan_s_allreduce/Allreduce.h"

TEST(Allreduce, test_Int) {
    int rank = 0;
    int size = 0;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int value = 10;
    int sum = 0;
    My_AllReduce(&value, &sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    ASSERT_EQ(sum, value * size);

    MPI_Barrier(MPI_COMM_WORLD);
}
TEST(Allreduce, test_float) {
    int rank = 0;
    int size = 0;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    float value = 10.5;
    float sum = 0;
    My_AllReduce(&value, &sum, 1, MPI_FLOAT, MPI_SUM, 0, MPI_COMM_WORLD);
    ASSERT_EQ(sum, value * size);

    MPI_Barrier(MPI_COMM_WORLD);
}
TEST(Allreduce, test_double) {
    int rank = 0;
    int size = 0;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    double value = 2.0;
    double sum = 0;
    My_AllReduce(&value, &sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    ASSERT_EQ(sum, value * size);

    MPI_Barrier(MPI_COMM_WORLD);
}
TEST(Allreduce, test_multi) {
    int rank = 0;
    int size = 0;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int value = 2;
    int sum = 0;
    My_AllReduce(&value, &sum, 1, MPI_INT, MPI_PROD, 0, MPI_COMM_WORLD);
    ASSERT_EQ(sum, std::pow(value, size));

    MPI_Barrier(MPI_COMM_WORLD);
}
TEST(Allreduce, test_min) {
    int rank = 0;
    int size = 0;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int value = 2;
    int sum = 0;
    if (rank == 0)
        value = 1;
    My_AllReduce(&value, &sum, 1, MPI_INT, MPI_MIN, 0, MPI_COMM_WORLD);
    ASSERT_EQ(sum, 1);

    MPI_Barrier(MPI_COMM_WORLD);
}


int main(int argc, char** argv) {
    int result_code = 0;
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::TestEventListeners& listeners =
        ::testing::UnitTest::GetInstance()->listeners();

    if (MPI_Init(&argc, &argv) != MPI_SUCCESS)
        MPI_Abort(MPI_COMM_WORLD, -1);
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank != 0) {
        delete listeners.Release(listeners.default_result_printer());
    }
    result_code = RUN_ALL_TESTS();
    MPI_Finalize();

    return result_code;
}
