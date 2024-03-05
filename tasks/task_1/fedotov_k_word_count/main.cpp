// Copyright 2023 Fedotov Kirill
#include <gtest/gtest.h>
#include <mpi.h>
#include <string>
#include "task_1/fedotov_k_word_count/word_count.h"

TEST(word_count, sequentalCount) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if (rank == 0) {
    std::string st = "MPI is live ";
    int res = getLinearCount(st, st.size());
    ASSERT_EQ(res, 3);
  }
}

TEST(word_count, isLetter_with_capital_letter) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  std::string st = "MPI is live ";
  if (rank == 0) {
    bool res = isLetter('A');
    ASSERT_EQ(res, true);
  }
}

TEST(word_count, isLetter_with_letter) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  std::string st = "MPI is live ";
  if (rank == 0) {
    bool res = isLetter('a');
    ASSERT_EQ(res, true);
  }
}

TEST(word_count, isLetter_with_another_symbol) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  std::string st = "MPI is live ";
  if (rank == 0) {
    bool res = isLetter(';');
    ASSERT_EQ(res, false);
  }
}

TEST(word_count, parallelCount) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::string st;
  if (rank == 0) {
    randWord(&st, 99);
  }
  int res = getCount(st);

  if (rank == 0) {
    ASSERT_EQ(res, 99);
  }
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  MPI_Init(&argc, &argv);

  ::testing::AddGlobalTestEnvironment(new GTestMPIListener::MPIEnvironment);
  ::testing::TestEventListeners& listeners =
      ::testing::UnitTest::GetInstance()->listeners();

  listeners.Release(listeners.default_result_printer());
  listeners.Release(listeners.default_xml_generator());

  listeners.Append(new GTestMPIListener::MPIMinimalistPrinter);
  return RUN_ALL_TESTS();
}
