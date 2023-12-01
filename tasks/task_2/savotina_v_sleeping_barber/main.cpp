// Copyright 2023 Savotina Valeria
#include <gtest/gtest.h>

#include "./sleeping_barber.h"

TEST(TESTS, Test_correct) {
  int rank = 0;
  int sizeWorld = 0;
  MPI_Comm comm = MPI_COMM_WORLD;

  MPI_Comm_rank(comm, &rank);
  MPI_Comm_size(comm, &sizeWorld);

  int clients = sizeWorld - 2;
  int seats = 3;

  if (clients < 1) {
    SUCCEED();
  } else {
    if (rank == 0) {
      ASSERT_NO_THROW(room(rank, clients, seats, comm));
    } else if (rank == 1) {
      ASSERT_NO_THROW(barber(rank, clients, seats, comm));
    } else {
      ASSERT_NO_THROW(client(rank, clients, seats, comm));
    }
  }
}

TEST(TESTS, Test_zero_clients) {
  int rank = 0;
  int sizeWorld = 0;
  MPI_Comm comm = MPI_COMM_WORLD;

  MPI_Comm_rank(comm, &rank);
  MPI_Comm_size(comm, &sizeWorld);

  int clients = 0;
  int seats = 3;

  if (rank == 0) {
    ASSERT_ANY_THROW(room(rank, clients, seats, comm));
  } else if (rank == 1) {
    ASSERT_ANY_THROW(barber(rank, clients, seats, comm));
  } else {
    ASSERT_ANY_THROW(client(rank, clients, seats, comm));
  }
}
TEST(TESTS, Test_zero_seats) {
  int rank = 0;
  int sizeWorld = 0;
  MPI_Comm comm = MPI_COMM_WORLD;

  MPI_Comm_rank(comm, &rank);
  MPI_Comm_size(comm, &sizeWorld);

  int clients = sizeWorld - 2;
  int seats = 0;

  if (rank == 0) {
    ASSERT_ANY_THROW(room(rank, clients, seats, comm));
  } else if (rank == 1) {
    ASSERT_ANY_THROW(barber(rank, clients, seats, comm));
  } else {
    ASSERT_ANY_THROW(client(rank, clients, seats, comm));
  }
}
TEST(TESTS, Test_zero_clients_and_seats) {
  int rank = 0;
  int sizeWorld = 0;
  MPI_Comm comm = MPI_COMM_WORLD;

  MPI_Comm_rank(comm, &rank);
  MPI_Comm_size(comm, &sizeWorld);

  int clients = 0;
  int seats = 0;

  if (rank == 0) {
    ASSERT_ANY_THROW(room(rank, clients, seats, comm));
  } else if (rank == 1) {
    ASSERT_ANY_THROW(barber(rank, clients, seats, comm));
  } else {
    ASSERT_ANY_THROW(client(rank, clients, seats, comm));
  }
}
TEST(TESTS, Test_one_seat) {
  int rank = 0;
  int sizeWorld = 0;
  MPI_Comm comm = MPI_COMM_WORLD;

  MPI_Comm_rank(comm, &rank);
  MPI_Comm_size(comm, &sizeWorld);

  int clients = sizeWorld - 2;
  int seats = 1;

  if (clients < 1) {
    SUCCEED();
  } else {
    if (rank == 0) {
      ASSERT_NO_THROW(room(rank, clients, seats, comm));
    } else if (rank == 1) {
      ASSERT_NO_THROW(barber(rank, clients, seats, comm));
    } else {
      ASSERT_NO_THROW(client(rank, clients, seats, comm));
    }
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
