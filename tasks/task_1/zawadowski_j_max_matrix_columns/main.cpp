// Copyright 2023 Zawadowski Jan
#include <gtest/gtest.h>
#include "./max_matrix_columns.h"

TEST(Max_Matrix_Columns, Test_find_max_in_given_matrix) {
    const int ROWS = 3,
              COLS = 4;
    std::vector<int> matrix = { 8, 14, 6, 8, 4, 3, 9, 3, 11, 5, 1, 8 },
                     maxValues,
                     maxValuesCon;
    int rank = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    maxValues = findMaxByMatrixColumns(matrix, ROWS, COLS);
    if (rank == 0) {
        maxValuesCon = findMaxByMatrixColumnsConsistently(matrix, ROWS, COLS);
        ASSERT_EQ(maxValues, maxValuesCon);
    }
}

TEST(Max_Matrix_Columns, Test_find_max_in_random_matrix) {
    const int ROWS = 3,
              COLS = 4,
              MIN = 0,
              MAX = 20;
    std::vector<int> matrix = fillMatrixRandom(ROWS, COLS, MIN, MAX),
                     maxValues,
                     maxValuesCon;
    int rank = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    maxValues = findMaxByMatrixColumns(matrix, ROWS, COLS);
    if (rank == 0) {
        maxValuesCon = findMaxByMatrixColumnsConsistently(matrix, ROWS, COLS);
        ASSERT_EQ(maxValues, maxValuesCon);
    }
}

TEST(Max_Matrix_Columns, Test_matrix_all_zero) {
    const int ROWS = 2,
              COLS = 3;
    std::vector<int> matrix = { 0, 0, 0, 0, 0, 0 },
                     maxValues,
                     maxValuesCon = { 0, 0, 0 };
    int rank = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    maxValues = findMaxByMatrixColumns(matrix, ROWS, COLS);
    if (rank == 0)
        ASSERT_EQ(maxValues, maxValuesCon);
}

TEST(Max_Matrix_Columns, Test_matrix_all_negative) {
    const int ROWS = 2,
              COLS = 3,
              MIN = -20,
              MAX = -1;
    std::vector<int> matrix = fillMatrixRandom(ROWS, COLS, MIN, MAX),
                     maxValues,
                     maxValuesCon;
    int rank = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    maxValues = findMaxByMatrixColumns(matrix, ROWS, COLS);
    if (rank == 0) {
        maxValuesCon = findMaxByMatrixColumnsConsistently(matrix, ROWS, COLS);
        ASSERT_EQ(maxValues, maxValuesCon);
    }
}

TEST(Max_Matrix_Columns, Test_matirx_one_number) {
    const int ROWS = 1,
              COLS = 1,
              MIN = 0,
              MAX = 9;
    std::vector<int> matrix = fillMatrixRandom(ROWS, COLS, MIN, MAX),
                     maxValues,
                     maxValuesCon;
    int rank = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    maxValues = findMaxByMatrixColumns(matrix, ROWS, COLS);
    if (rank == 0) {
        maxValuesCon = findMaxByMatrixColumnsConsistently(matrix, ROWS, COLS);
        ASSERT_EQ(maxValues, maxValuesCon);
    }
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
