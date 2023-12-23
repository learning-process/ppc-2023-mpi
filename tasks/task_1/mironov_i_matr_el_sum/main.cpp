// Copyright 2023 Mironov Ilya
#include <gtest/gtest.h>
#include <boost/mpi/environment.hpp>
#include <boost/mpi/communicator.hpp>
#include "./matr_el_sum.h"



TEST(SumOfMatrixElements, signSumOfPositive) {
    boost::mpi::communicator world;
    int n = 3, m = 7;
    std::vector<int> matr;
    if (world.rank() == 0) {
        matr = get_random_matrix(n * m, 0, 50);
    }
    int res = get_parallel_matr_el_sum(matr, n * m);
    if (world.rank() == 0) {
        ASSERT_GE(res, 0);
    }
}

TEST(SumOfMatrixElements, signSumOfNegative) {
    boost::mpi::communicator world;
    int n = 3, m = 7;
    std::vector<int> matr;
    if (world.rank() == 0) {
        matr = get_random_matrix(n * m, -50, 0);
    }
    int res = get_parallel_matr_el_sum(matr, n * m);
    if (world.rank() == 0) {
        ASSERT_LE(res, 0);
    }
}

TEST(SumOfMatrixElements, SumOfPositive) {
    boost::mpi::communicator world;
    int n = 2, m = 4;
    std::vector<int> matr;
    if (world.rank() == 0) {
        matr = get_random_matrix(n * m, 0, 100);
    }
    int res = get_parallel_matr_el_sum(matr, n * m);
    if (world.rank() == 0) {
        int seq_res = get_sequential_matr_el_sum(matr);
        ASSERT_EQ(seq_res, res);
    }
}

TEST(SumOfMatrixElements, SumOfNegative) {
    boost::mpi::communicator world;
    int n = 2, m = 4;
    std::vector<int> matr;
    if (world.rank() == 0) {
        matr = get_random_matrix(n * m, -100, 0);
    }
    int res = get_parallel_matr_el_sum(matr, n * m);
    if (world.rank() == 0) {
        int seq_res = get_sequential_matr_el_sum(matr);
        ASSERT_EQ(seq_res, res);
    }
}

TEST(SumOfMatrixElements, RandomSum) {
    boost::mpi::communicator world;
    int n = 2, m = 4;
    std::vector<int> matr;
    if (world.rank() == 0) {
        matr = get_random_matrix(n * m, -1000, 1000);
    }
    int res = get_parallel_matr_el_sum(matr, n * m);
    if (world.rank() == 0) {
        int seq_res = get_sequential_matr_el_sum(matr);
        ASSERT_EQ(seq_res, res);
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
