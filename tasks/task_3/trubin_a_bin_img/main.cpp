// Copyright 2023 Trubin Artem
#include <mpi.h>
#include <gtest/gtest.h>
#include <time.h>
#include <iostream>
#include "./bin_img.h"

TEST(broadcast, test1) {
  int rank, size, exp_count = 7;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  image img(8, 8);

  if (rank == 0) {
    img.data[0][0] = 1;
    img.data[0][1] = 1;
    img.data[0][2] = 1;
    img.data[0][3] = 1;
    img.data[0][5] = 1;
    img.data[0][6] = 1;
    img.data[1][5] = 1;
    img.data[1][6] = 1;
    img.data[2][5] = 1;
    img.data[2][6] = 1;
    img.data[2][1] = 1;
    img.data[2][2] = 1;
    img.data[4][2] = 1;
    img.data[4][4] = 1;
    img.data[5][2] = 1;
    img.data[5][4] = 1;
    img.data[5][6] = 1;
    img.data[5][1] = 1;
    img.data[6][1] = 1;
    img.data[6][2] = 1;
    img.data[6][6] = 1;
    img.data[7][4] = 1;
    img.data[7][6] = 1;
    img.data[7][7] = 1;
    // std::cout << img;
    // std::cout << std::endl;
  }

  labeling(&img);

  if (rank == 0) {
    // std::cout << img;
    ASSERT_EQ(exp_count, 7);
  }
}

TEST(broadcast, test2) {
  int rank, size, exp_count = 3;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  image img(4, 4);

  if (rank == 0) {
    img.data[0][0] = 1;
    img.data[1][0] = 1;
    img.data[3][0] = 1;
    img.data[0][2] = 1;
    img.data[0][2] = 1;
    img.data[1][2] = 1;
    img.data[3][1] = 1;

    // std::cout << img;
    // std::cout << std::endl;
  }

  labeling(&img);

  if (rank == 0) {
    // std::cout << img;
    ASSERT_EQ(exp_count, 3);
  }
}

TEST(broadcast, test3) {
  int rank, size, exp_count = 1;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  image img(4, 4);

  if (rank == 0) {
    img.data[0][0] = 1;
    img.data[1][0] = 1;
    img.data[2][0] = 1;
    img.data[3][0] = 1;
    img.data[3][2] = 1;
    img.data[3][3] = 1;
    img.data[1][1] = 1;
    img.data[1][2] = 1;
    // std::cout << img;
    // std::cout << std::endl;
  }

  labeling(&img);

  if (rank == 0) {
    // std::cout << img;
    ASSERT_EQ(exp_count, 1);
  }
}

TEST(broadcast, test4) {
  int rank, size, exp_count = 0;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  image img(4, 4);

  if (rank == 0) {
    img.data[0][1] = 1;
    img.data[3][0] = 1;
    img.data[3][3] = 1;
    img.data[3][2] = 1;
    img.data[0][3] = 1;
    img.data[1][3] = 1;
    img.data[2][3] = 1;

    // std::cout << img;
    // std::cout << std::endl;
  }

  labeling(&img);

  if (rank == 0) {
    // std::cout << img;
    ASSERT_EQ(exp_count, 0);
  }
}

TEST(broadcast, test5) {
  int rank, size, exp_count = -1;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  image img(4, 4);

  if (rank == 0) {
    img.data[0][0] = 1;
    img.data[0][1] = 1;
    img.data[0][2] = 1;
    img.data[0][3] = 1;
    img.data[1][0] = 1;
    img.data[1][3] = 1;
    img.data[2][0] = 1;
    img.data[2][3] = 1;
    img.data[3][0] = 1;
    img.data[3][1] = 1;
    img.data[3][2] = 1;
    img.data[3][3] = 1;

    // std::cout << img;
    // std::cout << std::endl;
  }

  labeling(&img);

  if (rank == 0) {
    // std::cout << img;
    ASSERT_EQ(exp_count, -1);
  }
}

int main(int argc, char** argv) {
  int result_code = 0;

  ::testing::InitGoogleTest(&argc, argv);
  ::testing::TestEventListeners& listeners =
      ::testing::UnitTest::GetInstance()->listeners();

  MPI_Init(&argc, &argv);
  result_code = RUN_ALL_TESTS();
  MPI_Finalize();

  return result_code;
}
