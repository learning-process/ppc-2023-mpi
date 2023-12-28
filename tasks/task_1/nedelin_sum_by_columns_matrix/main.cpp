// Copyright 2023 NEDELIN DMITRY

#include <mpi.h>
#include <gtest/gtest.h>
#include "../nedelin_sum_by_columns_matrix/sum_columns.h"

void SumOfColumnsTest(const int rows, const int cols) {
    int ProcId;
    std::vector<int> Matrix;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcId);
    if (ProcId == 0) {
        Matrix = CreateMatrix(rows, cols);
    }
    std::vector<int> SumPar = GetSumColsParallel(Matrix, rows, cols);
    if (ProcId == 0) {
        std::vector<int> SumLin = GetSumCols(Matrix, rows, cols);

        for (int i = 0; i < cols; i++) {
            ASSERT_EQ(SumPar.at(i), SumLin.at(i));
        }
    }
}

TEST(Matrix_Sum_Cols, Test1) {
    SumOfColumnsTest(5, 5);
}

TEST(Matrix_Sum_Cols, Test2) {
    SumOfColumnsTest(10, 10);
}

TEST(Matrix_Sum_Cols, Test3) {
    SumOfColumnsTest(100, 100);
}

TEST(Matrix_Sum_Cols, Test4) {
    SumOfColumnsTest(500, 500);
}

TEST(Matrix_Sum_Cols, Test5) {
    SumOfColumnsTest(1000, 1000);
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
