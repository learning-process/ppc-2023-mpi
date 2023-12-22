// Copyright 2023 Kalinin Alexandr
#include <gtest/gtest.h>
#include "task_2/kalinin_a_gorA_verB/gorA_verB.h"

TEST(Parallel_Operations_MPI, square_matrix) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int row_count = 20, column_count = 20;
    std::vector<int> matrix1(row_count * column_count),
        matrix2(row_count * column_count);

    if (rank == 0) {
        matrix1 = getRandomMatrix(row_count, column_count);
        matrix2 = getRandomMatrix(column_count, row_count);
    }

    std::vector<int> res =
        getParallelOperations(matrix1, matrix2,
            row_count, column_count);

    if (rank == 0) {
        std::vector<int> expected_res =
            getSequentialOperations(matrix1, matrix2, row_count,
                column_count, row_count);

        ASSERT_EQ(res, expected_res);
    }
}

TEST(Parallel_Operations_MPI, arbitary_matrix) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int row_count = 20, column_count = 30;
    std::vector<int> matrix1(row_count * column_count),
        matrix2(row_count * column_count);

    if (rank == 0) {
        matrix1 = getRandomMatrix(row_count, column_count);
        matrix2 = getRandomMatrix(column_count, row_count);
    }

    ASSERT_NO_THROW(getParallelOperations(matrix1,
        matrix2, row_count, column_count));
}

TEST(Parallel_Operations_MPI, more_columns) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int>::size_type row_count = 20, column_count = 30;
    std::vector<int> matrix1(row_count * column_count),
        matrix2(row_count * column_count);

    if (rank == 0) {
        matrix1 = getRandomMatrix(row_count, column_count);
        matrix2 = getRandomMatrix(column_count, row_count);
    }

    std::vector<int> res = getParallelOperations(matrix1,
        matrix2, row_count, column_count);

    if (rank == 0) {
        std::vector<int> expected_res =
            getSequentialOperations(matrix1, matrix2, row_count,
                column_count, row_count);

        ASSERT_EQ(res, expected_res);
    }
}
TEST(Parallel_Operations_MPI, odd_size_and_more_rows) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int row_count = 27, column_count = 15;
    std::vector<int> matrix1(row_count * column_count),
        matrix2(row_count * column_count);

    if (rank == 0) {
        matrix1 = getRandomMatrix(row_count, column_count);
        matrix2 = getRandomMatrix(column_count, row_count);
    }

    std::vector<int> res = getParallelOperations(matrix1,
        matrix2, row_count, column_count);

    if (rank == 0) {
        std::vector<int> expected_res =
            getSequentialOperations(matrix1, matrix2,
                row_count, column_count, row_count);

        ASSERT_EQ(res, expected_res);
    }
}

TEST(Parallel_Operations_MPI, odd_size_and_more_columns) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int row_count = 15, column_count = 27;
    std::vector<int> matrix1(row_count * column_count),
        matrix2(row_count * column_count);

    if (rank == 0) {
        matrix1 = getRandomMatrix(row_count, column_count);
        matrix2 = getRandomMatrix(column_count, row_count);
    }

    std::vector<int> res = getParallelOperations(matrix1,
        matrix2, row_count, column_count);

    if (rank == 0) {
        std::vector<int> expected_res =
            getSequentialOperations(matrix1, matrix2,
                row_count, column_count, row_count);

        ASSERT_EQ(res, expected_res);
    }
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
