// Copyright 2023 Khavronina Alexandra

#include <gtest/gtest.h>
#include <vector>

#include <boost/mpi/communicator.hpp>
#include <boost/mpi/environment.hpp>
#include <boost/serialization/vector.hpp>

#include "./min_matrix.h"


TEST(Min_value_in_matrix_rows_MPI, Random_Value_Test_1) {
    boost::mpi::communicator world;
    std::vector<int> global_matrix;
    const int m = 24;
    const int n = 15;

    if (world.rank() == 0) {
        global_matrix = gen_random_matrix(m, n);
    }

    std::vector<size_t> global_sum = find_min_values_in_matrix_rows_parallel(global_matrix, m, n);

    if (world.rank() == 0) {
        std::vector<size_t> reference_sum = get_min_values_in_matrix_rows(global_matrix, m);
        for (int i = 0; i < reference_sum.size(); i++)
            ASSERT_EQ(reference_sum[i], global_sum[i]);
    }
}

TEST(Min_value_in_matrix_rows_MPI, Random_Value_Test_2) {
    boost::mpi::communicator world;
    std::vector<int> global_matrix;
    const int m = 24;
    const int n = 15;

    if (world.rank() == 0) {
        global_matrix = gen_random_matrix(m, n);
    }

    std::vector<size_t> global_sum = find_min_values_in_matrix_rows_parallel(global_matrix, m, n);

    if (world.rank() == 0) {
        std::vector<size_t> reference_sum = get_min_values_in_matrix_rows(global_matrix, m);
        for (int i = 0; i < reference_sum.size(); i++)
            ASSERT_EQ(reference_sum[i], global_sum[i]);
    }
}

TEST(Min_value_in_matrix_rows_MPI, Not_Random_Value_Test) {
    boost::mpi::communicator world;
    std::vector<int> global_matrix;
    const int m = 4;
    const int n = 5;

    if (world.rank() == 0) {
        global_matrix = {
            0, -1, 3, 42, -5,
            6, 2, 2, 3, 4,
            -1, -3, 1, 3, 5,
            100, 50, 20, 15, 1};
    }

    std::vector<size_t> global_sum = find_min_values_in_matrix_rows_parallel(global_matrix, m, n);

    if (world.rank() == 0) {
        ASSERT_EQ(4, global_sum[0]);
        ASSERT_EQ(6, global_sum[1]);
        ASSERT_EQ(11, global_sum[2]);
        ASSERT_EQ(19, global_sum[3]);
    }
}

TEST(Min_value_in_matrix_rows_MPI, Equal_Values_Test) {
    boost::mpi::communicator world;
    std::vector<int> global_matrix;
    const int m = 4;
    const int n = 5;

    if (world.rank() == 0) {
        global_matrix = {
            42, 42, 42, 42, 42,
            42, 42, 42, 42, 42,
            42, 42, 42, 42, 42,
            42, 42, 42, 42, 42};
    }

    std::vector<size_t> global_sum = find_min_values_in_matrix_rows_parallel(global_matrix, m, n);

    if (world.rank() == 0) {
        ASSERT_EQ(0, global_sum[0]);
        ASSERT_EQ(5, global_sum[1]);
        ASSERT_EQ(10, global_sum[2]);
        ASSERT_EQ(15, global_sum[3]);
    }
}

TEST(Min_value_in_matrix_rows_MPI, Matrix_With_One_Row_Test) {
    boost::mpi::communicator world;
    std::vector<int> global_matrix;
    const int m = 1;
    const int n = 20;

    if (world.rank() == 0) {
        global_matrix = {
            0, -1, 3, 42, -5,
            6, 2, 2, 3, 4,
            -1, -3, 1, 3, 5,
            100, 50, 20, 15, -100};
    }

    std::vector<size_t> global_sum = find_min_values_in_matrix_rows_parallel(global_matrix, m, n);

    if (world.rank() == 0) {
        ASSERT_EQ(19, global_sum[0]);
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
