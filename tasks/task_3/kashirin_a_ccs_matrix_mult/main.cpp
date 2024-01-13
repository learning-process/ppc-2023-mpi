// Copyright 2023 Kashirin Alexander
#include <gtest/gtest.h>
#include <mpi.h>
#include <cstdlib>
#include "./ccs_matrix_mult.h"


TEST(TEST_CCS, Test_constructor) {
    int rank;
    int numProc;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &numProc);
    double matrix1[] = { 1, 0,
                      0, 1 };
    ClassMatrix matrix2(matrix1, 2, 2);
    double elems[] = { 1, 1 };
    int rows[] = { 0, 1 };
    int columns[] = { 0, 1, 2 };
    if (rank == 0) {
        for (int i = 0; i < 2; i++) {
            EXPECT_EQ(elems[i], matrix2.ccsMatrix.Values[i]);
            EXPECT_EQ(rows[i], matrix2.ccsMatrix.RowInd[i]);
        }
        for (int j = 0; j < 3; j++) {
            EXPECT_EQ(columns[j], matrix2.ccsMatrix.ColPtr[j]);
        }
    }
}

TEST(TEST_CCS, Test_mult1) {
    int rank;
    int numProc;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &numProc);


    double m1[] = {1, 0, 0,
                  0, 1, 0,
                  0, 0, 1 };
    ClassMatrix matrix1(m1, 3, 3);
    double m2[] = {1, 0, 0,
                  0, 1, 0,
                  0, 0, 1 };
    ClassMatrix matrix2(m2, 3, 3);
    ClassMatrix matrix3;
    matrix3 = matrix1 * matrix2;

    double elems[] = { 1, 1, 1 };
    int rows[] = { 0, 1, 2 };
    int columns[] = { 0, 1, 2, 3 };

    if (rank == 0) {
        for (int i = 0; i < 3; i++) {
            EXPECT_EQ(elems[i], matrix3.ccsMatrix.Values[i]);
            EXPECT_EQ(rows[i], matrix3.ccsMatrix.RowInd[i]);
        }
        for (int j = 0; j < 4; j++) {
            EXPECT_EQ(columns[j], matrix3.ccsMatrix.ColPtr[j]);
        }
    }
}

TEST(TEST_CCS, Test_mult2) {
    int rank;
    int numProc;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &numProc);


    double m1[] = {1, 0, 0,
                  2, 1, 0,
                  3, 2, 1 };
    ClassMatrix matrix1(m1, 3, 3);
    double m2[] = {1, 2, 3,
                  0, 1, 2,
                  0, 0, 1 };
    ClassMatrix matrix2(m2, 3, 3);
    ClassMatrix matrix3;
    matrix3 = matrix1 * matrix2;

    double elems[] = { 1, 2, 3, 2, 5, 8, 3, 8, 14 };
    int rows[] = { 0, 1, 2, 0, 1, 2, 0, 1, 2 };
    int columns[] = { 0, 3, 6, 9 };

    if (rank == 0) {
        for (int i = 0; i < 9; i++) {
            EXPECT_EQ(elems[i], matrix3.ccsMatrix.Values[i]);
            EXPECT_EQ(rows[i], matrix3.ccsMatrix.RowInd[i]);
        }
        for (int j = 0; j < 4; j++) {
            EXPECT_EQ(columns[j], matrix3.ccsMatrix.ColPtr[j]);
        }
    }
}

TEST(TEST_CCS, Test_parallel1) {
    int rank;
    int numProc;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &numProc);


    double m1[] = { 1, 0, 0,
                  2, 1, 0,
                  3, 2, 1 };
    ClassMatrix matrix1(m1, 3, 3);
    double m2[] = { 1, 2, 3,
                  0, 1, 2,
                  0, 0, 1 };
    ClassMatrix matrix2(m2, 3, 3);
    ClassMatrix matrix3;
    matrix3 = matrix1 * matrix2;
    ClassMatrix matrix4;
    matrix4 = MultiplyCCS(&matrix1, &matrix2);
    if (rank == 0) {
        for (int i = 0; i < 9; i++) {
            EXPECT_EQ(matrix4.ccsMatrix.Values[i], matrix3.ccsMatrix.Values[i]);
            EXPECT_EQ(matrix4.ccsMatrix.RowInd[i], matrix3.ccsMatrix.RowInd[i]);
        }
        for (int j = 0; j < 4; j++) {
            EXPECT_EQ(matrix4.ccsMatrix.ColPtr[j], matrix3.ccsMatrix.ColPtr[j]);
        }
    }
}

TEST(TEST_CCS, Test_parallel2) {
    int rank;
    int numProc;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &numProc);

    double m1[] = { 1, 0, 4, 0,
                 0, 1, 4, 4,
                 4, 4, 1, 0,
                 0, 4, 0, 1 };
    ClassMatrix matrix1(m1, 4, 4);
    double m2[] = { 1, 2, 3, 4,
                    2, 1, 2, 3,
                    3, 2, 1, 2,
                    4, 3, 2, 1};
    ClassMatrix matrix2(m2, 4, 4);
    ClassMatrix matrix3;
    matrix3 = matrix1 * matrix2;
    ClassMatrix matrix4;
    matrix4 = MultiplyCCS(&matrix1, &matrix2);
    if (rank == 0) {
        for (int i = 0; i < 16; i++) {
            EXPECT_EQ(matrix4.ccsMatrix.Values[i], matrix3.ccsMatrix.Values[i]);
            EXPECT_EQ(matrix4.ccsMatrix.RowInd[i], matrix3.ccsMatrix.RowInd[i]);
        }
        for (int j = 0; j < 5; j++) {
            EXPECT_EQ(matrix4.ccsMatrix.ColPtr[j], matrix3.ccsMatrix.ColPtr[j]);
        }
    }
}


int main(int argc, char** argv) {
    int result_code = 0;

    ::testing::InitGoogleTest(&argc, argv);
    ::testing::TestEventListeners& listeners =
        ::testing::UnitTest::GetInstance()->listeners();

    if (MPI_Init(&argc, &argv) != MPI_SUCCESS)
        MPI_Abort(MPI_COMM_WORLD, -1);
    result_code = RUN_ALL_TESTS();
    MPI_Finalize();

    return result_code;
}
