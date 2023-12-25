// Copyright 2023 Filatov Maxim

#include <gtest/gtest.h>
#include "task_1/filatov_m_max_value_by_matrix_columns/max_value_matrix_by_columns.h"
#include <boost/mpi/communicator.hpp>
#include <boost/mpi/collectives.hpp>

TEST(MPI_TESTS, parallel_matrix_3x3) {
    boost::mpi::communicator communicator;
    constexpr std::size_t vec_size = 9;
    constexpr std::size_t size = 3;
    std::vector<int> vec;
    if (!communicator.rank()) {
        vec = { 1, 2, 3,
                4, 5, 6,
                1, 2, 3 };
    }
    std::vector<int> pres = calculateParallelMaxInColumns(vec, size, size);
    if (!communicator.rank()) {
        std::vector<int> res(size);
        res = findMaxInColumns(vec, size, size);
        EXPECT_EQ(res, pres);
    }
}

TEST(MPI_TESTS, test_parallel_negative_matrix_3x3) {
    boost::mpi::communicator communicator;
    constexpr std::size_t vec_size = 9;
    constexpr std::size_t size = 3;
    std::vector<int> vec;
    if (!communicator.rank()) {
        vec = { -1, -2, -3,
                -4, -5, -6,
                -1, -2, -3 };
    }
    std::vector<int> pres = calculateParallelMaxInColumns(vec, size, size);
    if (!communicator.rank()) {
        std::vector<int> res(size);
        res = findMaxInColumns(vec, size, size);
        EXPECT_EQ(res, pres);
    }
}

TEST(MPI_TESTS, test_random_matrix_3x3) {
    boost::mpi::communicator communicator;
    constexpr std::size_t row = 3;
    constexpr std::size_t col = 3;
    std::vector<int>vec;
    if (!communicator.rank()) {
        vec = generateRandomMatrix(row, col);
    }
    std::vector<int>pres = calculateParallelMaxInColumns(vec, row, col);
    if (!communicator.rank()) {
        std::vector<int> res = findMaxInColumns(vec, row, col);
        ASSERT_EQ(res, pres);
    }
}

TEST(MPI_TESTS, test_random_matrix_7x9) {
    boost::mpi::communicator communicator;
    constexpr std::size_t row = 7;
    constexpr std::size_t col = 9;
    std::vector<int>matrix;
    if (!communicator.rank()) {
        matrix = generateRandomMatrix(row, col);
    }
    std::vector<int>pres = calculateParallelMaxInColumns(matrix, row, col);
    if (!communicator.rank()) {
        std::vector<int> res = findMaxInColumns(matrix, row, col);
        ASSERT_EQ(res, pres);
    }
}

TEST(MPI_TESTS, test_random_matrix_16x8) {
    boost::mpi::communicator communicator;
    constexpr std::size_t row = 16;
    constexpr std::size_t col = 8;
    std::vector<int>matrix;
    if (!communicator.rank()) {
        matrix = generateRandomMatrix(row, col);
    }
    std::vector<int>pres = calculateParallelMaxInColumns(matrix, row, col);
    if (!communicator.rank()) {
        std::vector<int> res = findMaxInColumns(matrix, row, col);
        ASSERT_EQ(res, pres);
    }
}

TEST(MPI_TESTS, test_matrix_zero_3x3) {
    boost::mpi::communicator communicator;
    constexpr std::size_t row = 3;
    constexpr std::size_t col = 3;
    std::vector<int>matrix;
    if (!communicator.rank()) {
        matrix.assign(row*col, 0);
    }
    std::vector<int>pres = calculateParallelMaxInColumns(matrix, row, col);
    if (!communicator.rank()) {
        std::vector<int> res = findMaxInColumns(matrix, row, col);
        ASSERT_EQ(res, pres);
    }
}

TEST(MPI_TESTS, test_matrix_negative_3x3) {
    boost::mpi::communicator communicator;
    constexpr std::size_t row = 3;
    constexpr std::size_t col = 3;
    std::vector<int>matrix;
    if (!communicator.rank()) {
        matrix = generateRandomMatrix(row, col);
        for (auto num : matrix) {
            num *= -1;
        }
    }
    std::vector<int>pres = calculateParallelMaxInColumns(matrix, row, col);
    if (!communicator.rank()) {
        std::vector<int> res = findMaxInColumns(matrix, row, col);
        ASSERT_EQ(res, pres);
    }
}

int main(int argc, char** argv) {
    boost::mpi::environment env(argc, argv);
    boost::mpi::communicator communicator;
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::TestEventListeners& listeners =
        ::testing::UnitTest::GetInstance()->listeners();
    if (communicator.rank()) {
        delete listeners.Release(listeners.default_result_printer());
    }
    return RUN_ALL_TESTS();
}
