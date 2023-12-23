// Copyright 2023 Khodyrev Fedor

#include <mpi.h>
#include <gtest/gtest.h>
#include <cmath>
#include <iostream>
#include "./monte_carlo_method.h"
#include <boost/mpi/environment.hpp>
#include <boost/mpi/communicator.hpp>

TEST(MPI_TESTS, Test_exponentional_function) {
  int rank = 0;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  double lower_limit = 0;
  double upper_limit = 1;
  int num_points = 10000;
  auto exponentional_function = [](double x) { return std::exp(-x * x); };
  double result_par = par_monte_carlo_integration(num_points, lower_limit,
  upper_limit, exponentional_function);
  if (rank == 0) {
    double result_seq = seq_monte_carlo_integration(num_points, lower_limit,
    upper_limit, exponentional_function);
    EXPECT_LT(std::fabs(result_seq - result_par), 0.1);
  }
}

TEST(MPI_TESTS, Test_exponentional_function_without_limit) {
  int rank = 0;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  double lower_limit = 0;
  double upper_limit = 0;
  int num_points = 10000;
  auto exponentional_function = [](double x) { return std::exp(-x * x); };
  double result_par = par_monte_carlo_integration(num_points, lower_limit,
  upper_limit, exponentional_function);
  if (rank == 0) {
    ASSERT_EQ(result_par, 0.0);
  }
}

TEST(MPI_TESTS, Test_quadratic_function) {
  int rank = 0;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  double lower_limit = 100;
  double upper_limit = 200;
  int num_points = 10000;
  auto quadratic_function = [](double x) { return x * x; };
  double result_par = par_monte_carlo_integration(num_points, lower_limit,
  upper_limit, quadratic_function);
  if (rank == 0) {
    double result_seq = seq_monte_carlo_integration(num_points, lower_limit,
    upper_limit, quadratic_function);
    EXPECT_LT(std::fabs(result_seq - result_par), 0.1);
  }
}

TEST(MPI_TESTS, Test_quadratic_function_without_limit) {
  int rank = 0;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  double lower_limit = 0;
  double upper_limit = 0;
  int num_points = 10000;
  auto quadratic_function = [](double x) { return x * x; };
  double result_par = par_monte_carlo_integration(num_points, lower_limit,
  upper_limit, quadratic_function);
  if (rank == 0) {
    ASSERT_EQ(result_par, 0.0);
  }
}

TEST(MPI_TESTS, Test_sine_function) {
  int rank = 0;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  double lower_limit = -1;
  double upper_limit = 1;
  int num_points = 10000;
  auto sine_function = [](double x) { return std::sin(x); };
  double result_par = par_monte_carlo_integration(num_points, lower_limit,
  upper_limit, sine_function);
  if (rank == 0) {
    double result_seq = seq_monte_carlo_integration(num_points, lower_limit,
    upper_limit, sine_function);
    EXPECT_LT(std::fabs(result_seq - result_par), 0.1);
  }
}

TEST(MPI_TESTS, Test_sine_function_without_limit) {
  int rank = 0;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  double lower_limit = 0;
  double upper_limit = 0;
  int num_points = 10000;
  auto sine_function = [](double x) { return std::sin(x); };
  double result_par = par_monte_carlo_integration(num_points, lower_limit,
  upper_limit, sine_function);
  if (rank == 0) {
    ASSERT_EQ(result_par, 0.0);
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
