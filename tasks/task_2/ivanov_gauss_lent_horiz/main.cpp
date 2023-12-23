// Copyright 2023 Ivanov Nikita
#include <gtest/gtest.h>
#include <vector>
#include <boost/mpi/communicator.hpp>
#include <boost/mpi/collectives.hpp>
#include <boost/mpi/environment.hpp>
#include "task_2/ivanov_gauss_lent_horiz/gauss_lent_horiz.h"

TEST(Parallel_Gauss, test_1) {
    boost::mpi::communicator world;
    int size = 8;
    std::vector<double> matrix(size * (size + 1));
    if (world.rank() == 0)
        matrix = create_random_matrix(size);

    boost::mpi::broadcast(world, matrix.data(), size * (size + 1), 0);
    std::vector<double> x;
    x = Gauss(matrix, size);

    if (world.rank() == 0)
        ASSERT_EQ(check_result(matrix, x, size), true);
}

TEST(Parallel_Gauss, test_2) {
    boost::mpi::communicator world;
    int size = 8;
    std::vector<double> matrix(size * (size + 1));
    if (world.rank() == 0)
        matrix = create_random_matrix(size);

    boost::mpi::broadcast(world, matrix.data(), size * (size + 1), 0);
    std::vector<double> x;
    x = Gauss(matrix, size);

    if (world.rank() == 0)
        ASSERT_EQ(check_result(matrix, x, size), true);
}

TEST(Parallel_Gauss, test_3) {
    boost::mpi::communicator world;
    int size = 8;
    std::vector<double> matrix(size * (size + 1));
    if (world.rank() == 0)
        matrix = create_random_matrix(size);

    boost::mpi::broadcast(world, matrix.data(), size * (size + 1), 0);
    std::vector<double> x;
    x = Gauss(matrix, size);

    if (world.rank() == 0)
        ASSERT_EQ(check_result(matrix, x, size), true);
}

TEST(Parallel_Gauss, test_4) {
    boost::mpi::communicator world;
    int size = 10;
    std::vector<double> matrix(size * (size + 1));
    if (world.rank() == 0)
        matrix = create_random_matrix(size);

    boost::mpi::broadcast(world, matrix.data(), size * (size + 1), 0);
    std::vector<double> x;
    x = Gauss(matrix, size);

    if (world.rank() == 0)
        ASSERT_EQ(check_result(matrix, x, size), true);
}

TEST(Parallel_Gauss, test_5) {
    boost::mpi::communicator world;
    int size = 10;
    std::vector<double> matrix(size * (size + 1));
    if (world.rank() == 0)
        matrix = create_random_matrix(size);

    boost::mpi::broadcast(world, matrix.data(), size * (size + 1), 0);
    std::vector<double> x;
    x = Gauss(matrix, size);

    if (world.rank() == 0)
        ASSERT_EQ(check_result(matrix, x, size), true);
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
