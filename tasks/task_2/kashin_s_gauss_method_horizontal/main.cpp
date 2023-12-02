// Copyright 2023 Kashin Stepa
#include <gtest/gtest.h>
#include "task_2/kashin_s_gauss_method_horizontal/gauss_method_horizontal.h"

TEST(gauss_method_horizontal, Test_simple_matrix) {
    int rankProc = 0;
    int numProc = 0;

    MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);
    MPI_Comm_size(MPI_COMM_WORLD, &numProc);
    int n = 3;
    double* A = new double[]{
            2, 1, -1, 8,
            -3, -1, 2, -11,
            -2, 1, 2, -3
    };
    gaussianEliminationPAR(A,n,numProc,rankProc);
    if (rankProc == 0) {
        std::vector<double> result = {2, 3, -1};
        std::vector<double> answer = {A[3],A[7],A[11]};
        ASSERT_EQ(result, getResult(A,n));
    }
    delete[] A;
}



TEST(gauss_method_horizontal, Test_ten_elems_from_0_to_99) {
    int rankProc = 0;
    int numProc = 0;

    MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);
    MPI_Comm_size(MPI_COMM_WORLD, &numProc);

    int n = 10;
    double* matrix;
    double* ans;
    if (rankProc == 0) {
        matrix = new double[(n * (n + 1))];
        generateGaussianMatrix(matrix,n,100);
        ans = new double[(n * (n + 1))];
        std::copy(matrix, matrix + (n * (n + 1)), ans);
    }
    gaussianEliminationPAR(matrix,n,numProc,rankProc);

    if (rankProc == 0) {
        gaussianElimination(ans,n);
        ASSERT_EQ(getResult(ans,n),getResult(matrix,n));
        delete[] matrix;
        delete[] ans;
    }
}

TEST(gauss_method_horizontal, Test_one_hundreds_from_0_to_9999) {
    int rankProc = 0;
    int numProc = 0;

    MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);
    MPI_Comm_size(MPI_COMM_WORLD, &numProc);

    int n = 100;
    double* matrix;
    double* ans;
    if (rankProc == 0) {
        matrix = new double[(n * (n + 1))];
        generateGaussianMatrix(matrix,n,10000);
        ans = new double[(n * (n + 1))];
        std::copy(matrix, matrix + (n * (n + 1)), ans);
    }
    gaussianEliminationPAR(matrix,n,numProc,rankProc);

    if (rankProc == 0) {
        gaussianElimination(ans,n);
        ASSERT_EQ(getResult(ans,n),getResult(matrix,n));
        delete[] matrix;
        delete[] ans;
    }
}

TEST(gauss_method_horizontal, Test_one_thousand_from_0_to_99) {
    int rankProc = 0;
    int numProc = 0;

    MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);
    MPI_Comm_size(MPI_COMM_WORLD, &numProc);

    int n = 1000;
    double* matrix;
    double* ans;
    if (rankProc == 0) {
        matrix = new double[(n * (n + 1))];
        generateGaussianMatrix(matrix,n,100);
        ans = new double[(n * (n + 1))];
        std::copy(matrix, matrix + (n * (n + 1)), ans);
    }
    gaussianEliminationPAR(matrix,n,numProc,rankProc);

    if (rankProc == 0) {
        gaussianElimination(ans,n);
        ASSERT_EQ(getResult(ans,n),getResult(matrix,n));
        delete[] matrix;
        delete[] ans;
    }
}

TEST(gauss_method_horizontal, Test_one_thousand_from_0_to_9999) {
    int rankProc = 0;
    int numProc = 0;

    MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);
    MPI_Comm_size(MPI_COMM_WORLD, &numProc);

    int n = 1000;
    double* matrix;
    double* ans;
    if (rankProc == 0) {
        matrix = new double[(n * (n + 1))];
        generateGaussianMatrix(matrix,n,10000);
        ans = new double[(n * (n + 1))];
        std::copy(matrix, matrix + (n * (n + 1)), ans);
    }
    gaussianEliminationPAR(matrix,n,numProc,rankProc);

    if (rankProc == 0) {
        gaussianElimination(ans,n);
        ASSERT_EQ(getResult(ans,n),getResult(matrix,n));
        delete[] matrix;
        delete[] ans;
    }
}

int main(int argc, char **argv) {
   int result_code = 0;

   ::testing::InitGoogleTest(&argc, argv);
   ::testing::TestEventListeners &listeners = ::testing::UnitTest::GetInstance()->listeners();

   if (MPI_Init(&argc, &argv) != MPI_SUCCESS)
       MPI_Abort(MPI_COMM_WORLD, -1);
   result_code = RUN_ALL_TESTS();
   MPI_Finalize();

   return result_code;
}
