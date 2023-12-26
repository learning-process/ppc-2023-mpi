// Copyright 2023 Musaev Ilgar

#include <mpi.h>
#include <gtest/gtest.h>
#include <iostream>
#include <ctime>
#include <algorithm>
#include <random>
#include "./readers_writers.h"

TEST(Writers_Readers_Problem, writersCount_is_not_valid_is_negative) {
  int size, writersCount, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  writersCount = -1;
  if (rank == 0)
    std::cout << "writersCount is wrong. Cannot be < 0\n";
  ASSERT_ANY_THROW(startTask(writersCount));
}

TEST(Writers_Readers_Problem, writersCount_is_not_valid_is_bad) {
  int size, writersCount, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  writersCount = size;
  if (rank == 0)
    std::cout << "writersCount is wrong. Cannot be > then (size - 2)\n";
  ASSERT_ANY_THROW(startTask(writersCount));
}

TEST(Writers_Readers_Problem, writersCount_is_fine) {
  int size, writersCount, rankM;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  writersCount = 2;
  MPI_Comm_rank(MPI_COMM_WORLD, &rankM);
  if (writersCount < size - 2 && writersCount > 0) {
    ASSERT_EQ(startTask(writersCount), 0);
  } else {
    ASSERT_ANY_THROW(startTask(writersCount));
  }
}

TEST(Writers_Readers_Problem, writersCount_is_fine2) {
  int size, writersCount, rankM;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  writersCount = 3;
  MPI_Comm_rank(MPI_COMM_WORLD, &rankM);
  if (writersCount < size - 2 && writersCount > 0) {
    ASSERT_EQ(startTask(writersCount), 0);
  } else {
    ASSERT_ANY_THROW(startTask(writersCount));
  }
}

TEST(Writers_Readers_Problem, writersCount_is_fine3) {
  int size, writersCount, rankM;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  writersCount = 4;
  MPI_Comm_rank(MPI_COMM_WORLD, &rankM);
  if (writersCount < size - 2 && writersCount > 0) {
    ASSERT_EQ(startTask(writersCount), 0);
  } else {
    ASSERT_ANY_THROW(startTask(writersCount));
  }
}

TEST(Writers_Readers_Problem, writersCount_is_fine4) {
  int size, writersCount, rankM;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  writersCount = 10;
  MPI_Comm_rank(MPI_COMM_WORLD, &rankM);
  if (writersCount < size - 2 && writersCount > 0) {
    ASSERT_EQ(startTask(writersCount), 0);
  } else {
    ASSERT_ANY_THROW(startTask(writersCount));
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
