// Copyright 2023 Shmelev Ivan
#include <gtest/gtest.h>

#include "./max_of_matrix_elements.h"

TEST(TESTS, Test_1) {
  int rank = 0;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  std::vector<int> matrix{1, 2,   3,  4,  5,  6,  7,  8,
                          9, 100, 11, 12, 13, 14, 15, 16};
  int par = MaxInMatrixPar(matrix);

  if (rank == 0) {
    int seq = MaxInMatrixSeq(matrix);
    ASSERT_EQ(par, seq);
  }
}

TEST(TESTS, Test_only_zero) {
  int rank = 0;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  std::vector<int> matrix{0, 0, 0, 0, 0, 0, 0, 0, 0};
  int par = MaxInMatrixPar(matrix);

  if (rank == 0) {
    int seq = MaxInMatrixSeq(matrix);
    ASSERT_EQ(par, seq);
  }
}

TEST(TESTS, Test_one_elem) {
  int rank = 0;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  std::vector<int> matrix{100};
  int par = MaxInMatrixPar(matrix);

  if (rank == 0) {
    int seq = MaxInMatrixSeq(matrix);
    ASSERT_EQ(par, seq);
  }
}

TEST(TESTS, Test_zero_elem) {
  int rank = 0;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  std::vector<int> matrix{};
  int par = MaxInMatrixPar(matrix);

  if (rank == 0) {
    int seq = MaxInMatrixSeq(matrix);
    ASSERT_EQ(par, seq);
  }
}

TEST(TESTS, Test_rand) {
  int rank = 0;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  std::srand(std::time(NULL));
  int a = -10000;
  int b = 10000;
  size_t size = 1000000;
  std::vector<int> matrix(size);

  for (auto& val : matrix) val = std::rand() % (b - a + 1) + a;

  int par = MaxInMatrixPar(matrix);

  if (rank == 0) {
    int seq = MaxInMatrixSeq(matrix);
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
