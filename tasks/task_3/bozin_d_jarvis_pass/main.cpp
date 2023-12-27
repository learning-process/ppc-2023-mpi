// Copyright 2023 Bozin Dmitry

#include <gtest/gtest.h>
#include "./jarvis_pass.h"

TEST(BasicTest, Test1) {
  int ProcRank;
  int n, m;
  n = 3;
  m = 4;
  std::vector<int> image = { 0, 0, 0, 179,
                            0, 0, 1, 128,
                            128, 0, 0, 128 };
  std::vector<point> not_par;
  std::vector<point> par;
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
  par = getConvexHullParallel(image, n, m);
  if (ProcRank == 0) {
    not_par = getConvexHull(image, n, m);
    bool check = checkAnswers(not_par, par);
    ASSERT_EQ(true, check);
  }
}

TEST(BasicTest, Test2) {
  int ProcRank;
  int n, m;
  n = 8;
  m = 8;
  std::vector<int> image = { 220, 0, 0, 220, 8, 122, 0, 0,
                            0, 0, 128, 179, 223, 0, 0, 0,
                            179, 0, 0, 2, 128, 255, 0, 123,
                            0, 0, 128, 179, 223, 0, 0, 0,
                            0, 0, 0, 179, 0, 0, 0, 0,
                            0, 0, 0, 179, 223, 0, 0, 0,
                            0, 0, 128, 0, 80, 0, 80, 0,
                            0, 0, 0, 179, 223, 0, 0, 0, };
  std::vector<point> not_par;
  std::vector<point> par;
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
  par = getConvexHullParallel(image, n, m);
  if (ProcRank == 0) {
    not_par = getConvexHull(image, n, m);
    bool check = checkAnswers(not_par, par);
    ASSERT_EQ(true, check);
  }
}

TEST(BasicTest, Test3) {
  int ProcRank;
  int n, m;
  n = 8;
  m = 12;
  std::vector<int> image = { 220, 0, 0, 220, 8, 122, 0, 0, 0, 255, 100, 0,
                            0, 0, 128, 179, 223, 0, 0, 0, 0, 0, 120, 120,
                            179, 0, 0, 2, 128, 255, 0, 123, 0, 0, 0, 0,
                            0, 0, 128, 179, 223, 0, 0, 0, 123, 0, 0, 0,
                            0, 0, 0, 179, 0, 0, 0, 0, 0, 0, 0, 0,
                            0, 0, 0, 179, 223, 0, 0, 0, 220, 220, 0, 0,
                            0, 0, 128, 0, 80, 0, 80, 0, 0, 0, 0, 0,
                            0, 0, 0, 179, 223, 0, 0, 0, 100, 0, 0, 0 };
  std::vector<point> not_par;
  std::vector<point> par;
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
  par = getConvexHullParallel(image, n, m);
  if (ProcRank == 0) {
    not_par = getConvexHull(image, n, m);
    bool check = checkAnswers(not_par, par);
    ASSERT_EQ(true, check);
  }
}

TEST(BasicTest, Test4) {
  int ProcRank;
  int n, m;
  n = 12;
  m = 8;
  std::vector<int> image = { 220, 0, 0, 220, 8, 122, 0, 0,
                            0, 0, 128, 179, 223, 0, 0, 0,
                            179, 0, 0, 2, 128, 255, 0, 123,
                            0, 0, 128, 179, 223, 0, 0, 0,
                            0, 0, 0, 179, 0, 0, 0, 0,
                            0, 0, 0, 179, 223, 0, 0, 0,
                            0, 0, 128, 0, 80, 0, 80, 179,
                            0, 0, 0, 179, 223, 0, 0, 0,
                            179, 0, 0, 179, 223, 0, 0, 0,
                            0, 0, 0, 0, 223, 0, 0, 0,
                            0, 179, 0, 100, 0, 0, 179, 0,
                            0, 1, 0, 179, 223, 0, 0, 100 };
  std::vector<point> not_par;
  std::vector<point> par;
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
  par = getConvexHullParallel(image, n, m);
  if (ProcRank == 0) {
    not_par = getConvexHull(image, n, m);
    bool check = checkAnswers(not_par, par);
    ASSERT_EQ(true, check);
  }
}

TEST(BasicTest, Test5) {
  int ProcRank;
  int n, m;
  n = 9;
  m = 10;
  std::vector<int> image = { 220, 0, 0, 220, 8, 122, 0, 0, 120, 225,
                            0, 0, 128, 179, 223, 0, 0, 0, 0, 0,
                            179, 0, 0, 2, 128, 255, 0, 123, 225, 0,
                            0, 0, 128, 179, 223, 0, 0, 0, 100, 0,
                            0, 0, 0, 179, 0, 0, 0, 0, 30, 0,
                            0, 0, 0, 179, 223, 0, 0, 0, 0, 0,
                            0, 0, 128, 0, 80, 0, 80, 179, 0, 0,
                            0, 0, 0, 179, 223, 0, 0, 0, 225, 0,
                            179, 0, 0, 179, 100, 0, 100, 0, 0, 0 };
  std::vector<point> not_par;
  std::vector<point> par;
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
  par = getConvexHullParallel(image, n, m);
  if (ProcRank == 0) {
    not_par = getConvexHull(image, n, m);
    bool check = checkAnswers(not_par, par);
    ASSERT_EQ(true, check);
  }
}

int main(int argc, char** argv) {
  int res = 0;

  ::testing::InitGoogleTest(&argc, argv);
  ::testing::TestEventListeners& listeners = ::testing::UnitTest::GetInstance()->listeners();

  MPI_Init(&argc, &argv);
  int ProcRank;
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
  if (ProcRank != 0) {
    delete listeners.Release(listeners.default_result_printer());
  }
  res = RUN_ALL_TESTS();
  MPI_Finalize();

  return res;
}
