// Copyright 2023 Makhinya Danil
#include <gtest/gtest.h>
#include <vector>
#include <cmath>
#include "./readers_writers.h"
#include <boost/mpi/environment.hpp>
#include <boost/mpi/communicator.hpp>


TEST(Parallel_Readers_Writes_MPI, Test_no_throw) {
    boost::mpi::communicator world;

    if (world.rank() == 0) {
        ASSERT_NO_THROW(start_task());
    }
}

TEST(Parallel_Readers_Writes_MPI, Test_random_order_big_size) {
    boost::mpi::communicator world;

    if (world.rank() == 0) {
        ASSERT_NO_THROW(run_random_order_read_and_write(0, 10000));
    }
}

TEST(Parallel_Readers_Writes_MPI, Test_random_order_small_size) {
    boost::mpi::communicator world;

    if (world.rank() == 0) {
        ASSERT_NO_THROW(run_random_order_read_and_write(0, 5));
    }
}

TEST(Parallel_Readers_Writes_MPI, Test_random_order_big_size2) {
    boost::mpi::communicator world;

    if (world.rank() == 0) {
        ASSERT_NO_THROW(run_random_order_read_and_write(0, 5000, 1));
    }
}

TEST(Parallel_Readers_Writes_MPI, Test_random_order_big_size3) {
    boost::mpi::communicator world;

    if (world.rank() == 0) {
        ASSERT_NO_THROW(run_random_order_read_and_write(0, 1, 5000));
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
