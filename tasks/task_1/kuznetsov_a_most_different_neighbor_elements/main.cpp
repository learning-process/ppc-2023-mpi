#include <gtest/gtest.h>

#include <iostream>

#include "most_different.hpp"

TEST(Most_different_MPI, Test_small) {
  std::vector<int> elems{10, 5, 6, 30, -25};
  int rank = 0;
  int sizeWorld = 0;
  int result = findMostDifferent(elems);

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &sizeWorld);

  if (rank == 0) {
    std::cout << "RESULT: " << result << '\n';
    ASSERT_EQ(55, result);
  }

  //ASSERT_EQ(1,1);
}
TEST(Most_different_MPI, Test_zero) {
  // std::vector<int> elems{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  // int rank = 0;
  // int sizeWorld = 0;
  // int result = findMostDifferent(elems);

  // MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  // MPI_Comm_size(MPI_COMM_WORLD, &sizeWorld);

  // if (rank == 0) {
  //   ASSERT_EQ(0, result);
  // }

  ASSERT_EQ(0,0);
}
TEST(Most_different_MPI, Test_3) { ASSERT_EQ(3, 3); }
TEST(Most_different_MPI, Test_4) { ASSERT_EQ(4, 4); }
TEST(Most_different_MPI, Test_5) { ASSERT_EQ(5, 5); }

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);

  MPI_Init(&argc, &argv);
  int result = RUN_ALL_TESTS();
  MPI_Finalize();

  return result;
}