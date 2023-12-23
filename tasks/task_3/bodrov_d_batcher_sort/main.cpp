// Copyright 2023 Bodrov Daniil
#include <gtest/gtest.h>
#include "./batcher_sort.h"

TEST(ParallelBatcherSortTest, Test_Sorted_Vector) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> testVect = {0, 1, 2, 3, 4, 5, 7, 10, 11, 13, 23, 44, 100};
    std::vector<int> refVect = testVect;
    double t1, t2;
    if (rank == 0) {
        t1 = MPI_Wtime();
    }
    parallelBatcherSort(&testVect, 0, testVect.size() - 1);
    if (rank == 0) {
        t2 = MPI_Wtime();
        std::cout << "Time : " << t2 - t1 << std::endl;
        batcherSort(&refVect, 0, refVect.size() - 1);
        EXPECT_EQ(testVect, refVect);
    }
}

TEST(ParallelBatcherSortTest, Test_Reverse_Sorted_Vector) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> testVect = {100, 44, 23, 13, 11, 10, 7, 5, 4, 3, 2, 1, 0};
    std::vector<int> refVect = testVect;
    double t1, t2;
    if (rank == 0) {
        t1 = MPI_Wtime();
    }
    parallelBatcherSort(&testVect, 0, testVect.size() - 1);
    if (rank == 0) {
        t2 = MPI_Wtime();
        std::cout << "Time : " << t2 - t1 << std::endl;
        batcherSort(&refVect, 0, refVect.size() - 1);
        EXPECT_EQ(testVect, refVect);
    }
}


TEST(ParallelBatcherSortTest, Test_Entered_Vector) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> testVect = {5, 11, 23, 2, 4, 1, 44, 3, 0, 10, 13, 7, 100};
    std::vector<int> refVect = testVect;
    double t1, t2;
    if (rank == 0) {
        t1 = MPI_Wtime();
    }
    parallelBatcherSort(&testVect, 0, testVect.size() - 1);
    if (rank == 0) {
        t2 = MPI_Wtime();
        std::cout << "Time : " << t2 - t1 << std::endl;
        batcherSort(&refVect, 0, refVect.size() - 1);
        EXPECT_EQ(testVect, refVect);
    }
}

TEST(ParallelBatcherSortTest, Test_Negative_Elements) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> testVect = {-5, -1, -4, -2, -8, -9, -3, -6, -7};
    std::vector<int> refVect = testVect;
    double t1, t2;
    if (rank == 0) {
        t1 = MPI_Wtime();
    }
    parallelBatcherSort(&testVect, 0, testVect.size() - 1);
    if (rank == 0) {
        t2 = MPI_Wtime();
        std::cout << "Time : " << t2 - t1 << std::endl;
        batcherSort(&refVect, 0, refVect.size() - 1);
        EXPECT_EQ(testVect, refVect);
    }
}

TEST(ParallelBatcherSortTest, Test_Repeated_Elements) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> testVect = {5, 3, 4, 5, 1, 2, 5, 3, 2, 1, 4};
    std::vector<int> refVect = testVect;
    double t1, t2;
    if (rank == 0) {
        t1 = MPI_Wtime();
    }
    parallelBatcherSort(&testVect, 0, testVect.size() - 1);
    if (rank == 0) {
        t2 = MPI_Wtime();
        std::cout << "Time : " << t2 - t1 << std::endl;
        batcherSort(&refVect, 0, refVect.size() - 1);
        EXPECT_EQ(testVect, refVect);
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
