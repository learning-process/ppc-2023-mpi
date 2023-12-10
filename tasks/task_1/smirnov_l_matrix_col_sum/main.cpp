// Copyright 2023 Smirnov Leonid
#include <gtest/gtest.h>
#include <task_1/smirnov_l_matrix_col_sum/matrix_col_sum.h>
#include <iostream>
#include <utility>

TEST(ParallelMatrixColsSum, Test_detailed_equal_vectors_elements) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    size_t rows = 4;
    size_t cols = 6;
    std::vector<double> matrix;

    if (rank == 0) {
        matrix = {
            1.0, 2.0, 3.0, 4.0, 5.0, 6.0,
            1.0, 2.0, 3.0, 4.0, 5.0, 6.0,
            1.0, 2.0, 3.0, 4.0, 5.0, 6.0,
            1.0, 2.0, 3.0, 4.0, 5.0, 6.0
        };
        matrix = transposition(matrix, rows, cols);
    }

    auto tmp = rows;
    rows = cols;
    cols = tmp;

    std::vector<double> parallel_result = MatrixColsSumParallel(matrix, rows, cols);

    if (rank == 0) {
        std::vector<double> sequential_result = MatrixColsSumSequential(matrix, rows, cols);
        ASSERT_EQ(sequential_result, parallel_result);
    }
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    int result = 0;
    
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::TestEventListeners& listeners =
        ::testing::UnitTest::GetInstance()->listeners();
    
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank != 0) {
        delete listeners.Release(listeners.default_result_printer());
    }

    result = RUN_ALL_TESTS();
    MPI_Finalize();

    return result;
}
