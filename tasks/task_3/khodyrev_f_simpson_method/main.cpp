// Copyright 2023 Khodyrev Fedor
#include <gtest/gtest.h>
#include <vector>
#include "./simpson_method.h"

TEST(Parallel_Operations_MPI, Test_One_Dimensional_Integral) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    std::vector<double> a = { 0.0 };
    std::vector<double> b = { 1.0 };
    int n = 100;

    double result = par_simpson_method(function_one_dimension, a, b, n);
    double result2 = 0.0;
    if (rank == 0) {
        result2 = seq_simpson_method(function_one_dimension, a, b, n);
    }
    EXPECT_LT(std::abs(result - result2), 0.01);
}

TEST(Parallel_Operations_MPI, Test_Two_Dimensional_Integral_With_First_Area) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    std::vector<double> a = { 1.0, 2.0 };
    std::vector<double> b = { 3.0, 5.0 };
    int n = 100;

    double result = par_simpson_method(function_two_dimension, a, b, n);
    double result2 = 0.0;
    if (rank == 0) {
        result2 = seq_simpson_method(function_two_dimension, a, b, n);
    }
    EXPECT_LT(std::abs(result - result2), 0.01);
}

TEST(Parallel_Operations_MPI, Test_Three_Dimensional_Integral_With_First_Area) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    std::vector<double> a = { 1.0, 2.0, 4.0 };
    std::vector<double> b = { 3.0, 4.0, 5.0 };
    int n = 100;

    double result = par_simpson_method(function_three_dimension, a, b, n);
    double result2 = 0.0;
    if (rank == 0) {
        result2 = seq_simpson_method(function_three_dimension, a, b, n);
    }
    EXPECT_LT(std::abs(result - result2), 0.01);
}

TEST(Parallel_Operations_MPI, Test_Two_Dimensional_Integral_With_Sec_Area) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    std::vector<double> a = { 1.0, 2.0 };
    std::vector<double> b = { 2.0, 4.0 };
    int n = 100;

    double result = par_simpson_method(function_two_dimension, a, b, n);
    double result2 = 0.0;
    if (rank == 0) {
        result2 = seq_simpson_method(function_two_dimension, a, b, n);
    }
    EXPECT_LT(std::abs(result - result2), 0.01);
}

TEST(Parallel_Operations_MPI, Test_Three_Dimensional_Integral_With_Sec_Area) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    std::vector<double> a = { 6.0, 1.0, 2.0 };
    std::vector<double> b = { 12.0, 43.0, 34.0 };
    int n = 100;

    double result = par_simpson_method(function_three_dimension, a, b, n);
    double result2 = 0.0;
    if (rank == 0) {
        result2 = seq_simpson_method(function_three_dimension, a, b, n);
    }
    EXPECT_LT(std::abs(result - result2), 0.01);
}

int main(int argc, char** argv) {
  int result_code = 0;

  ::testing::InitGoogleTest(&argc, argv);
  ::testing::TestEventListeners& listeners =
      ::testing::UnitTest::GetInstance()->listeners();

  if (MPI_Init(&argc, &argv) != MPI_SUCCESS) MPI_Abort(MPI_COMM_WORLD, -1);
  result_code = RUN_ALL_TESTS();
  MPI_Finalize();

  return result_code;
}
