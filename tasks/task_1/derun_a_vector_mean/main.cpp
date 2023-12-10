
// Copyright 2023 Derun Andrew
#include <gtest/gtest.h>
#include <mpi.h>  // 123

#include <iostream>

#include "task_1/derun_a_vector_mean/vector_mean.h"

TEST(MPI_Vector_Mean, Test_NEGATIVE_SIZE) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<int> global_vec;
  const int count_size_vector = -1;

  if (rank == 0) global_vec = getRandomVector<int>(count_size_vector, 0, 100);

  float pMean = getParallelMean(global_vec, count_size_vector);

  if (rank == 0) {
    float sMean = getSequentialMean(global_vec);
    ASSERT_FLOAT_EQ(sMean, pMean);
  }
}

TEST(MPI_Vector_Mean, Test_0_Elements) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<int> global_vec;
  const int count_size_vector = 0;

  if (rank == 0) global_vec = getRandomVector<int>(count_size_vector, 0, 100);

  float pMean = getParallelMean(global_vec, count_size_vector);

  if (rank == 0) {
    float sMean = getSequentialMean(global_vec);
    ASSERT_FLOAT_EQ(sMean, pMean);
  }
}

TEST(MPI_Vector_Mean, Test_10_Elements) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<int> global_vec;
  const int count_size_vector = 12;

  if (rank == 0) global_vec = getRandomVector<int>(count_size_vector, 0, 100);

  float pMean = getParallelMean(global_vec, count_size_vector);

  if (rank == 0) {
    float sMean = getSequentialMean(global_vec);
    ASSERT_FLOAT_EQ(sMean, pMean);
  }
}

TEST(MPI_Vector_Mean, Test_12_Elements) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<int> global_vec;
  const int count_size_vector = 12;

  if (rank == 0) global_vec = getRandomVector<int>(count_size_vector, 0, 100);

  float pMean = getParallelMean(global_vec, count_size_vector);

  if (rank == 0) {
    float sMean = getSequentialMean(global_vec);
    ASSERT_FLOAT_EQ(sMean, pMean);
  }
}

TEST(MPI_Vector_Mean, Test_120_Elements) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<int> global_vec;
  const int count_size_vector = 120;

  if (rank == 0) global_vec = getRandomVector<int>(count_size_vector, 0, 100);

  float pMean = getParallelMean(global_vec, count_size_vector);

  if (rank == 0) {
    float sMean = getSequentialMean(global_vec);
    ASSERT_FLOAT_EQ(sMean, pMean);
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
