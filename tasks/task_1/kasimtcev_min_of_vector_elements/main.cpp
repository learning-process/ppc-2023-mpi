// Copyright 2023 Kasimtcev Roman
#include <gtest/gtest.h>
#include "task_1/kasimtcev_min_of_vector_elements/matrix_min.h"

TEST(Parallel_Operations_MPI, Square_Matrix_random) {
    std::vector<int> global_matrix;
    const int count_rows = 5000;
    const int count_cols = 5000;

    int proc_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);

    global_matrix = CreateRandomMatrix(count_rows, count_cols);

    MPI_Barrier(MPI_COMM_WORLD);
    int global_min = FndMinParallel(global_matrix);

    if (proc_rank == 0) {
        int min = FndMin(global_matrix);
        ASSERT_EQ(min, global_min);
    }
}

TEST(Parallel_Operations_MPI, Rectangular_matrix_random) {
    std::vector<int> global_matrix;
    const int count_rows = 2500;
    const int count_cols = 5000;
    int proc_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);
    global_matrix = CreateRandomMatrix(count_rows, count_cols);

    int global_min = FndMinParallel(global_matrix);

    if (proc_rank == 0) {
        int min = FndMin(global_matrix);
        ASSERT_EQ(min, global_min);
    }
}

TEST(Parallel_Operations_MPI, Square_matrix_with_equal_values) {
    const int count_rows = 5000;
    const int count_cols = 2500;
    int proc_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);

    std::vector<int> global_matrix(count_rows * count_cols, 3);

    int global_min = FndMinParallel(global_matrix);

    if (proc_rank == 0) {
        ASSERT_EQ(3, global_min);
    }
}

TEST(Parallel_Operations_MPI, Rectangular_matrix_with_equal_values) {
    const int count_rows = 1100;
    const int count_cols = 2400;
    int proc_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);

    std::vector<int> global_matrix(count_rows * count_cols, 4);

    int global_min = FndMinParallel(global_matrix);

    if (proc_rank == 0) {
        int min = FndMin(global_matrix);
        ASSERT_EQ(4, global_min);
    }
}

TEST(Parallel_Operations_MPI, Random_matrix_with_maximum_value_at_the_end) {
    std::vector<int> global_matrix;
    const int count_rows = 5600;
    const int count_cols = 700;

    int proc_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);

    global_matrix = CreateRandomMatrix(count_rows, count_cols);
    global_matrix[count_rows * count_cols - 1] = 1001;
    int global_min = FndMinParallel(global_matrix);

    if (proc_rank == 0) {
        int min = FndMin(global_matrix);
        ASSERT_EQ(min, global_min);
    }
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    int result_code = 0;
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::TestEventListeners& listeners =
      ::testing::UnitTest::GetInstance()->listeners();
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank != 0) {
        delete listeners.Release(listeners.default_result_printer());
    }
    result_code = RUN_ALL_TESTS();
    MPI_Finalize();
    return result_code;
}

