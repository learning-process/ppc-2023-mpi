// Copyright 2023 Pushkarev Ilya

#include <gtest/gtest.h>
#include <boost/mpi/environment.hpp>
#include <boost/mpi/communicator.hpp>
#include "./shortest_path_moore.h"


TEST(MOORE, Test_Graph_1) {
    boost::mpi::communicator world;
    std::vector<int> matr {
        0, 3, 1, 2,
        3, 0, 0, 9,
        6, 3, 0, 10,
        0, 5, 4, 0
    };
    bool has_negative_cycle = false;
    auto dist = mooore(world.rank(), world.size(), 4, matr, &has_negative_cycle);
    if (world.rank() == 0) {
        ASSERT_EQ(has_negative_cycle, false);
        ASSERT_EQ(dist[0], 0);
        ASSERT_EQ(dist[1], 3);
        ASSERT_EQ(dist[2], 1);
        ASSERT_EQ(dist[3], 2);
    }
}

TEST(MOORE, Test_Graph_2) {
    boost::mpi::communicator world;
    std::vector<int> matr {
        0, 10, 10, 10,
        10, 0, 10, -1,
        10, -1, 0, 10,
        10, 10, 2, 0
    };
    bool has_negative_cycle = false;
    auto dist = mooore(world.rank(), world.size(), 4, matr, &has_negative_cycle);
    if (world.rank() == 0) {
        ASSERT_EQ(has_negative_cycle, false);
        ASSERT_EQ(dist[0], 0);
        ASSERT_EQ(dist[1], 9);
        ASSERT_EQ(dist[2], 10);
        ASSERT_EQ(dist[3], 8);
    }
}

TEST(MOORE, Test_Graph_3) {
    boost::mpi::communicator world;
    std::vector<int> matr {
        0, 10, 10, 10,
        10, 0, 10, -1,
        10, -1, 0, 10,
        10, 10, -1, 0
    };
    bool has_negative_cycle = false;
    auto dist = mooore(world.rank(), world.size(), 4, matr, &has_negative_cycle);
    if (world.rank() == 0) {
        ASSERT_EQ(has_negative_cycle, true);
    }
}

TEST(MOORE, Test_Graph_4) {
    boost::mpi::communicator world;
    std::vector<int> matr {
        0, 3, 1, 2,
        3, 0, 0, 9,
        6, 3, 0, 10,
        0, 5, 4, 0
    };
    bool has_negative_cycle = false;
    auto dist = mooore(world.rank(), world.size(), 4, matr, &has_negative_cycle);
    if (world.rank() == 0) {
        ASSERT_EQ(has_negative_cycle, false);
        ASSERT_EQ(dist[0], 0);
        ASSERT_EQ(dist[1], 3);
        ASSERT_EQ(dist[2], 1);
        ASSERT_EQ(dist[3], 2);
    }
}

TEST(MOORE, Test_Graph_5) {
    boost::mpi::communicator world;
    std::vector<int> matr {
        0, 6, -30, 30,
        8, 0, -14, 39,
        32, 24, 0, 35,
        21, 1, -19, 0
    };
    bool has_negative_cycle = false;
    auto dist = mooore(world.rank(), world.size(), 4, matr, &has_negative_cycle);
    if (world.rank() == 0) {
        ASSERT_EQ(has_negative_cycle, false);
        ASSERT_EQ(dist[0], 0);
        ASSERT_EQ(dist[1], -6);
        ASSERT_EQ(dist[2], -30);
        ASSERT_EQ(dist[3], 5);
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

