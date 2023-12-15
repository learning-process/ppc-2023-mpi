// Copyright 2023 Mortina Anastasiya
#include <gtest/gtest.h>
#include <mpi.h>
#include <iostream>
#include "./alg.h"

TEST(Mid_Value_Vector_MPI, Test_Random_Vector_With_2_Elements) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  std::vector <int> global_vector(2);
  
  for (auto &elem : global_vector) elem = generateRandomNumbers(0, 1000);
  
  double mid_parallel = midValueOfVectorParallel(global_vector);

  if (rank == 0) {
    double mid_sequential = sumOfVectorSequential(global_vector) / global_vector.size();
    ASSERT_EQ(mid_parallel, mid_sequential);
  }
}

TEST(Mid_Value_Vector_MPI, Test_Random_Vector_With_15_Elements) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  std::vector <int> global_vector(15);
  
  for (auto &elem : global_vector) elem = generateRandomNumbers(0, 1000);
  
  double mid_parallel = midValueOfVectorParallel(global_vector);

  if (rank == 0) {
    double mid_sequential = sumOfVectorSequential(global_vector) / global_vector.size();
    ASSERT_EQ(mid_parallel, mid_sequential);
  }
}

TEST(Mid_Value_Vector_MPI, Test_Random_Vector_With_100_Elements) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  std::vector <int> global_vector(100);
  
  for (auto &elem : global_vector) elem = generateRandomNumbers(0, 1000);
  
  double mid_parallel = midValueOfVectorParallel(global_vector);

  if (rank == 0) {
    double mid_sequential = sumOfVectorSequential(global_vector) / global_vector.size();
    ASSERT_EQ(mid_parallel, mid_sequential);
  }
}

TEST(Mid_Value_Vector_MPI, Test_Random_Vector_With_1_Elements) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  std::vector <int> global_vector(1);
  
  for (auto &elem : global_vector) elem = generateRandomNumbers(0, 1000);
  
  double mid_parallel = midValueOfVectorParallel(global_vector);

  if (rank == 0) {
    double mid_sequential = sumOfVectorSequential(global_vector) / global_vector.size();
    ASSERT_EQ(mid_parallel, mid_sequential);
  }
}

TEST(Mid_Value_Vector_MPI, Test_Random_Vector_Whith_const_Elements) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  std::vector <int> global_vector = {1, 2, 3, 4, 5, 6, 7, 8, 9};
  
  double mid_parallel = midValueOfVectorParallel(global_vector);

  if (rank == 0) {
    double mid_sequential = sumOfVectorSequential(global_vector) / global_vector.size();
    ASSERT_EQ(mid_parallel, mid_sequential);
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