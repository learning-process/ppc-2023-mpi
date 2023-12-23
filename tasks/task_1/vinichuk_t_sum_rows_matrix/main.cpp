// Copyright 2023 Vinichuk Timofey
#include <gtest/gtest.h>
#include <vector>
#include "./matrix_row_sum.h"
#include <boost/mpi/environment.hpp>
#include <boost/mpi/communicator.hpp>
#include "task_1/vinichuk_t_sum_rows_matrix/matrix_row_sum.h"
TEST(Parallel_Operations_MPI, Test_Sum1) {
    boost::mpi::communicator world;
    std::vector<int> matrix;

    if (world.rank() == 0) {
        matrix = {  1, 2, 3,
                    3, 6, 8,
                    4, 1, 5};
    }

    auto global_sum = ParallelSumRowsMatrix(matrix, 3);

    if (world.rank() == 0) {
        auto reference_sum = SeqSumRowsMatrix(matrix, 3);
        ASSERT_EQ(reference_sum, global_sum);
    }
}

TEST(Parallel_Operations_MPI, Test_Sum2) {
    boost::mpi::communicator world;
    std::vector<int> matrix;


    if (world.rank() == 0) {
        matrix = RandomVector(20, 20);
    }

    auto global_sum = ParallelSumRowsMatrix(matrix, 20);

    if (world.rank() == 0) {
        auto reference_sum = SeqSumRowsMatrix(matrix, 20);
        ASSERT_EQ(reference_sum, global_sum);
    }
}

TEST(Parallel_Operations_MPI, Test_Sum3) {
    boost::mpi::communicator world;
    std::vector<int> matrix;


    if (world.rank() == 0) {
        matrix = RandomVector(40, 40);
    }

    auto global_sum = ParallelSumRowsMatrix(matrix, 40);

    if (world.rank() == 0) {
        auto reference_sum = SeqSumRowsMatrix(matrix, 40);
        ASSERT_EQ(reference_sum, global_sum);
    }
}

TEST(Parallel_Operations_MPI, Test_Sum4) {
    boost::mpi::communicator world;
    std::vector<int> matrix;


    if (world.rank() == 0) {
        matrix = RandomVector(50, 50);
    }

    auto global_sum = ParallelSumRowsMatrix(matrix, 50);

    if (world.rank() == 0) {
        auto reference_sum = SeqSumRowsMatrix(matrix, 50);
        ASSERT_EQ(reference_sum, global_sum);
    }
}

TEST(Parallel_Operations_MPI, Test_Sum5) {
    boost::mpi::communicator world;
    std::vector<int> matrix;


    if (world.rank() == 0) {
        matrix = RandomVector(100, 100);
    }

    auto global_sum = ParallelSumRowsMatrix(matrix, 100);

    if (world.rank() == 0) {
        auto reference_sum = SeqSumRowsMatrix(matrix, 100);
        ASSERT_EQ(reference_sum, global_sum);
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
