// Copyright 2023 Ulyanov Daniil
#include <gtest/gtest.h>
#include <mpi.h>
#include <iostream>
#include "./star_topology.h"

TEST(star_topology, test_1) {
  int rank, num;

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &num);

  int data, send = 1, recv = 2, root = 0;

  if (num > 2) {
    if (rank == send) {
      data = 1234;
    }

    int ans = send_int(data, send, recv, root);

    if (rank == recv) {
      ASSERT_EQ(ans, 1234);
    }
  }
}

TEST(star_topology, test_2) {
  int rank, num;

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &num);

  int data, send = 1, recv = 1, root = 0;

  if (num > 1) {
    if (rank == send) {
      data = 1111;
    }

    int ans = send_int(data, send, recv, root);

    if (rank == recv) {
      ASSERT_EQ(ans, 1111);
    }
  }
}

TEST(star_topology, test_3) {
  int rank, num;

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &num);

  int data, send = 0, recv = 1, root = 0;

  if (num > 1) {
    if (rank == send) {
      data = 1000;
    }

    int ans = send_int(data, send, recv, root);

    if (rank == recv) {
      ASSERT_EQ(ans, 1000);
    }
  }
}

TEST(star_topology, test_4) {
  int rank, num;

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &num);

  int data, send = 1, recv = 0, root = 0;

  if (num > 1) {
    if (rank == send) {
      data = 4352;
    }

    int ans = send_int(data, send, recv, root);

    if (rank == recv) {
      ASSERT_EQ(ans, 4352);
    }
  }
}

TEST(star_topology, test_5) {
  int rank, num;

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &num);

  int data, send = 0, recv = 0, root = 0;

  if (rank == send) {
    data = 8348;
  }

  int ans = send_int(data, send, recv, root);

  if (rank == recv) {
    ASSERT_EQ(ans, 8348);
  }
}

int main(int argc, char **argv) {
  int result_code = 0;

  ::testing::InitGoogleTest(&argc, argv);
  ::testing::TestEventListeners& listeners =
    ::testing::UnitTest::GetInstance()->listeners();

  if (MPI_Init(&argc, &argv) != MPI_SUCCESS)
    MPI_Abort(MPI_COMM_WORLD, -1);
  result_code = RUN_ALL_TESTS();
  MPI_Finalize();

  return result_code;
}
