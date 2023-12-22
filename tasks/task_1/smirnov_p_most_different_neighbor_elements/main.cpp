// Copyright 2023 Smirnov Pavel

#include <mpi.h>
#include <gtest/gtest.h>
#include <vector>
#include <iostream>
#include "./most_different_vector_elements.h"
#include <boost/mpi/environment.hpp>
#include <boost/mpi/communicator.hpp>

TEST(most_different_vector_elements, can_find_the_right_answer) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<int> v(5);

  if (rank == 0) {
    v = getRandVect(5);
    }

  double res = mpi_find(v);

  if (rank == 0) {
    ASSERT_EQ(single_find(v), res);
  }
}

TEST(most_different_vector_elements, can_find_the_right_answer_with_small_vector) {
  int rank;
  std::vector<int> v(3);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if (rank == 0) {
    v = getRandVect(3);
  }

  double res = mpi_find(v);

  if (rank == 0) {
    ASSERT_EQ(single_find(v), res);
  }
}

TEST(most_different_vector_elements, can_find_the_right_answer_with_big_vector) {
  std::vector<int> v(100);
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if (rank == 0) {
    v = getRandVect(100);
  }

  double res = mpi_find(v);

  if (rank == 0) {
    ASSERT_EQ(single_find(v), res);
  }
}

TEST(most_different_vector_elements, can_find_the_answer_with_very_big_vector) {
  std::vector<int> v(1000);
  int rank;

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if (rank == 0) {
    v = getRandVect(1000);
  }

  double res = mpi_find(v);

  if (rank == 0) {
    ASSERT_EQ(single_find(v), res);
  }
}

TEST(most_different_vector_elements, can_find_the_right_answer_with_even_number_elements_vector) {
  std::vector<int> v(7);
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if (rank == 0) {
    v = getRandVect(7);
  }

  double res = mpi_find(v);

  if (rank == 0) {
    ASSERT_EQ(single_find(v), res);
  }
}

int main(int argc, char** argv) {
    boost::mpi::environment env(argc, argv);
    boost::mpi::communicator world;
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::TestEventListeners& listeners = ::testing::UnitTest::GetInstance()->listeners();
    if (world.rank() != 0) {
        delete listeners.Release(listeners.default_result_printer());
    }
    return RUN_ALL_TESTS();
}

