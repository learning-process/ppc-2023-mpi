// Copyright 2023 Kuznetsov Artem
#include <gtest/gtest.h>
#include <mpi.h>

#include <iostream>

#include "./linear_topology.h"

TEST(MPI_TESTS, Test_inc) {
  int rank = 0;
  int size_world = 0;

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size_world);

  const int sender = 0;
  const int receiver = size_world - 1;
  int data = 0;

  if (rank == sender) data = 500;

  send_data_linear(&data, 1, MPI_INT, sender, receiver, 0, MPI_COMM_WORLD);

  if (rank == receiver) {
    ASSERT_EQ(500, data);
  }
}

TEST(MPI_TESTS, Test_dec) {
  int rank = 0;
  int size_world = 0;

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size_world);

  const int sender = size_world - 1;
  const int receiver = 0;
  int data = 0;

  if (rank == sender) data = 500;

  send_data_linear(&data, 1, MPI_INT, sender, receiver, 0, MPI_COMM_WORLD);

  if (rank == receiver) {
    ASSERT_EQ(500, data);
  }
}

TEST(MPI_TESTS, Test_two_processors) {
  int rank = 0;
  int size_world = 0;

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size_world);

  const int sender = 0;
  const int receiver = 1;
  int data = 0;

  if (rank == sender) data = 500;

  send_data_linear(&data, 1, MPI_INT, sender, receiver, 0, MPI_COMM_WORLD);

  if (rank == receiver) {
    ASSERT_EQ(500, data);
  }
}

TEST(MPI_TESTS, Test_middle_send_inc) {
  int rank = 0;
  int size_world = 0;

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size_world);

  const int sender = size_world / 2;
  const int receiver = size_world - 1;
  int data = 0;

  if (rank == sender) data = 500;

  send_data_linear(&data, 1, MPI_INT, sender, receiver, 0, MPI_COMM_WORLD);

  if (rank == receiver) {
    ASSERT_EQ(500, data);
  }
}

TEST(MPI_TESTS, Test_middle_send_dec) {
  int rank = 0;
  int size_world = 0;

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size_world);

  const int sender = size_world / 2;
  const int receiver = 0;
  int data = 0;

  if (rank == sender) data = 500;

  send_data_linear(&data, 1, MPI_INT, sender, receiver, 0, MPI_COMM_WORLD);

  if (rank == receiver) {
    ASSERT_EQ(500, data);
  }
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
