// Copyright 2023 Savotina Valeria
#include <gtest/gtest.h>

#include "./par_configure_gradient.h"
#include "./seq_configure_gradient.h"

TEST(TESTS, Test_size_1) {
  int rank = 0;
  MPI_Comm comm = MPI_COMM_WORLD;
  MPI_Comm_rank(comm, &rank);

  std::vector<double> A{10};
  std::vector<double> b{12};

  auto parResult = parConfigureGradientMethod(b.size(), A, b, comm);

  if (rank == 0) {
    auto seqResult = seqConfigureGradientMethod(b.size(), A, b);
    ASSERT_TRUE(isEqualVectors(seqResult, parResult));
  }
}

TEST(TESTS, Test_size_2) {
  int rank = 0;
  MPI_Comm comm = MPI_COMM_WORLD;
  MPI_Comm_rank(comm, &rank);

  std::vector<double> A{10, 1, 2, 3};
  std::vector<double> b{12, 3};

  auto parResult = parConfigureGradientMethod(b.size(), A, b, comm);

  if (rank == 0) {
    auto seqResult = seqConfigureGradientMethod(b.size(), A, b);
    ASSERT_TRUE(isEqualVectors(seqResult, parResult));
  }
}

TEST(TESTS, Test_size_3) {
  int rank = 0;
  MPI_Comm comm = MPI_COMM_WORLD;
  MPI_Comm_rank(comm, &rank);

  std::vector<double> A{10, 1, 2, 3, 1, 10, 2, 3, 1};
  std::vector<double> b{12, 3, 11};

  auto parResult = parConfigureGradientMethod(b.size(), A, b, comm);

  if (rank == 0) {
    auto seqResult = seqConfigureGradientMethod(b.size(), A, b);
    ASSERT_TRUE(isEqualVectors(seqResult, parResult));
  }
}

TEST(TESTS, Test_size_4) {
  int rank = 0;
  MPI_Comm comm = MPI_COMM_WORLD;
  MPI_Comm_rank(comm, &rank);

  std::vector<double> A{10, 1, 2, 3, 1, 10, 2, 3, 1, 0, 10, 2, 2, 0, 0, 10};
  std::vector<double> b{12, 3, 11, 2};

  auto parResult = parConfigureGradientMethod(b.size(), A, b, comm);

  if (rank == 0) {
    auto seqResult = seqConfigureGradientMethod(b.size(), A, b);
    ASSERT_TRUE(isEqualVectors(seqResult, parResult));
  }
}

TEST(TESTS, Test_empty) {
  int rank = 0;
  MPI_Comm comm = MPI_COMM_WORLD;
  MPI_Comm_rank(comm, &rank);

  std::vector<double> A{};
  std::vector<double> b{};

  ASSERT_ANY_THROW(parConfigureGradientMethod(b.size(), A, b, comm));
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
