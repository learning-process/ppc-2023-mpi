// Copyright 2023 Vinokurov Ivan
#include <gtest/gtest.h>
#include <vector>
#include "task_2/vinokurov_i_seidel_iteration_method/seidel_iteration_method.h"



int main(int argc, char** argv) {
  MPI_Init(&argc, &argv);
  ::testing::InitGoogleTest(&argc, argv);
  int result = RUN_ALL_TESTS();
  MPI_Finalize();
  return result;
}
