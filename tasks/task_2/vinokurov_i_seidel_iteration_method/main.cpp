// Copyright 2023 Vinokurov Ivan
#include <gtest/gtest.h>
#include <vector>
#include <iostream>
#include "task_2/vinokurov_i_seidel_iteration_method/seidel_iteration_method.h"

TEST(TESTS, CanUseFunctionTest) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<std::vector<double>> A = { {4, -1, 0, 0},
                                           {-1, 4, -1, 0},
                                           {0, -1, 4, -1},
                                           {0, 0, -1, 3} };
    std::vector<double> b = { 15, 10, 10, 10 };
    int numRows = 4;
    double eps = 0.0001;

    std::vector<double> result(numRows, 0.0);
    ASSERT_NO_THROW(result = funcSystemSolveSeidelMPI(A, b, numRows, eps));
}

TEST(TESTS, ConvergenceTest) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<std::vector<double>> A = { {4, -1, 0, 0},
                                           {-1, 4, -1, 0},
                                           {0, -1, 4, -1},
                                           {0, 0, -1, 3} }; 
    std::vector<double> b = { 15, 10, 10, 10 }; 
    int numRows = 4; 
    double eps = 0.0001; 
    
    std::vector<double> result = funcSystemSolveSeidelMPI(A, b, numRows, eps);

    ASSERT_NEAR(result[0], 5, 0.0001);
    ASSERT_NEAR(result[1], 5, 0.0001);
    ASSERT_NEAR(result[2], 5, 0.0001);
    ASSERT_NEAR(result[3], 5, 0.0001);
}

TEST(TESTS, ConvergenceTestLargeNumbers) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<std::vector<double>> A = { {100, -20, 3, -4},
                                           {-20, 120, 5, 6},
                                           {3, 5, 90, -7},
                                           {-4, 6, -7, 80} };
    std::vector<double> b = { 200, 300, 400, 500 };
    int numRows = 4;
    double eps = 0.001;

    std::vector<double> result = funcSystemSolveSeidelMPI(A, b, numRows, eps);
    ASSERT_NEAR(result[0], 2.603, 0.001);
    ASSERT_NEAR(result[1], 2.405, 0.001);
    ASSERT_NEAR(result[2], 4.738, 0.001);
    ASSERT_NEAR(result[3], 6.614, 0.001);
}

TEST(TESTS, ConvergenceTestNegativeNumbers) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<std::vector<double>> A = { {-4, -1, 0, 0},
                                           {-1, -4, -1, 0},
                                           {0, -1, -4, -1},
                                           {0, 0, -1, -3} };
    std::vector<double> b = { -15, -10, -10, -10 };
    int numRows = 4;
    double eps = 0.001;

    std::vector<double> result = funcSystemSolveSeidelMPI(A, b, numRows, eps);
    ASSERT_NEAR(result[0], 3.431, 0.001);
    ASSERT_NEAR(result[1], 1.274, 0.001);
    ASSERT_NEAR(result[2], 1.470, 0.001);
    ASSERT_NEAR(result[3], 2.844, 0.001);
}

TEST(TESTS, NotConvergentTest) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<std::vector<double>> A = { {-5, -4, -3, -2},
                                           {-3, -6, -7, -8},
                                           {-9, -10, -11, -12},
                                           {-13, -14, -15, -16} };
    std::vector<double> b = { -10, -20, -30, -40 };
    int numRows = 4;
    double eps = 0.001;

    std::vector<double> result = funcSystemSolveSeidelMPI(A, b, numRows, eps);
    // returns 0 if there are no roots for the system  
    ASSERT_NEAR(result[0], 0, 0.001);
    ASSERT_NEAR(result[1], 0, 0.001);
    ASSERT_NEAR(result[2], 0, 0.001);
    ASSERT_NEAR(result[3], 0, 0.001);
}

int main(int argc, char** argv) {
  MPI_Init(&argc, &argv);
  ::testing::InitGoogleTest(&argc, argv);
  int result = RUN_ALL_TESTS();
  MPI_Finalize();
  return result;
}
