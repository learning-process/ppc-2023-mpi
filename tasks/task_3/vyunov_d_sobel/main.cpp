// Copyright 2023 Vynov Danila
#include <gtest/gtest.h>

#include <mpi.h>

#include "./sobel.h"

TEST(PARALLEL_SOBEL_MPI, TEST_1) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  double start, end;

  int* image = nullptr;
  const int width = 2123, height = 1256;

  if (rank == 0) {
    image = getRandomImage(width, height);
  }
  start = MPI_Wtime();
  int* new_image_parallel = getParallelSobel(image, width, height);
  end = MPI_Wtime();

  if (rank == 0) {
    double ptime = end - start;
    start = MPI_Wtime();
    int* reference_new_image = getSequentialSobel(image, width, height);
    end = MPI_Wtime();
    double stime = end - start;

    std::cout << "P time " << ptime << std::endl;
    std::cout << "S time " << stime << std::endl;
    std::cout << "Speedup " << stime / ptime << std::endl;

    for (int i = 0; i < (width - 2) * (height - 2);
         i += (width - 2)) {
      ASSERT_EQ(new_image_parallel[i], reference_new_image[i]);
    }
  }
}

TEST(PARALLEL_SOBEL_MPI, TEST_2) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  double start, end;

  int* image = nullptr;
  const int width = 1000, height = 1200;

  if (rank == 0) {
    image = getRandomImage(width, height);
  }
  start = MPI_Wtime();
  int* new_image_parallel = getParallelSobel(image, width, height);
  end = MPI_Wtime();

  if (rank == 0) {
    double ptime = end - start;
    start = MPI_Wtime();
    int* reference_new_image = getSequentialSobel(image, width, height);
    end = MPI_Wtime();
    double stime = end - start;

    std::cout << "P time " << ptime << std::endl;
    std::cout << "S time " << stime << std::endl;
    std::cout << "Speedup " << stime / ptime << std::endl;

    for (int i = 0; i < (width - 2) * (height - 2); i += (width - 2)) {
      ASSERT_EQ(new_image_parallel[i], reference_new_image[i]);
    }
  }
}

TEST(PARALLEL_SOBEL_MPI, TEST_3) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  double start, end;

  int* image = nullptr;
  const int width = 200, height = 400;

  if (rank == 0) {
    image = getRandomImage(width, height);
  }
  start = MPI_Wtime();
  int* new_image_parallel = getParallelSobel(image, width, height);
  end = MPI_Wtime();

  if (rank == 0) {
    double ptime = end - start;
    start = MPI_Wtime();
    int* reference_new_image = getSequentialSobel(image, width, height);
    end = MPI_Wtime();
    double stime = end - start;

    std::cout << "P time " << ptime << std::endl;
    std::cout << "S time " << stime << std::endl;
    std::cout << "Speedup " << stime / ptime << std::endl;

    for (int i = 0; i < (width - 2) * (height - 2); i += (width - 2)) {
      ASSERT_EQ(new_image_parallel[i], reference_new_image[i]);
    }
  }
}

TEST(PARALLEL_SOBEL_MPI, TEST_4) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  double start, end;

  int* image = nullptr;
  const int width = 123, height = 456;

  if (rank == 0) {
    image = getRandomImage(width, height);
  }
  start = MPI_Wtime();
  int* new_image_parallel = getParallelSobel(image, width, height);
  end = MPI_Wtime();

  if (rank == 0) {
    double ptime = end - start;
    start = MPI_Wtime();
    int* reference_new_image = getSequentialSobel(image, width, height);
    end = MPI_Wtime();
    double stime = end - start;

    std::cout << "P time " << ptime << std::endl;
    std::cout << "S time " << stime << std::endl;
    std::cout << "Speedup " << stime / ptime << std::endl;

    for (int i = 0; i < (width - 2) * (height - 2); i += (width - 2)) {
      ASSERT_EQ(new_image_parallel[i], reference_new_image[i]);
    }
  }
}

TEST(PARALLEL_SOBEL_MPI, TEST_5) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  double start, end;

  int* image = nullptr;
  const int width = 333, height = 444;

  if (rank == 0) {
    image = getRandomImage(width, height);
  }
  start = MPI_Wtime();
  int* new_image_parallel = getParallelSobel(image, width, height);
  end = MPI_Wtime();

  if (rank == 0) {
    double ptime = end - start;
    start = MPI_Wtime();
    int* reference_new_image = getSequentialSobel(image, width, height);
    end = MPI_Wtime();
    double stime = end - start;

    std::cout << "P time " << ptime << std::endl;
    std::cout << "S time " << stime << std::endl;
    std::cout << "Speedup " << stime / ptime << std::endl;

    for (int i = 0; i < (width - 2) * (height - 2); i += (width - 2)) {
      ASSERT_EQ(new_image_parallel[i], reference_new_image[i]);
    }
  }
}

int main(int argc, char** argv) {
  MPI_Init(&argc, &argv);
     int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::TestEventListeners& listeners = ::testing::UnitTest::GetInstance()->listeners();
    if (rank != 0) {
        delete listeners.Release(listeners.default_result_printer());
    }
    int result = RUN_ALL_TESTS();

    MPI_Finalize();

    return result;
}
