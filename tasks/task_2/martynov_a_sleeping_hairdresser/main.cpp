// Copyright 2023 Martynov Alexandr
#include <gtest/gtest.h>
#include <mpi.h>
#include "../tasks/task_2/martynov_a_sleeping_hairdresser/sleeping_mans.h"
TEST(sleeping_hairdresser, only_two_mans) {
  int myrank, count_of_proc;
  MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
  MPI_Comm_size(MPI_COMM_WORLD, &count_of_proc);
  if (myrank == 0) {
    ASSERT_NO_THROW(bearded_man(myrank, 2, count_of_proc - 2));
  } else if (myrank == 1) {
    ASSERT_NO_THROW(queue(2, count_of_proc - 2));
  } else {
    ASSERT_NO_THROW(visitor(myrank));
  }
  MPI_Barrier(MPI_COMM_WORLD);
}

TEST(sleeping_hairdresser, fifteen_mans) {
  int myrank, count_of_proc;
  MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
  MPI_Comm_size(MPI_COMM_WORLD, &count_of_proc);

  if (myrank == 0) {
    ASSERT_NO_THROW(bearded_man(myrank, 15, count_of_proc - 2));
  } else if (myrank == 1) {
    ASSERT_NO_THROW(queue(15, count_of_proc - 2));
  } else {
    ASSERT_NO_THROW(visitor(myrank));
  }
  MPI_Barrier(MPI_COMM_WORLD);
}


TEST(sleeping_hairdresser, ten_mans) {
  int myrank, count_of_proc;
  MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
  MPI_Comm_size(MPI_COMM_WORLD, &count_of_proc);

  if (myrank == 0) {
    ASSERT_NO_THROW(bearded_man(myrank, 10, count_of_proc - 2));
  } else if (myrank == 1) {
    ASSERT_NO_THROW(queue(10, count_of_proc - 2));
  } else {
    ASSERT_NO_THROW(visitor(myrank));
  }
  MPI_Barrier(MPI_COMM_WORLD);
}

TEST(sleeping_hairdresser, 20_mans) {
  int myrank, count_of_proc;
  MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
  MPI_Comm_size(MPI_COMM_WORLD, &count_of_proc);

  if (myrank == 0) {
    ASSERT_NO_THROW(bearded_man(myrank, 20, count_of_proc - 2));
  } else if (myrank == 1) {
    ASSERT_NO_THROW(queue(10, count_of_proc - 2));
  } else {
    ASSERT_NO_THROW(visitor(myrank));
  }
  MPI_Barrier(MPI_COMM_WORLD);
}



TEST(sleeping_hairdresser, 35_mans) {
  int myrank, count_of_proc;
  MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
  MPI_Comm_size(MPI_COMM_WORLD, &count_of_proc);

  if (myrank == 0) {
    ASSERT_NO_THROW(bearded_man(myrank, 35, count_of_proc - 2));
  } else if (myrank == 1) {
    ASSERT_NO_THROW(queue(10, count_of_proc - 2));
  } else {
    ASSERT_NO_THROW(visitor(myrank));
  }
  MPI_Barrier(MPI_COMM_WORLD);
}

TEST(sleeping_hairdresser, one_man) {
  int myrank, count_of_proc;
  MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
  MPI_Comm_size(MPI_COMM_WORLD, &count_of_proc);

  if (myrank == 0) {
    ASSERT_NO_THROW(bearded_man(myrank, 1, count_of_proc - 2));
  } else if (myrank == 1) {
    ASSERT_NO_THROW(queue(1, count_of_proc - 2));
  } else {
    ASSERT_NO_THROW(visitor(myrank));
  }
  MPI_Barrier(MPI_COMM_WORLD);
}
int main(int argc, char** argv) {
  MPI_Init(&argc, &argv);
  int myrank;
  MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
  ::testing::InitGoogleTest(&argc, argv);
  ::testing::TestEventListeners& listeners =
    ::testing::UnitTest::GetInstance()->listeners();
  if (myrank != 0) {
    delete listeners.Release(listeners.default_result_printer());
  }
  int result = RUN_ALL_TESTS();

  MPI_Finalize();

  return result;
}
