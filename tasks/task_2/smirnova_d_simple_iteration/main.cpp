// Copyright 2023 Smirnova Daria
#include <gtest/gtest.h>
#include <vector>
#include "./simple_iteration.h"
#include <boost/mpi/environment.hpp>
#include <boost/mpi/communicator.hpp>


TEST(Simple_Iteration_MPI, Wrong_size_matrix) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<double> matrix(4);

    if (rank == 0) {
        matrix = generate_random_Matrix(4);
    }
    if (rank == 0) {
        std::vector<double> act(4);
        ASSERT_ANY_THROW(act = get_result(matrix, 2, 0.001));
    }
}

TEST(Simple_Iteration_MPI, Matrix_on_6_size) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<double> matrix(6);
    if (rank == 0) {
        ASSERT_NO_THROW(matrix = generate_random_Matrix(6));
    }
}

TEST(Simple_Iteration_MPI, Test_on_matrix_size_4) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<double> matrix(20);
    if (rank == 0) {
        matrix = { 10, -1, 2, 0, 6,
                -1, 11, -1, 3, 25,
                2, -1, 10, -1, -11,
                0, 3, -1, 8, 15 };
    }

    std::vector<double> actual(4);

    actual = get_result(matrix, 4, 0.99);

    std::vector<double> expected(4);
    if (rank == 0) {
        expected = { 1.04727, 1.7159, -0.80522, 0.88522 };

        EXPECT_TRUE(is_equal(actual, expected));
    }
}

TEST(Simple_Iteration_MPI, Matrix_size_less_then_2) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<double> matrix(1);
    if (rank == 0) {
        ASSERT_ANY_THROW(matrix = generate_random_Matrix(1));
    }
}

TEST(Simple_Iteration_MPI, Matrix_on_20_size) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<double> matrix(20);
    if (rank == 0) {
        ASSERT_NO_THROW(matrix = generate_random_Matrix(20));
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