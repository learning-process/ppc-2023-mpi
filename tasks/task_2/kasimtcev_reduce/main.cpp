// Copyright 2023 Kasimtcev Roman
#include <gtest/gtest.h>
#include <mpi.h>
#include <cstdlib>
#include "./reduce.h"

typedef int TypeOfVector;
static const MPI_Datatype mpi_TypeOfVector = MPI_INT;
static const int SizeVector = 500;
static const TypeOfVector MinElement = -500;
static const TypeOfVector MaxElement = 500;

inline void TestVector() {
  double res_time[2];
  TypeOfVector *a, *b;
  int i, n;
  int Num, Rank;
  TypeOfVector sum, SumOfAll, RealSuml;
  MPI_Comm_size(MPI_COMM_WORLD, &Num);
  MPI_Comm_rank(MPI_COMM_WORLD, &Rank);
  if (Rank == 0) {
    n = Num * SizeVector;
    a = new TypeOfVector[n];
    b = new TypeOfVector[n];
    RealSuml = 0;
    for (i = 0; i < n; i++) {
      a[i] = std::rand() % (MinElement - MaxElement + 1) + MaxElement;
      b[i] = std::rand() % (MinElement - MaxElement + 1) + MaxElement;
      RealSuml += a[i] * b[i];
    }
  }
  MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
  n = n / Num;
  if (Rank != 0) {
    a = new TypeOfVector[n];
    b = new TypeOfVector[n];
  }

  if (Rank == 0) {
    MPI_Scatter(a, n, mpi_TypeOfVector, MPI_IN_PLACE, n, mpi_TypeOfVector, 0, MPI_COMM_WORLD);
  } else {
    MPI_Scatter(NULL, n, mpi_TypeOfVector, a, n, mpi_TypeOfVector, 0, MPI_COMM_WORLD);
  }
  if (Rank == 0) {
    MPI_Scatter(b, n, mpi_TypeOfVector, MPI_IN_PLACE, n, mpi_TypeOfVector, 0, MPI_COMM_WORLD);
  } else {
    MPI_Scatter(NULL, n, mpi_TypeOfVector, b, n, mpi_TypeOfVector, 0, MPI_COMM_WORLD);
  }

  sum = SumOfAll = 0;
  for (i = 0; i < n; i++) {
    sum += a[i] * b[i];
  }

  if (Rank == 0) {
    res_time[0] = MPI_Wtime();
  }
  MPI_Reduce(&sum, &SumOfAll, 1, mpi_TypeOfVector, MPI_SUM, 0, MPI_COMM_WORLD);
  if (Rank == 0) {
    res_time[1] = MPI_Wtime();
    std::cout << "MPI_Reduce time = " << (res_time[1] - res_time[0]) << '\n';
    EXPECT_EQ(RealSuml, SumOfAll);
  }

  SumOfAll = 0;
  if (Rank == 0) {
    res_time[0] = MPI_Wtime();
  }
  EXPECT_EQ(MPI_SUCCESS, mpi_reduce(&sum, &SumOfAll, 1, mpi_TypeOfVector, MPI_SUM, 0, MPI_COMM_WORLD));
  if (Rank == 0) {
    res_time[1] = MPI_Wtime();
    std::cout << "mpi_reduce time = " << (res_time[1] - res_time[0]) << '\n';
  }
  if (Rank == 0) {
    EXPECT_EQ(RealSuml, SumOfAll);
  }
  delete[] a;
  delete[] b;
}

TEST(Parallel_Operation_Reduce_MPI, test_calc_tree_props) {
  proc_tree_props props;
  int n = 15;
  int* path = new int[n];
  int path_size;

  Calculate(7, n, &props, path, &path_size);
  EXPECT_EQ(-1, props.parent);
  EXPECT_EQ(3, props.children[0]);
  EXPECT_EQ(11, props.children[1]);
  EXPECT_EQ(1, path_size);
  if (path_size == 1) {
    EXPECT_EQ(7, path[0]);
  }

  Calculate(9, n, &props, path, &path_size);
  EXPECT_EQ(11, props.parent);
  EXPECT_EQ(8, props.children[0]);
  EXPECT_EQ(10, props.children[1]);
  EXPECT_EQ(3, path_size);
  if (path_size == 3) {
    EXPECT_EQ(7, path[0]);
    EXPECT_EQ(11, path[1]);
    EXPECT_EQ(9, path[2]);
  }

  Calculate(2, n, &props, path, &path_size);
  EXPECT_EQ(1, props.parent);
  EXPECT_EQ(-1, props.children[0]);
  EXPECT_EQ(-1, props.children[1]);
  EXPECT_EQ(4, path_size);
  if (path_size == 4) {
    EXPECT_EQ(7, path[0]);
    EXPECT_EQ(3, path[1]);
    EXPECT_EQ(1, path[2]);
    EXPECT_EQ(2, path[3]);
  }
  delete[] path;
}

TEST(Parallel_Operation_Reduce_MPI, test1_vector_scalar) {
  TestVector();
}

TEST(Parallel_Operation_Reduce_MPI, test2_vector_scalar) {
  TestVector();
}

TEST(Parallel_Operation_Reduce_MPI, test3_vector_scalar) {
  TestVector();
}

TEST(Parallel_Operation_Reduce_MPI, test4_vector_scalar) {
  TestVector();
}

TEST(Parallel_Operation_Reduce_MPI, test5_vector_scalar) {
  TestVector();
}

int main(int argc, char** argv) {
  int world_rank, result;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
  ::testing::InitGoogleTest(&argc, argv);
  ::testing::TestEventListeners& listeners = ::testing::UnitTest::GetInstance()->listeners();
  if (world_rank != 0) {
    delete listeners.Release(listeners.default_result_printer());
  } else {
    std::srand(std::time(nullptr));
  }
  result = RUN_ALL_TESTS();
  MPI_Finalize();
  return result;
}
