// Copyright 2023 Vasyliev Ivan
#include <gtest/gtest.h>
#include <string>
#include <boost/mpi/environment.hpp>
#include <boost/mpi/communicator.hpp>
#include "./topology_rule.h"

TEST(TopologyRuleTest, Test1) {
    boost::mpi::communicator world;
    if (world.size() == 1) { return; }
    int data = 0, tag = 0;
    if (world.rank() == 0) {
        data = 42;
        RULE_SEND(world, data, world.size() - 1, tag);
    } else if (world.rank() == world.size() - 1) {
        RULE_RECV(world, data, 0, tag);
        ASSERT_EQ(data, 42);
    } else {
        RULE_HELP(world, 0, world.size() - 1, tag);
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
