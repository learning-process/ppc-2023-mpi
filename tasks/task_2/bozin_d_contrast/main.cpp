// Copyright 2023 Bozin Dmitry
#include <gtest/gtest.h>
#include <mpi.h>
#include <iostream>
#include "./contrast.h"

TEST(Parallel_Contrast, Test1) {
  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  std::vector<std::vector<int>> globalVec;
  std::vector<std::vector<int>> resParalel;
  std::vector<std::vector<int>> resSeq;
  int weight = size + 1;
  int height = size + 2;

  if (rank == 0) {
    globalVec = std::vector<std::vector<int>>(weight);

    for (int i = 0; i < weight; i++) {
      globalVec[i] = getRandomVector(height);
    }
    resSeq = SequentialContrast(globalVec, weight, height);
  }

  resParalel = ParallelContrast(globalVec, weight, height);


  if (rank == 0) {
    ASSERT_EQ(resParalel, resSeq);
  }
}



TEST(Parallel_Contrast, Test2) {
  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  std::vector<std::vector<int>> globalVec;
  std::vector<std::vector<int>> resParalel;
  std::vector<std::vector<int>> resSeq;
  int weight = 1;
  int height = size * 20;

  if (rank == 0) {
    globalVec = std::vector<std::vector<int>>(weight);

    for (int i = 0; i < weight; i++) {
      globalVec[i] = getRandomVector(height);
    }
    resSeq = SequentialContrast(globalVec, weight, height);
  }

  resParalel = ParallelContrast(globalVec, weight, height);


  if (rank == 0) {
    ASSERT_EQ(resParalel, resSeq);
  }
}

TEST(Parallel_Contrast, Test3) {
  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  std::vector<std::vector<int>> globalVec;
  std::vector<std::vector<int>> resParalel;
  std::vector<std::vector<int>> resSeq;
  int weight = 1;
  int height = size * 20 + 1;

  if (rank == 0) {
    globalVec = std::vector<std::vector<int>>(weight);

    for (int i = 0; i < weight; i++) {
      globalVec[i] = getRandomVector(height);
    }
    resSeq = SequentialContrast(globalVec, weight, height);
  }

  resParalel = ParallelContrast(globalVec, weight, height);


  if (rank == 0) {
    ASSERT_EQ(resParalel, resSeq);
  }
}

TEST(Parallel_Contrast, Test4) {
  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  std::vector<std::vector<int>> globalVec;
  std::vector<std::vector<int>> resParalel;
  std::vector<std::vector<int>> resSeq;
  int weight = 1;
  int height = size * 20 - 1;

  if (rank == 0) {
    globalVec = std::vector<std::vector<int>>(weight);

    for (int i = 0; i < weight; i++) {
      globalVec[i] = getRandomVector(height);
    }
    resSeq = SequentialContrast(globalVec, weight, height);
  }

  resParalel = ParallelContrast(globalVec, weight, height);


  if (rank == 0) {
    ASSERT_EQ(resParalel, resSeq);
  }
}

TEST(Parallel_Contrast, Test5) {
  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  std::vector<std::vector<int>> globalVec;
  std::vector<std::vector<int>> resParalel;
  std::vector<std::vector<int>> resSeq;
  int weight = size * 20 - 1;
  int height = 1;

  if (rank == 0) {
    globalVec = std::vector<std::vector<int>>(weight);

    for (int i = 0; i < weight; i++) {
      globalVec[i] = getRandomVector(height);
    }
    resSeq = SequentialContrast(globalVec, weight, height);
  }

  resParalel = ParallelContrast(globalVec, weight, height);


  if (rank == 0) {
    ASSERT_EQ(resParalel, resSeq);
  }
}

int main(int argc, char** argv) {
  int resultCode = 0;

  ::testing::InitGoogleTest(&argc, argv);
  ::testing::TestEventListeners& listeners =
    ::testing::UnitTest::GetInstance()->listeners();

  if (MPI_Init(&argc, &argv) != MPI_SUCCESS)
    MPI_Abort(MPI_COMM_WORLD, -1);
  resultCode = RUN_ALL_TESTS();
  MPI_Finalize();

  return resultCode;
}
