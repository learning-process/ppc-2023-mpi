// Copyright 2023 Savotina Valeria
#include <gtest/gtest.h>

#include "./par_configure_gradient.h"

TEST(TESTS, Test_1) { ASSERT_EQ(1, 1); }
TEST(TESTS, Test_2) { ASSERT_EQ(1, 1); }
TEST(TESTS, Test_3) { ASSERT_EQ(1, 1); }
TEST(TESTS, Test_4) { ASSERT_EQ(1, 1); }
TEST(TESTS, Test_5) { ASSERT_EQ(1, 1); }

int main(int argc, char** argv) {
  int result_code = 0;
  int rank = 0;

  ::testing::InitGoogleTest(&argc, argv);
  ::testing::TestEventListeners& listeners =
      ::testing::UnitTest::GetInstance()->listeners();

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if (rank != 0) {
    delete listeners.Release(listeners.default_result_printer());
  }

  result_code = RUN_ALL_TESTS();
  MPI_Finalize();

  return result_code;
}
