// Copyright 2023 Prokofev Kirill
#include <gtest/gtest.h>
#include <mpi.h>
#include <iostream>
#include <algorithm>
#include "./sort_realization.h"

TEST(Sort_Realization, Test_Entered_Vector) {
  int rankProc;
  MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);
  std::vector<int> testVect = {5, 11, 23, 2, 4, 1, 44, 3, 0, 10, 13, 7};
  std::vector<int> refVect = testVect;
  double t1, t2;
  if (rankProc == 0) {
    t1 = MPI_Wtime();
  }
  ShellSortParallel(testVect);
  if (rankProc == 0) {
    t2 = MPI_Wtime();
    std::cout << "algorithm time : " << t2 - t1 << std::endl;
    std::sort(refVect.begin(), refVect.end());
    EXPECT_EQ(testVect, refVect);
  }
}

TEST(Sort_Realization, Test_Random_Vector_1000_Elem) {
  int rankProc;
  MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);
  std::vector<int> testVect = GenerateRandomVector(1000);
  std::vector<int> refVect = testVect;
  double t1, t2;
  if (rankProc == 0) {
    t1 = MPI_Wtime();
  }
  ShellSortParallel(testVect);
  if (rankProc == 0) {
    t2 = MPI_Wtime();
    std::cout << "algorithm time : " << t2 - t1 << std::endl;
    std::sort(refVect.begin(), refVect.end());
    EXPECT_EQ(testVect, refVect);
  }
}

TEST(Sort_Realization, Test_Random_Vector_5000_Elem) {
  int rankProc;
  MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);
  std::vector<int> testVect = GenerateRandomVector(5000);
  std::vector<int> refVect = testVect;
  double t1, t2;
  if (rankProc == 0) {
    t1 = MPI_Wtime();
  }
  ShellSortParallel(testVect);
  if (rankProc == 0) {
    t2 = MPI_Wtime();
    std::cout << "algorithm time : " << t2 - t1 << std::endl;
    std::sort(refVect.begin(), refVect.end());
    EXPECT_EQ(testVect, refVect);
  }
}

TEST(Sort_Realization, Test_Random_Vector_12223_Elem) {
  int rankProc;
  MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);
  std::vector<int> testVect = GenerateRandomVector(12223);
  std::vector<int> refVect = testVect;
  double t1, t2;
  if (rankProc == 0) {
    t1 = MPI_Wtime();
  }
  ShellSortParallel(testVect);
  if (rankProc == 0) {
    t2 = MPI_Wtime();
    std::cout << "algorithm time : " << t2 - t1 << std::endl;
    std::sort(refVect.begin(), refVect.end());
    EXPECT_EQ(testVect, refVect);
  }
}
TEST(Sort_Realization, Test_Random_Vector_777_Elem) {
  int rankProc;
  MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);
  std::vector<int> testVect = GenerateRandomVector(777);
  std::vector<int> refVect = testVect;
  double t1, t2;
  if (rankProc == 0) {
    t1 = MPI_Wtime();
  }
  ShellSortParallel(testVect);
  if (rankProc == 0) {
    t2 = MPI_Wtime();
    std::cout << "algorithm time : " << t2 - t1 << std::endl;
    std::sort(refVect.begin(), refVect.end());
    EXPECT_EQ(testVect, refVect);
  }
}
TEST(Sort_Realization, Test_Random_Vector_50000_Elem) {
  int rankProc;
  MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);
  std::vector<int> testVect = GenerateRandomVector(50000);
  std::vector<int> refVect = testVect;
  double t1, t2;
  if (rankProc == 0) {
    t1 = MPI_Wtime();
  }
  ShellSortParallel(testVect);
  if (rankProc == 0) {
    t2 = MPI_Wtime();
    std::cout << "algorithm time : " << t2 - t1 << std::endl;
    std::sort(refVect.begin(), refVect.end());
    EXPECT_EQ(testVect, refVect);
  }
}

int main(int argc, char** argv) {
  int result_code = 0;
  ::testing::InitGoogleTest(&argc, argv);
  ::testing::TestEventListeners& listeners = ::testing::UnitTest::GetInstance()->listeners();
  if (MPI_Init(&argc, &argv) != MPI_SUCCESS)
    MPI_Abort(MPI_COMM_WORLD, -1);
  result_code = RUN_ALL_TESTS();
  MPI_Finalize();
  return result_code;
}
