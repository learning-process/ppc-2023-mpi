// Copyright 2023 Derun Andrew
#include <gtest/gtest.h>
#include <mpi.h>

#include <iostream>

#include "./vector_mean.h"

TEST(Parallel_Vector_Sum, Test_only_1) {
  int rankProc = 0;
  int numProc = 0;

  MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);
  MPI_Comm_size(MPI_COMM_WORLD, &numProc);

  std::vector<int> vec{1, 1, 1, 1};

  int result = meanPar(vec);

  if (rankProc == 0) {
    int resSeq = meanSeq(vec);
    ASSERT_EQ(result, resSeq);
  }
}

TEST(Parallel_Vector_Sum, Test_only_0) {
  int rankProc = 0;
  int numProc = 0;

  MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);
  MPI_Comm_size(MPI_COMM_WORLD, &numProc);

  std::vector<int> vec{0, 0, 0, 0, 0, 0, 0, 0, 0};

  int result = meanPar(vec);

  if (rankProc == 0) {
    int resSeq = meanSeq(vec);
    ASSERT_EQ(result, resSeq);
  }
}

TEST(Parallel_Vector_Sum, Test_not_elems) {
  int rankProc = 0;
  int numProc = 0;

  MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);
  MPI_Comm_size(MPI_COMM_WORLD, &numProc);

  std::vector<int> vec{};
  int result = meanPar(vec);

  if (rankProc == 0) {
    int resSeq = meanSeq(vec);
    ASSERT_EQ(result, resSeq);
  }
}

TEST(Parallel_Vector_Sum, Test_random_elems) {
  int rankProc = 0;
  int numProc = 0;

  MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);
  MPI_Comm_size(MPI_COMM_WORLD, &numProc);

  std::vector<int> vec(15);
  for (auto &elem : vec) elem = generateRandomNumber(-1000, 1000);

  int result = meanPar(vec);

  if (rankProc == 0) {
    int resSeq = meanSeq(vec);
    ASSERT_EQ(result, resSeq);
  }
}

TEST(Parallel_Vector_Sum, Test_half_elems) {
  int rankProc = 0;
  int numProc = 0;

  MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);
  MPI_Comm_size(MPI_COMM_WORLD, &numProc);

  std::vector<int> vec{0, 0, 0, 0, 0, 3, 3, 3, 3, 3,
                       0, 0, 0, 0, 0, 3, 3, 3, 3, 3};

  int result = meanPar(vec);

  if (rankProc == 0) {
    int resSeq = meanSeq(vec);
    ASSERT_EQ(result, resSeq);
  }
}

int main(int argc, char **argv) {
  int result_code = 0;

  ::testing::InitGoogleTest(&argc, argv);
  ::testing::TestEventListeners &listeners =
      ::testing::UnitTest::GetInstance()->listeners();

  std::srand(std::time(0));

  if (MPI_Init(&argc, &argv) != MPI_SUCCESS) MPI_Abort(MPI_COMM_WORLD, -1);
  result_code = RUN_ALL_TESTS();
  MPI_Finalize();

  return result_code;
}
