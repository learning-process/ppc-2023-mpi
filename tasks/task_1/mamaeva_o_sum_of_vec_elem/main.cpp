// Copyright 2023 Mamaeva Olga
#include <gtest/gtest.h>
#include "../tasks/task_1/mamaeva_o_sum_of_vec_elem/sum_of_vec_elem.h"


const int n = 1;

TEST(Parallel_Operations_MPI, Test_Sum) {
    int ProcRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    std::vector<double> vec;
    if (ProcRank == 0) {
        vec = getRandomVector(n);
    }

    double sum_all = getParallelOperations(vec, n, "+");

    if (ProcRank == 0) {
        double sum = getSequentialOperations(vec, n, "+");
        ASSERT_EQ(sum, sum_all);
    }
}

TEST(Parallel_Operations_MPI, Test_Sum_2) {
    int ProcRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    std::vector<double> vec;
    if (ProcRank == 0) {
        vec = getRandomVector(n + 1);
    }

    double sum_all = getParallelOperations(vec, n + 1, "+");

    if (ProcRank == 0) {
        double sum = getSequentialOperations(vec, n + 1, "+");
        ASSERT_EQ(sum, sum_all);
    }
}

TEST(Parallel_Operations_MPI, Test_Sum_3) {
     int ProcRank;
     MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
     std::vector<double> vec;
     if (ProcRank == 0) {
         vec = getRandomVector(n + 2);
     }

     double sum_all = getParallelOperations(vec, n + 2, "+");

     if (ProcRank == 0) {
         double sum = getSequentialOperations(vec, n + 2, "+");
         ASSERT_EQ(sum, sum_all);
     }
}

TEST(Parallel_Operations_MPI, Test_Diff) {
    int ProcRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    std::vector<double> vec;
    if (ProcRank == 0) {
        vec = getRandomVector(n + 3);
    }

    double diff_all = getParallelOperations(vec, n + 3, "-");

    if (ProcRank == 0) {
        double diff = getSequentialOperations(vec, n + 3, "-");
        ASSERT_EQ(diff, diff_all);
    }
}

TEST(Parallel_Operations_MPI, Test_Diff_2) {
    int ProcRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    std::vector<double> vec;
    if (ProcRank == 0) {
        vec = getRandomVector(n + 4);
    }

    double diff_all = getParallelOperations(vec, n + 4, "-");

    if (ProcRank == 0) {
        double diff = getSequentialOperations(vec, n + 4, "-");
        ASSERT_EQ(diff, diff_all);
    }
}

TEST(Parallel_Operations_MPI, Test_Diff_3) {
     int ProcRank;
     MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
     std::vector<double> vec;
     if (ProcRank == 0) {
         vec = getRandomVector(n + 5);
     }

     double diff_all = getParallelOperations(vec, n + 5, "-");

     if (ProcRank == 0) {
         double diff = getSequentialOperations(vec, n + 5, "-");
         ASSERT_EQ(diff, diff_all);
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

