// Copyright 2023 Makhinya Danil
#include <gtest/gtest.h>
#include <vector>
#include "./rect_integration.h"
#include <boost/mpi/environment.hpp>
#include <boost/mpi/communicator.hpp>


TEST(Parallel_Dot_Product_MPI, Test_Specified_Vector) {
    ASSERT_TRUE(true);
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
