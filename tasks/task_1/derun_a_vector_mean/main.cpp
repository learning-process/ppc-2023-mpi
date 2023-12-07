// Copyright 2023 Derun Andrew
#include <gtest/gtest.h>
#include <mpi.h>

#include <iostream>

#include "./vector_mean.h"

TEST(Parallel_Vector_Mean, Test_only_1) {
  int rankProc = 0;
  int numProc = 0;
  double epsilon = 0.001;

  MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);
  MPI_Comm_size(MPI_COMM_WORLD, &numProc);

  std::vector<int> vec{1, 1, 1, 1};

  double result = meanPar(vec);

  if (rankProc == 0) {
    double resSeq = meanSeq(vec);
    ASSERT_NEAR(result, resSeq, epsilon);
  }
}

TEST(Parallel_Vector_Mean, Test_only_0) {
  int rankProc = 0;
  int numProc = 0;
  double epsilon = 0.001;

  MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);
  MPI_Comm_size(MPI_COMM_WORLD, &numProc);

  std::vector<int> vec{0, 0, 0, 0, 0, 0, 0, 0, 0};

  double result = meanPar(vec);

  if (rankProc == 0) {
    double resSeq = meanSeq(vec);
    ASSERT_NEAR(result, resSeq, epsilon);
  }
}

TEST(Parallel_Vector_Mean, Test_random_elems_1) {
  int rankProc = 0;
  int numProc = 0;
  double epsilon = 0.001;

  MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);
  MPI_Comm_size(MPI_COMM_WORLD, &numProc);

  std::vector<int> vec(15);
  for (auto &elem : vec) elem = generateRandomNumber(-1000, 1000);

  double result = meanPar(vec);

  if (rankProc == 0) {
    double resSeq = meanSeq(vec);
    ASSERT_NEAR(result, resSeq, epsilon);
  }
}

TEST(Parallel_Vector_Mean, Test_random_elems_2) {
  int rankProc = 0;
  int numProc = 0;
  double epsilon = 0.001;

  MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);
  MPI_Comm_size(MPI_COMM_WORLD, &numProc);

  std::vector<int> vec(15);
  for (auto &elem : vec) elem = generateRandomNumber(0, 10000);

  double result = meanPar(vec);

  if (rankProc == 0) {
    double resSeq = meanSeq(vec);
    ASSERT_NEAR(result, resSeq, epsilon);
  }
}

TEST(Parallel_Vector_Mean, Test_half_elems) {
  int rankProc = 0;
  int numProc = 0;
  double epsilon = 0.001;

  MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);
  MPI_Comm_size(MPI_COMM_WORLD, &numProc);

  std::vector<int> vec{0, 0, 0, 0, 0, 3, 3, 3, 3, 3,
                       0, 0, 0, 0, 0, 3, 3, 3, 3, 3};

  double result = meanPar(vec);

  if (rankProc == 0) {
    double resSeq = meanSeq(vec);
    ASSERT_NEAR(result, resSeq, epsilon);
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
