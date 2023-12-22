// Copyright 2023 Volodin Evgeniy
#include <gtest/gtest.h>
#include "task_1/volodin_e_matrix_max/matrix_max.h"

TEST(Parallel_Operations_MPI, Square_Matrix_random) {
    std::vector<int> global_matrix;
    const int count_rows = 2000;
    const int count_columns = 2000;
    int proc_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);

    global_matrix = createRandomMatrix(count_rows, count_columns);

    MPI_Barrier(MPI_COMM_WORLD);
    int global_max = findMaxParallel(global_matrix);

    if (proc_rank == 0) {
        int max = findMax(global_matrix);
        ASSERT_EQ(max, global_max);
    }
}

TEST(Parallel_Operations_MPI, Rectangular_matrix_random) {
    std::vector<int> global_matrix;
    const int count_rows = 1000;
    const int count_columns = 2000;
    int proc_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);
    global_matrix = createRandomMatrix(count_rows, count_columns);

    int global_max = findMaxParallel(global_matrix);

    if (proc_rank == 0) {
        int max = findMax(global_matrix);
        ASSERT_EQ(max, global_max);
    }
}

TEST(Parallel_Operations_MPI, Square_matrix_with_equal_values) {
    const int count_rows = 2000;
    const int count_columns = 1000;
    int proc_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);

    std::vector<int> global_matrix(count_rows * count_columns, 3);

    int global_max = findMaxParallel(global_matrix);

    if (proc_rank == 0) {
        ASSERT_EQ(3, global_max);
    }
}

TEST(Parallel_Operations_MPI, Rectangular_matrix_with_equal_values) {
    const int count_rows = 2200;
    const int count_columns = 2400;
    int proc_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);

    std::vector<int> global_matrix(count_rows * count_columns, 4);

    int global_max = findMaxParallel(global_matrix);

    if (proc_rank == 0) {
        int max = findMax(global_matrix);
        ASSERT_EQ(4, global_max);
    }
}

TEST(Parallel_Operations_MPI, Random_matrix_with_maximum_value_at_the_end) {
    std::vector<int> global_matrix;
    const int count_rows = 4000;
    const int count_columns = 300;

    int proc_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);

    global_matrix = createRandomMatrix(count_rows, count_columns);
    global_matrix[count_rows * count_columns - 1] = 1001;
    int global_max = findMaxParallel(global_matrix);

    if (proc_rank == 0) {
        int max = findMax(global_matrix);
        ASSERT_EQ(max, global_max);
    }
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    int result_code = 0;
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::TestEventListeners& listeners = ::testing::UnitTest::GetInstance()->listeners();
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank != 0) {
        delete listeners.Release(listeners.default_result_printer());
    }
    result_code = RUN_ALL_TESTS();
    MPI_Finalize();
    return result_code;
}
