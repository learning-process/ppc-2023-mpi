// Copyright 2023 Makhinya Danil
#include <gtest/gtest.h>
#include <vector>
#include <cmath>
#include "./gradient_method.h"
#include <boost/mpi/environment.hpp>
#include <boost/mpi/communicator.hpp>

TEST(Parallel_Readers_Writes_MPI, Test_no_throw1) {
    ASSERT_NO_THROW();
}

TEST(Parallel_Readers_Writes_MPI, Test_no_throw2) {
    ASSERT_NO_THROW();
}

TEST(Parallel_Readers_Writes_MPI, Test_no_throw3) {
    ASSERT_NO_THROW();
}

TEST(Parallel_Readers_Writes_MPI, Test_no_throw4) {
    ASSERT_NO_THROW();
}

TEST(Parallel_Readers_Writes_MPI, Test_no_throw5) {
    ASSERT_NO_THROW();
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
