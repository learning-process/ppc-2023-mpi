// Copyright 2023 Makhinya Danil
#include <gtest/gtest.h>
#include <vector>
#include <cmath>
#include "./readers_writers.h"
#include <boost/mpi/environment.hpp>
#include <boost/mpi/communicator.hpp>

TEST(Parallel_Readers_Writes_MPI, Test_no_throw1) {
    boost::mpi::communicator world;
    int rank = world.rank();
    int sizeWorld = world.size();

    run_problem_readers_writers(2, 2);

    if (rank == 0) {
        EXPECT_NO_THROW();
    }
}

TEST(Parallel_Readers_Writes_MPI, Test_no_throw2) {
    boost::mpi::communicator world;
    int rank = world.rank();
    int sizeWorld = world.size();

    run_problem_readers_writers(1, 2);

    if (rank == 0) {
        EXPECT_NO_THROW();
    }
}

TEST(Parallel_Readers_Writes_MPI, Test_no_throw3) {
    boost::mpi::communicator world;
    int rank = world.rank();
    int sizeWorld = world.size();

    run_problem_readers_writers(2, 1);

    if (rank == 0) {
        EXPECT_NO_THROW();
    }
}

TEST(Parallel_Readers_Writes_MPI, Test_no_throw4) {
    boost::mpi::communicator world;
    int rank = world.rank();
    int sizeWorld = world.size();

    if (rank == 0) {
        EXPECT_NO_THROW();
    }
}

TEST(Parallel_Readers_Writes_MPI, Test_no_throw5) {
    boost::mpi::communicator world;
    int rank = world.rank();
    int sizeWorld = world.size();

    run_problem_readers_writers(0, 0);

    if (rank == 0) {
        EXPECT_NO_THROW();
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
