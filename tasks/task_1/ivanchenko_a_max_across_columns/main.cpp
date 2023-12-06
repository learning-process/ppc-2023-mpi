// Copyright 2023 Ivanchenko Aleksei
#include <gtest/gtest.h>
#include <vector>
#include <boost/mpi/environment.hpp>
#include <boost/mpi/communicator.hpp>
#include "task_1/ivanchenko_a_max_across_columns/max_across_columns.h"

TEST(MAX_ACROSS_COLUMN, sequentional_positive_matrix) {
    boost::mpi::communicator world;
    std::vector<int> matrix = {1, 2, 3,
                               4, 5, 6,
                               7, 8, 9,
                               1, 0, 0};
    std::vector<int> expected = {7, 8, 9};
    if (world.rank() == 0) {
        EXPECT_EQ(expected, getMaxSequentional(matrix, 4, 3));
    }
}
TEST(MAX_ACROSS_COLUMN, sequentional_negative_matrix) {
    boost::mpi::communicator world;
    std::vector<int> matrix = {-1, -2, -3,
                               -4, -5, -6,
                               -7, -8, -9,
                               -1, 0, 0};
    std::vector<int> expected = {-1, 0, 0};
    if (world.rank() == 0) {
        EXPECT_EQ(expected, getMaxSequentional(matrix, 4, 3));
    }
}
TEST(MAX_ACROSS_COLUMN, sequentional_big_integers) {
    boost::mpi::communicator world;
    std::vector<int> matrix = {-100000,  2000,     -300000000,
                               -200000, -5000,     -700000000,
                               -700000,  81200001, -900000000,
                               -300000, -21302,    -200000000,
                               -120000,  81200000, -121332221};

    std::vector<int> expected = {-100000, 81200001,-121332221};
    if (world.rank() == 0) {
        EXPECT_EQ(expected, getMaxSequentional(matrix, 5, 3));
    }
}
TEST(MAX_ACROSS_COLUMN, parallel_small_matrix) {
    boost::mpi::communicator world;
    std::vector<int> vec;
    const size_t vec_size = 25;

    if (world.rank() == 0) {
        vec = {1, 2, 3, 4, 5,
               6, 7, 8, 9, 10,
               1, 2, 3, 4, 5,
               16, 17, 18, 19, 110,
               1, 2, 3, 4, 5};
    }

    std::vector<int> parallel_res = getMaxParallel(vec, vec_size / 5 , 5);

    if (world.rank() == 0) {
        std::vector<int> seq_res(5);
        seq_res = getMaxSequentional(vec, 5, 5);
        EXPECT_EQ(seq_res, parallel_res);
    }
}
TEST(MAX_ACROSS_COLUMN, parallel_matrix_11x11_positive) {
    boost::mpi::communicator world;
    std::vector<int> vec;
    size_t rows = 11, columns = 11;

    if (world.rank() == 0) {
        vec = getRandomMatrix(rows, columns, 0, 1000);
    }

    std::vector<int> parallel_res = getMaxParallel(vec, rows , columns);

    if (world.rank() == 0) {
        std::vector<int> seq_res(columns);
        seq_res = getMaxSequentional(vec, rows, columns);
        EXPECT_EQ(seq_res, parallel_res);
    }
}
TEST(MAX_ACROSS_COLUMN, parallel_matrix_11x11_negative) {
    boost::mpi::communicator world;
    std::vector<int> vec;
    size_t rows = 11, columns = 11;

    if (world.rank() == 0) {
        vec = getRandomMatrix(rows, columns, -1000, 0);
    }

    std::vector<int> parallel_res = getMaxParallel(vec, rows , columns);

    if (world.rank() == 0) {
        std::vector<int> seq_res(columns);
        seq_res = getMaxSequentional(vec, rows, columns);
        EXPECT_EQ(seq_res, parallel_res);
    }
}
TEST(MAX_ACROSS_COLUMN, parallel_matrix_11x11) {
    boost::mpi::communicator world;
    std::vector<int> vec;
    size_t rows = 11, columns = 11;

    if (world.rank() == 0) {
        vec = getRandomMatrix(rows, columns, -1000, 1000);
    }

    std::vector<int> parallel_res = getMaxParallel(vec, rows , columns);

    if (world.rank() == 0) {
        std::vector<int> seq_res(columns);
        seq_res = getMaxSequentional(vec, rows, columns);
        EXPECT_EQ(seq_res, parallel_res);
    }
}
TEST(MAX_ACROSS_COLUMN, parallel_matrix_17x17) {
    boost::mpi::communicator world;
    std::vector<int> vec;
    size_t rows = 17, columns = 17;

    if (world.rank() == 0) {
        vec = getRandomMatrix(rows, columns, -1000, 1000);
    }

    std::vector<int> parallel_res = getMaxParallel(vec, rows , columns);

    if (world.rank() == 0) {
        std::vector<int> seq_res(columns);
        seq_res = getMaxSequentional(vec, rows, columns);
        EXPECT_EQ(seq_res, parallel_res);
    }
}
TEST(MAX_ACROSS_COLUMN, parallel_matrix_24x24) {
    boost::mpi::communicator world;
    std::vector<int> vec;
    size_t rows = 24, columns = 24;

    if (world.rank() == 0) {
        vec = getRandomMatrix(rows, columns, -1000, 1000);
    }

    std::vector<int> parallel_res = getMaxParallel(vec, rows , columns);

    if (world.rank() == 0) {
        std::vector<int> seq_res(columns);
        seq_res = getMaxSequentional(vec, rows, columns);
        EXPECT_EQ(seq_res, parallel_res);
    }
}
TEST(MAX_ACROSS_COLUMN, parallel_matrix_144x73) {
    boost::mpi::communicator world;
    std::vector<int> vec;
    size_t rows = 24, columns = 17;

    if (world.rank() == 0) {
        vec = getRandomMatrix(rows, columns, -1000, 1000);
    }

    std::vector<int> parallel_res = getMaxParallel(vec, rows , columns);

    if (world.rank() == 0) {
        std::vector<int> seq_res(columns);
        seq_res = getMaxSequentional(vec, rows, columns);
        EXPECT_EQ(seq_res, parallel_res);
    }
}
TEST(MAX_ACROSS_COLUMN, parallel_matrix_73x144) {
    boost::mpi::communicator world;
    std::vector<int> vec;
    size_t rows = 17, columns = 24;

    if (world.rank() == 0) {
        vec = getRandomMatrix(rows, columns, -1000, 1000);
    }

    std::vector<int> parallel_res = getMaxParallel(vec, rows , columns);

    if (world.rank() == 0) {
        std::vector<int> seq_res(columns);
        seq_res = getMaxSequentional(vec, rows, columns);
        EXPECT_EQ(seq_res, parallel_res);
    }
}

int main(int argc, char** argv) {
    boost::mpi::environment env(argc, argv);
    boost::mpi::communicator world;
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::TestEventListeners& listeners = ::testing::UnitTest::GetInstance()->listeners();
    if (world.rank() != 0) {
        delete listeners.Release(listeners.default_result_printer());
    }
    return RUN_ALL_TESTS();
}