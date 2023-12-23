// Copyright 2023 Vasyliev Ivan
#include <gtest/gtest.h>
#include <string>
#include <boost/mpi/environment.hpp>
#include <boost/mpi/communicator.hpp>
#include "./topology_rule.h"

TEST(TopologyRuleTest, Test1) {
    boost::mpi::communicator world;
    if (world.size() == 1) { return; }
    int src = 0, dst = world.size() - 1;
    int data = 0, tag = 0;
    if (world.rank() == src) {
        data = 42;
        RULE_SEND(world, data, dst, tag);
    } else if (world.rank() == dst) {
        RULE_RECV(world, &data, src, tag);
        ASSERT_EQ(data, 42);
    } else {
        RULE_HELP(world, src,  dst, tag);
    }
}


TEST(TopologyRuleTest, Test2) {
    boost::mpi::communicator world;
    if (world.size() == 1) { return; }
    int src = world.size() - 1, dst = 0;
    int data = 0, tag = 0;
    if (world.rank() == src) {
        data = 42;
        RULE_SEND(world, data, dst, tag);
    } else if (world.rank() == dst) {
        RULE_RECV(world, &data, src, tag);
        ASSERT_EQ(data, 42);
    } else {
        RULE_HELP(world, src,  dst, tag);
    }
}


TEST(TopologyRuleTest, Test3) {
    boost::mpi::communicator world;
    int src = 0, dst = world.size() / 2;
    if (src == dst) { return; }
    int data = 0, tag = 0;
    if (world.rank() == src) {
        data = 42;
        RULE_SEND(world, data, dst, tag);
    } else if (world.rank() == dst) {
        RULE_RECV(world, &data, src, tag);
        ASSERT_EQ(data, 42);
    } else {
        RULE_HELP(world, src, dst, tag);
    }
}


TEST(TopologyRuleTest, Test4) {
    boost::mpi::communicator world;
    int src =  world.size() / 2, dst = 0;
    if (src == dst) { return; }
    int data = 0, tag = 0;
    if (world.rank() == src) {
        data = 42;
        RULE_SEND(world, data, dst, tag);
    } else if (world.rank() == dst) {
        RULE_RECV(world, &data, src, tag);
        ASSERT_EQ(data, 42);
    } else {
        RULE_HELP(world, src, dst, tag);
    }
}


TEST(TopologyRuleTest, Test5) {
    boost::mpi::communicator world;
    if (world.size() == 1) { return; }
    int src = 0, dst = world.size() - 1;
    int data = 0, tag = 8080;
    if (world.rank() == src) {
        data = 42;
        RULE_SEND(world, data, dst, tag);
    } else if (world.rank() == dst) {
        RULE_RECV(world, &data, src, tag);
        ASSERT_EQ(data, 42);
    } else {
        RULE_HELP(world, src, dst, tag);
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
