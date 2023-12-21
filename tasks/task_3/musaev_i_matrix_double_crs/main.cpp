// Copyright 2023 Musaev Ilgar

#include <gtest/gtest.h>
#include <vector>
#include "./matrix_double_crs.h"

TEST(Multyplicate_crs_matrix, crs_matrix_row_index_test_1) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    crsMatrix* A = new crsMatrix;
    crsMatrix* B = new crsMatrix;
    crsMatrix* C = new crsMatrix;
    crsMatrix* Ctest = new crsMatrix;
    int NZ = 2;  // not null elem
    int N = 30;
    if (rank == 0) {
        GenerateRegularCRS(N, NZ, A);
        GenerateRegularCRS(N, NZ, B);
    }
    MultiplicateMPI(A, B, C);
    if (rank == 0) {
        MultiplicateGustafson(*A, *B, Ctest);
        for (int i = 0; i < N + 1; i++) {
            ASSERT_EQ(C->RowIndex[i], Ctest->RowIndex[i]);
        }
    }
}

TEST(Multyplicate_crs_matrix, crs_matrix_col_test_2) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    crsMatrix* A = new crsMatrix;
    crsMatrix* B = new crsMatrix;
    crsMatrix* C = new crsMatrix;
    crsMatrix* Ctest = new crsMatrix;
    int NZ = 2;
    int N = 30;
    if (rank == 0) {
        GenerateRegularCRS(N, NZ, A);
        GenerateRegularCRS(N, NZ, B);
    }
    MultiplicateMPI(A, B, C);
    if (rank == 0) {
        MultiplicateGustafson(*A, *B, Ctest);
        for (int i = 0; i < C->RowIndex[N]; i++) {
            ASSERT_EQ(C->Col[i], Ctest->Col[i]);
        }
    }
}

TEST(Multyplicate_crs_matrix, crs_matrix_value_test_3) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    crsMatrix* A = new crsMatrix;
    crsMatrix* B = new crsMatrix;
    crsMatrix* C = new crsMatrix;
    crsMatrix* Ctest = new crsMatrix;
    int NZ = 2;
    int N = 30;
    if (rank == 0) {
        GenerateRegularCRS(N, NZ, A);
        GenerateRegularCRS(N, NZ, B);
    }
    MultiplicateMPI(A, B, C);
    if (rank == 0) {
        MultiplicateGustafson(*A, *B, Ctest);
        for (int i = 0; i < C->RowIndex[N]; i++) {
            ASSERT_EQ(C->Value[i], Ctest->Value[i]);
        }
    }
}

TEST(Multyplicate_crs_matrix, crs_matrix_one_row_index_test_4) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    crsMatrix* A = new crsMatrix;
    crsMatrix* B = new crsMatrix;
    crsMatrix* C = new crsMatrix;
    crsMatrix* Ctest = new crsMatrix;
    int NZ = 1;  // not null elem
    int N = 40;
    if (rank == 0) {
        GenerateRegularCRS(N, NZ, A);
        GenerateRegularCRS(N, NZ, B);
    }
    MultiplicateMPI(A, B, C);
    if (rank == 0) {
        MultiplicateGustafson(*A, *B, Ctest);
        for (int i = 0; i < N + 1; i++) {
            ASSERT_EQ(C->RowIndex[i], Ctest->RowIndex[i]);
        }
    }
}
TEST(Multyplicate_crs_matrix, crs_matrix_one_col_test_5) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    crsMatrix* A = new crsMatrix;
    crsMatrix* B = new crsMatrix;
    crsMatrix* C = new crsMatrix;
    crsMatrix* Ctest = new crsMatrix;
    int NZ = 1;
    int N = 40;
    if (rank == 0) {
        GenerateRegularCRS(N, NZ, A);
        GenerateRegularCRS(N, NZ, B);
    }
    MultiplicateMPI(A, B, C);
    if (rank == 0) {
        MultiplicateGustafson(*A, *B, Ctest);
        for (int i = 0; i < C->RowIndex[N]; i++) {
            ASSERT_EQ(C->Col[i], Ctest->Col[i]);
        }
    }
}

TEST(Multyplicate_crs_matrix, crs_matrix_one_value_test_6) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    crsMatrix* A = new crsMatrix;
    crsMatrix* B = new crsMatrix;
    crsMatrix* C = new crsMatrix;
    crsMatrix* Ctest = new crsMatrix;
    int NZ = 1;
    int N = 40;
    if (rank == 0) {
        GenerateRegularCRS(N, NZ, A);
        GenerateRegularCRS(N, NZ, B);
    }
    MultiplicateMPI(A, B, C);
    if (rank == 0) {
        MultiplicateGustafson(*A, *B, Ctest);
        for (int i = 0; i < C->RowIndex[N]; i++) {
            ASSERT_EQ(C->Value[i], Ctest->Value[i]);
        }
    }
}

int main(int argc, char** argv) {
  int result_code = 0;

  ::testing::InitGoogleTest(&argc, argv);
  ::testing::TestEventListeners& listeners =
      ::testing::UnitTest::GetInstance()->listeners();

  MPI_Init(&argc, &argv);
  result_code = RUN_ALL_TESTS();
  MPI_Finalize();

  return result_code;
}
