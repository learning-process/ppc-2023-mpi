// Copyright 2023 Korablev Nikita
#include <gtest/gtest.h>
#include <boost/mpi/communicator.hpp>
#include <boost/mpi/environment.hpp>

#include <iostream>

#include "task_2/korablev_zeidel/zeidel.h"

TEST(MPI_TESTS, Test_inc) {
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
    for (int i = 0; i < mat_len; i++) std::cout << res[i] << std::endl;
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
