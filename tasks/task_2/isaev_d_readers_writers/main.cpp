// Copyright 2023 Isaev Dmitriy
#include <gtest/gtest.h>
#include "./readers_writers.h"

TEST(Parallel_Readers_Writers, Test1) {
  int rank = 0;
  int sizeWorld = 0;

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &sizeWorld);
  ReadersWriters(0, 0);

  if (rank == 0) {
    ASSERT_NO_THROW();
  }
}

TEST(Parallel_Readers_Writers, Test2) {
  int rank = 0;
  int sizeWorld = 0;

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &sizeWorld);

  ReadersWriters(1, 0);

  if (rank == 0) {
    ASSERT_NO_THROW();
  }
}

TEST(Parallel_Readers_Writers, Test3) {
  int rank = 0;
  int sizeWorld = 0;

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &sizeWorld);

  ReadersWriters(0, 1);

  if (rank == 0) {
    ASSERT_NO_THROW();
  }
}

TEST(Parallel_Readers_Writers, Test4) {
  int rank = 0;
  int sizeWorld = 0;

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &sizeWorld);

  ReadersWriters(1, 1);

  if (rank == 0) {
    ASSERT_NO_THROW();
  }
}

TEST(Parallel_Readers_Writers, Test5) {
  int rank = 0;
  int sizeWorld = 0;

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &sizeWorld);

  ReadersWriters(2, 1);

  if (rank == 0) {
    ASSERT_NO_THROW();
  }
}

int main(int argc, char **argv) {
  int result_code = 0;

  ::testing::InitGoogleTest(&argc, argv);
  ::testing::TestEventListeners &listeners =
      ::testing::UnitTest::GetInstance()->listeners();

  if (MPI_Init(&argc, &argv) != MPI_SUCCESS) MPI_Abort(MPI_COMM_WORLD, -1);
  result_code = RUN_ALL_TESTS();
  MPI_Finalize();

  return result_code;
}
