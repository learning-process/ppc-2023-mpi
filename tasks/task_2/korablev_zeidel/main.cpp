// Copyright 2023 Korablev Nikita
#include <gtest/gtest.h>
#include "task_2/korablev_zeidel/zeidel.h"
#include <boost/mpi/communicator.hpp>
#include <boost/mpi/environment.hpp>

TEST(MPI_TESTS, zeidel_1) {
  boost::mpi::communicator world;
  std::string var;
  world.send(0, 0, var);
  int mat_len = 3;
  double eps = 0.0001;
  double* matrix = new double[mat_len*(mat_len + 1)];
  // A
  matrix[0] = 5;
  matrix[1] = 0;
  matrix[2] = 1;
  matrix[3] = 3;
  matrix[4] = -4;
  matrix[5] = 2;
  matrix[6] = 1;
  matrix[7] = 2;
  matrix[8] = 3;
  // b
  matrix[9] = 2;
  matrix[10] = 6;
  matrix[11] = 4;
  //------------
  double* res = zeidel(matrix, mat_len, eps);

  if (world.rank() == 0) {
    for (int i = 0; i < mat_len; i++) {
      for (int j = 0; j < mat_len; j++) {
        matrix[i * (mat_len + 1) + mat_len] -= matrix[i * (mat_len + 1) + j] * res[j];
      }
      ASSERT_GT(fabs(matrix[i * (mat_len + 1) + mat_len]), eps);
    }
  }

  delete[] res;
  delete[] matrix;
}

TEST(MPI_TESTS, zeidel_2) {
  boost::mpi::communicator world;
  int mat_len = 3;
  double eps = 0.00001;
  double* matrix = new double[mat_len*(mat_len + 1)]();
  // A
  matrix[0] = 5;
  matrix[1] = 2;
  matrix[2] = -1;
  matrix[3] = -4;
  matrix[4] = 7;
  matrix[5] = 3;
  matrix[6] = 2;
  matrix[7] = -2;
  matrix[8] = 4;
  // b
  matrix[9] = 12;
  matrix[10] = 24;
  matrix[11] = 9;
  //------------
  double* res = zeidel(matrix, mat_len, eps);
  if (world.rank() == 0) {
    for (int i = 0; i < mat_len; i++) {
      for (int j = 0; j < mat_len; j++) {
        matrix[i * (mat_len + 1) + mat_len] -= matrix[i * (mat_len + 1) + j] * res[j];
      }
      ASSERT_GT(fabs(matrix[i * (mat_len + 1) + mat_len]), eps);
    }
  }

  delete[] res;
  delete[] matrix;
}

TEST(MPI_TESTS, parallel_zeidel_1) {
  boost::mpi::communicator world;
  std::string var;
  world.send(0, 0, var);
  int mat_len = 3;
  double eps = 0.0001;
  double* matrix = new double[mat_len*(mat_len + 1)];
  // A
  matrix[0] = 5;
  matrix[1] = 0;
  matrix[2] = 1;
  matrix[3] = 3;
  matrix[4] = -4;
  matrix[5] = 2;
  matrix[6] = 1;
  matrix[7] = 2;
  matrix[8] = 3;
  // b
  matrix[9] = 2;
  matrix[10] = 6;
  matrix[11] = 4;
  //------------
  double* res = zeidel(matrix, mat_len, eps);

  if (world.rank() == 0) {
    for (int i = 0; i < mat_len; i++) {
      for (int j = 0; j < mat_len; j++) {
        matrix[i * (mat_len + 1) + mat_len] -= matrix[i * (mat_len + 1) + j] * res[j];
      }
      ASSERT_GT(fabs(matrix[i * (mat_len + 1) + mat_len]), eps);
    }
  }

  delete[] res;
  delete[] matrix;
}

TEST(MPI_TESTS, parallel_zeidel_2) {
  boost::mpi::communicator world;
  int mat_len = 3;
  double eps = 0.00001;
  double* matrix = new double[mat_len*(mat_len + 1)]();
  // A
  matrix[0] = 5;
  matrix[1] = 2;
  matrix[2] = -1;
  matrix[3] = -4;
  matrix[4] = 7;
  matrix[5] = 3;
  matrix[6] = 2;
  matrix[7] = -2;
  matrix[8] = 4;
  // b
  matrix[9] = 12;
  matrix[10] = 24;
  matrix[11] = 9;
  //------------
  double* res = zeidel(matrix, mat_len, eps);
  if (world.rank() == 0) {
    for (int i = 0; i < mat_len; i++) {
      for (int j = 0; j < mat_len; j++) {
        matrix[i * (mat_len + 1) + mat_len] -= matrix[i * (mat_len + 1) + j] * res[j];
      }
      ASSERT_GT(fabs(matrix[i * (mat_len + 1) + mat_len]), eps);
    }
  }

  delete[] res;
  delete[] matrix;
}


int main(int argc, char** argv) {
  boost::mpi::environment env(argc, argv);
  boost::mpi::communicator world;
  ::testing::InitGoogleTest(&argc, argv);
  ::testing::TestEventListeners& listeners = ::testing::UnitTest::GetInstance()->listeners();
  if (world.rank() != 0) delete listeners.Release(listeners.default_result_printer());

  return RUN_ALL_TESTS();;
}
