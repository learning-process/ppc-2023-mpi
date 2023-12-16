// Copyright 2023 Kostanyan Arsen
#include <gtest/gtest.h>
#include <string>
#include <random>
#include <iostream>
#include "task_3/kostanyan_a_radix_sort_batcher/batcher_merge.h"
#include "task_3/kostanyan_a_radix_sort_batcher/radix_batcher.h"

TEST(TEST_MPI, Test_Radix_Sort) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        int vec_size = 30;
        std::vector<double> unsorted_vec = generateRandomVector(vec_size * vec_size);
        radix_sort(unsorted_vec.data(), unsorted_vec.size());
        for (int i = 0; i < unsorted_vec.size() - 1; i++) {
            ASSERT_LE(unsorted_vec[i], unsorted_vec[i+1]);
        }
    }
}

TEST(TEST_MPI, Test_Batcher_Merge_Not_Power_2) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int vec_size = 200;
    std::vector<double> unsorted_vec = generateRandomVector(size * vec_size);
    std::vector<double> sorted_vec = parallel_radix_batcher_sort(unsorted_vec);
    if (rank == 0) {
        for (int i = 0; i < sorted_vec.size() - 1; i++) {
            ASSERT_LE(sorted_vec[i], sorted_vec[i+1]);
        }
    }
}


TEST(TEST_MPI, Test_Batcher__Power_2) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int vec_size = 256;
    std::vector<double> unsorted_vec = generateRandomVector(size * vec_size);
    std::vector<double> sorted_vec = parallel_radix_batcher_sort(unsorted_vec);
    if (rank == 0) {
        for (int i = 0; i < sorted_vec.size() - 1; i++) {
            ASSERT_LE(sorted_vec[i], sorted_vec[i+1]);
        }
    }
}

TEST(TEST_MPI, Test_If_Merge_1) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int vec_size = 10;
    std::vector<double> unsorted_vec = generateRandomVector(size * vec_size);
    std::vector<double> sorted_vec = parallel_radix_batcher_sort(unsorted_vec);
    if (rank == 0) {
        for (int i = 0; i < sorted_vec.size() - 1; i++) {
            ASSERT_LE(sorted_vec[i], sorted_vec[i+1]);
        }
    }
}

TEST(TEST_MPI, Test_Long) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int vec_size = 1500;
    std::vector<double> unsorted_vec = generateRandomVector(size * vec_size);
    std::vector<double> sorted_vec = parallel_radix_batcher_sort(unsorted_vec);
    if (rank == 0) {
        for (int i = 0; i < sorted_vec.size() - 1; i++) {
            ASSERT_LE(sorted_vec[i], sorted_vec[i+1]);
        }
    }
}


int main(int argc, char **argv) {
    int result_code = 0;

    ::testing::InitGoogleTest(&argc, argv);
    ::testing::TestEventListeners &listeners =
        ::testing::UnitTest::GetInstance()->listeners();

    if (MPI_Init(&argc, &argv) != MPI_SUCCESS)
        MPI_Abort(MPI_COMM_WORLD, -1);
    result_code = RUN_ALL_TESTS();
    MPI_Finalize();

    return result_code;
}
