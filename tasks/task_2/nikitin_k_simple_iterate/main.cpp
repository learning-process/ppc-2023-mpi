// Copyright 2023 Nikitin Kirill
#include <gtest/gtest.h>
#include "task_2/nikitin_k_simple_iterate/simple_iterate.h"

TEST(TESTS, Test_1) {
  int rank = 0;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  std::vector<double> a = {4, -1, -1, 4};
  std::vector<double> b = {5, 5};
  double epsilon = 0.001;

  auto par = SimpleIterPar(a, b, epsilon, MPI_COMM_WORLD);

  if (rank == 0) {
    auto seq = SimpleIterSeq(a, b, epsilon);
    ASSERT_EQ(par, seq);
  }
}

TEST(TESTS, Test_2) {
  int rank = 0;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  std::vector<double> a = {4, -1, 0, -1, 4, -1, 0, -1, 4};
  std::vector<double> b = {5, 5, 10};
  double epsilon = 0.001;

  auto par = SimpleIterPar(a, b, epsilon, MPI_COMM_WORLD);

  if (rank == 0) {
    auto seq = SimpleIterSeq(a, b, epsilon);
    ASSERT_EQ(par, seq);
  }
}

TEST(TESTS, Test_3) {
  int rank = 0;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  std::vector<double> a = {10, 1, 2, 3, 1, 10, 2, 3, 1, 0, 10, 2, 2, 0, 0, 10};
  std::vector<double> b = {12, 3, 11, 2};
  double epsilon = 0.001;

  auto par = SimpleIterPar(a, b, epsilon, MPI_COMM_WORLD);

  if (rank == 0) {
    auto seq = SimpleIterSeq(a, b, epsilon);
    ASSERT_EQ(par, seq);
  }
}

TEST(TESTS, Test_empty) {
  int rank = 0;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  std::vector<double> a{};
  std::vector<double> b{};
  double epsilon = 0.001;

  auto par = SimpleIterPar(a, b, epsilon, MPI_COMM_WORLD);

  if (rank == 0) {
    auto seq = SimpleIterSeq(a, b, epsilon);
    ASSERT_EQ(par, seq);
  }
}

TEST(TESTS, Test_5) {
  int rank = 0;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  std::vector<double> a{5, 1, 2, 1, 1, 15, 1, 3, 4, 1, 9, 1, 1, -1, 1, 11};
  std::vector<double> b{9, 19, 6, 10};
  double epsilon = 0.001;

  auto par = SimpleIterPar(a, b, epsilon, MPI_COMM_WORLD);

  if (rank == 0) {
    auto seq = SimpleIterSeq(a, b, epsilon);
    ASSERT_EQ(par, seq);
  }
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  ::testing::TestEventListeners& listeners =
      ::testing::UnitTest::GetInstance()->listeners();

  int rank = 0;
  int exit_code = 0;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if (rank != 0) {
    delete listeners.Release(listeners.default_result_printer());
  }

  exit_code = RUN_ALL_TESTS();
  MPI_Finalize();

  return exit_code;
}
