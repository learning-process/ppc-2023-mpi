// Copyright 2023 Ionova Ekaterina
#include <gtest/gtest.h>

#include "./int_rect.h"

TEST(TESTS, Test_1) {
  int rank = 0;
  MPI_Comm comm = MPI_COMM_WORLD;
  MPI_Comm_rank(comm, &rank);

  double a = 10.5;
  double b = 20.9;
  double epsilon = 0.01;
  uint32_t n = 1'000'000;
  auto func = [](double x) { return x; };

  double par_result = parIntegrationRectangle(a, b, n, func, comm);

  if (rank == 0) {
    double seq_result = seqIntegrationRectangle(a, b, n, func);
    ASSERT_TRUE(isEqual(seq_result, par_result, epsilon));
  }
}

TEST(TESTS, Test_2) {
  int rank = 0;
  MPI_Comm comm = MPI_COMM_WORLD;
  MPI_Comm_rank(comm, &rank);

  double a = 10.5;
  double b = 20.9;
  double epsilon = 0.01;
  uint32_t n = 1'000'000;
  auto func = [](double x) { return x * x; };

  double par_result = parIntegrationRectangle(a, b, n, func, comm);

  if (rank == 0) {
    double seq_result = seqIntegrationRectangle(a, b, n, func);
    ASSERT_TRUE(isEqual(seq_result, par_result, epsilon));
  }
}

TEST(TESTS, Test_3) {
  int rank = 0;
  MPI_Comm comm = MPI_COMM_WORLD;
  MPI_Comm_rank(comm, &rank);

  double a = 10.5;
  double b = 20.9;
  double epsilon = 0.01;
  uint32_t n = 1'000'000;
  auto func = [](double x) { return sin(x); };

  double par_result = parIntegrationRectangle(a, b, n, func, comm);

  if (rank == 0) {
    double seq_result = seqIntegrationRectangle(a, b, n, func);
    ASSERT_TRUE(isEqual(seq_result, par_result, epsilon));
  }
}

TEST(TESTS, Test_4) {
  int rank = 0;
  MPI_Comm comm = MPI_COMM_WORLD;
  MPI_Comm_rank(comm, &rank);

  double a = 10.5;
  double b = 20.9;
  double epsilon = 0.01;
  uint32_t n = 1'000'000;
  auto func = [](double x) { return x * cos(x); };

  double par_result = parIntegrationRectangle(a, b, n, func, comm);

  if (rank == 0) {
    double seq_result = seqIntegrationRectangle(a, b, n, func);
    ASSERT_TRUE(isEqual(seq_result, par_result, epsilon));
  }
}

TEST(TESTS, Test_5) {
  int rank = 0;
  MPI_Comm comm = MPI_COMM_WORLD;
  MPI_Comm_rank(comm, &rank);

  double a = 10.5;
  double b = 20.9;
  double epsilon = 0.01;
  uint32_t n = 1'000'000;
  auto func = [](double x) { return pow(sin(x), 2.0) + pow(cos(x), 2.0); };

  double par_result = parIntegrationRectangle(a, b, n, func, comm);

  if (rank == 0) {
    double seq_result = seqIntegrationRectangle(a, b, n, func);
    ASSERT_TRUE(isEqual(seq_result, par_result, epsilon));
  }
}

int main(int argc, char** argv) {
  int result_code = 0;
  int rank = 0;

  ::testing::InitGoogleTest(&argc, argv);
  ::testing::TestEventListeners& listeners =
      ::testing::UnitTest::GetInstance()->listeners();

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if (rank != 0) {
    delete listeners.Release(listeners.default_result_printer());
  }

  result_code = RUN_ALL_TESTS();
  MPI_Finalize();

  return result_code;
}
