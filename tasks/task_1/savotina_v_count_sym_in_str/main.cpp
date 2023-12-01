// Copyright 2023 Savotina Valeria
#include <gtest/gtest.h>

#include "./count_sym_in_str.h"

TEST(TESTS, Test_small_string) {
  MPI_Comm comm = MPI_COMM_WORLD;
  int rank = 0;
  MPI_Comm_rank(comm, &rank);

  std::string str{"Hello World!"};
  char sym = 'l';

  int parResult = parCountSymInString(str, sym, comm);

  if (rank == 0) {
    int seqResult = seqCountSymInString(str, sym);
    ASSERT_EQ(seqResult, parResult);
  }
}

TEST(TESTS, Test_count_is_zero) {
  MPI_Comm comm = MPI_COMM_WORLD;
  int rank = 0;
  MPI_Comm_rank(comm, &rank);

  std::string str{"010010010"};
  char sym = '2';

  int parResult = parCountSymInString(str, sym, comm);

  if (rank == 0) {
    int seqResult = seqCountSymInString(str, sym);
    ASSERT_EQ(seqResult, parResult);
  }
}

TEST(TESTS, Test_string_is_empty) {
  MPI_Comm comm = MPI_COMM_WORLD;
  int rank = 0;
  MPI_Comm_rank(comm, &rank);

  std::string str{};
  char sym = '4';

  int parResult = parCountSymInString(str, sym, comm);

  if (rank == 0) {
    int seqResult = seqCountSymInString(str, sym);
    ASSERT_EQ(seqResult, parResult);
  }
}

TEST(TESTS, Test_random_small) {
  MPI_Comm comm = MPI_COMM_WORLD;
  int rank = 0;
  MPI_Comm_rank(comm, &rank);

  size_t size = 10000;
  std::string str{createRandomString(size)};
  char sym = num_lim<char>::min() / 2;

  int parResult = parCountSymInString(str, sym, comm);

  if (rank == 0) {
    int seqResult = seqCountSymInString(str, sym);
    ASSERT_EQ(seqResult, parResult);
  }
}

TEST(TESTS, Test_random_large) {
  MPI_Comm comm = MPI_COMM_WORLD;
  int rank = 0;
  MPI_Comm_rank(comm, &rank);

  size_t size = 1'000'000;
  std::string str{createRandomString(size)};
  char sym = num_lim<char>::max() / 2;

  int parResult = parCountSymInString(str, sym, comm);

  if (rank == 0) {
    int seqResult = seqCountSymInString(str, sym);
    ASSERT_EQ(seqResult, parResult);
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
