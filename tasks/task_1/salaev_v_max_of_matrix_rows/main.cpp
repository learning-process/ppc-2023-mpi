// Copyright 2023 Salaev Vladislav
#include <gtest/gtest.h>
#include <vector>

#include <boost/mpi/communicator.hpp>
#include <boost/mpi/environment.hpp>
#include <boost/serialization/vector.hpp>

#include "./max_of_matrix_rows.h"

TEST(Parallel_Max_Of_Matrix_Rows_MPI, Test_Random) {
    boost::mpi::communicator world;
    std::vector<int> global_matr;
    const int m = 24;
    const int n = 15;

    if (world.rank() == 0) {
        global_matr = getRandomMatrix(m, n);
    }

    std::vector<size_t> global_maxes = getParallelMaxInRows(global_matr, m, n);

    if (world.rank() == 0) {
        std::vector<size_t> reference_maxes = getSequentialMaxInRows(
            global_matr, m);
        for (int i = 0; i < reference_maxes.size(); ++i)
            ASSERT_EQ(reference_maxes[i], global_maxes[i]);
    }
}


TEST(Parallel_Max_Of_Matrix_Rows_MPI, Test_Manual) {
    boost::mpi::communicator world;
    std::vector<int> global_matr;
    const int m = 4;
    const int n = 5;

    if (world.rank() == 0) {
        global_matr = {
            10, -1, 5, 2, -5,
            123, 1, 0, 6, 4,
            -1, 12, 16, 4, 5,
            100, 150, 200, 15, 9
        };
    }

    std::vector<size_t> global_maxes = getParallelMaxInRows(global_matr, m, n);

    if (world.rank() == 0) {
        ASSERT_EQ(0, global_maxes[0]);
        ASSERT_EQ(0, global_maxes[1]);
        ASSERT_EQ(2, global_maxes[2]);
        ASSERT_EQ(2, global_maxes[3]);
    }
}

TEST(Parallel_Max_Of_Matrix_Rows_MPI, Test_ManualInOneRow) {
    boost::mpi::communicator world;
    std::vector<int> global_matr;
    const int m = 1;
    const int n = 20;

    if (world.rank() == 0) {
        global_matr = {
            10, -1, 5, 2, -5,
            123, 1, 0, 6, 4,
            -1, 12, 16, 4, 5,
            100, 150, 200, 15, 9
        };
    }

    std::vector<size_t> global_maxes = getParallelMaxInRows(global_matr, m, n);

    if (world.rank() == 0) {
        ASSERT_EQ(17, global_maxes[0]);
    }
}

TEST(Parallel_Max_Of_Matrix_Rows_MPI, Test_ManualAllEQ) {
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

    std::vector<size_t> global_maxes = getParallelMaxInRows(global_matr, m, n);

    if (world.rank() == 0) {
        ASSERT_EQ(0, global_maxes[0]);
        ASSERT_EQ(0, global_maxes[1]);
        ASSERT_EQ(0, global_maxes[2]);
        ASSERT_EQ(0, global_maxes[3]);
    }
}

TEST(Parallel_Max_Of_Matrix_Rows_MPI, Test_NegativeNumbers) {
    boost::mpi::communicator world;
    std::vector<int> global_matr;
    const int m = 3;
    const int n = 4;

    if (world.rank() == 0) {
        global_matr = {
            -1, -2, -3, -4,
            -8, -7, -6, -5,
            -10, -9, -12, -11
        };
    }

    std::vector<size_t> global_maxes = getParallelMaxInRows(global_matr, m, n);

    if (world.rank() == 0) {
        ASSERT_EQ(0, global_maxes[0]);
        ASSERT_EQ(3, global_maxes[1]);
        ASSERT_EQ(1, global_maxes[2]);
    }
}

int main(int argc, char** argv) {
    boost::mpi::environment env(argc, argv);
    boost::mpi::communicator world;
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::TestEventListeners& listeners = ::testing::UnitTest:
    :GetInstance()->listeners();
    if (world.rank() != 0) {
        delete listeners.Release(listeners.default_result_printer());
    }
    return RUN_ALL_TESTS();
}
