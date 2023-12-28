// Copyright 2023 Platonova Maria
#include <gtest/gtest.h>
#include <vector>

#include <boost/mpi/communicator.hpp>
#include <boost/mpi/environment.hpp>
#include <boost/serialization/vector.hpp>

#include "./min_matrix_rows.h"

TEST(Parallel_Min_In_Matrix_MPI, Test_Random) {
    boost::mpi::communicator world;
    std::vector<int> global_matr;
    const int m = 24;
    const int n = 15;

    if (world.rank() == 0) {
        global_matr = getRandomMatrix(m, n);
    }

    std::vector<size_t> global_sum = getParallelMin(global_matr, m, n);

    if (world.rank() == 0) {
        std::vector<size_t> reference_sum = getSequentialMin(global_matr, m);
        for (int i = 0; i < reference_sum.size(); ++i)
            ASSERT_EQ(reference_sum[i], global_sum[i]);
    }
}

TEST(Parallel_Min_In_Matrix_MPI, Test_Random2) {
    boost::mpi::communicator world;
    std::vector<int> global_matr;
    const int m = 24;
    const int n = 15;

    if (world.rank() == 0) {
        global_matr = getRandomMatrix(m, n);
    }

    std::vector<size_t> global_sum = getParallelMin(global_matr, m, n);

    if (world.rank() == 0) {
        std::vector<size_t> reference_sum = getSequentialMin(global_matr, m);
        for (int i = 0; i < reference_sum.size(); ++i)
            ASSERT_EQ(reference_sum[i], global_sum[i]);
    }
}

TEST(Parallel_Min_In_Matrix_MPI, Test_Manual) {
    boost::mpi::communicator world;
    std::vector<int> global_matr;
    const int m = 4;
    const int n = 5;

    if (world.rank() == 0) {
        global_matr = {
            0, -1, 3, 42, -5,
            6, 2, 2, 3, 4,
            -1, -3, 1, 3, 5,
            100, 50, 20, 15, 1};
    }

    std::vector<size_t> global_sum = getParallelMin(global_matr, m, n);

    if (world.rank() == 0) {
        ASSERT_EQ(4, global_sum[0]);
        ASSERT_EQ(6, global_sum[1]);
        ASSERT_EQ(11, global_sum[2]);
        ASSERT_EQ(19, global_sum[3]);
    }
}

TEST(Parallel_Min_In_Matrix_MPI, Test_ManualOneRow) {
    boost::mpi::communicator world;
    std::vector<int> global_matr;
    const int m = 1;
    const int n = 20;

    if (world.rank() == 0) {
        global_matr = {
            0, -1, 3, 42, -5,
            6, 2, 2, 3, 4,
            -1, -3, 1, 3, 5,
            100, 50, 20, 15, -100};
    }

    std::vector<size_t> global_sum = getParallelMin(global_matr, m, n);

    if (world.rank() == 0) {
        ASSERT_EQ(19, global_sum[0]);
    }
}

TEST(Parallel_Min_In_Matrix_MPI, Test_ManualAllEQ) {
    boost::mpi::communicator world;
    std::vector<int> global_matr;
    const int m = 4;
    const int n = 5;

    if (world.rank() == 0) {
        global_matr = {
            42, 42, 42, 42, 42,
            42, 42, 42, 42, 42,
            42, 42, 42, 42, 42,
            42, 42, 42, 42, 42};
    }

    std::vector<size_t> global_sum = getParallelMin(global_matr, m, n);

    if (world.rank() == 0) {
        ASSERT_EQ(0, global_sum[0]);
        ASSERT_EQ(5, global_sum[1]);
        ASSERT_EQ(10, global_sum[2]);
        ASSERT_EQ(15, global_sum[3]);
    }
}

int main(int argc, char** argv) {
    boost::mpi::environment env(argc, argv);
    boost::mpi::communicator world;
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::TestEventListeners& listeners =
        ::testing::UnitTest::GetInstance()->listeners();
    if (world.rank() != 0) {
        delete listeners.Release(listeners.default_result_printer());
    }
    return RUN_ALL_TESTS();
}
