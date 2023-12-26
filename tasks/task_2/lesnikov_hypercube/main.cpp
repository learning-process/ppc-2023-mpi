// Copyright 2023 Kulikov Artem
#include <gtest/gtest.h>
#include <vector>
#include <boost/mpi/communicator.hpp>
#include <boost/mpi/environment.hpp>
#include <boost/serialization/vector.hpp>

#include "./hypercube.h"


TEST(Hypercube, Send_To_Neighbour_Straight) {
    boost::mpi::communicator world;

    std::vector<int> expected = {1, 2, 3};

    if (is2Degree(world.size())) {
        std::vector<int> data;

        if (world.rank() == 0) {
            data = {1, 2, 3};
        }

        sendRecvData(0, 1, 0, &data);

        if (world.rank() == 1) {
            EXPECT_EQ(data, expected);
        }
    }
}

TEST(Hypercube, Send_To_Neighbour_Back) {
    boost::mpi::communicator world;

    std::vector<int> expected = {1, 2, 3};

    if (is2Degree(world.size())) {
        std::vector<int> data;

        if (world.rank() == 1) {
            data = {1, 2, 3};
        }

        sendRecvData(1, 0, 0, &data);

        if (world.rank() == 0) {
            EXPECT_EQ(data, expected);
        }
    }
}

TEST(Hypercube, Send_To_Farthest_Straight) {
    boost::mpi::communicator world;

    std::vector<int> expected = {1, 2, 3};

    if (is2Degree(world.size())) {
        std::vector<int> data;

        if (world.rank() == 0) {
            data = {1, 2, 3};
        }

        sendRecvData(0, world.size() - 1, 0, &data);

        if (world.rank() == world.size() - 1) {
            EXPECT_EQ(data, expected);
        }
    }
}

TEST(Hypercube, Send_To_Farthest_Back) {
    boost::mpi::communicator world;

    std::vector<int> expected = {1, 2, 3};

    if (is2Degree(world.size())) {
        std::vector<int> data;

        if (world.rank() == world.size() - 1) {
            data = {1, 2, 3};
        }

        sendRecvData(world.size() - 1, 0, 0, &data);

        if (world.rank() == 0) {
            EXPECT_EQ(data, expected);
        }
    }
}

TEST(Hypercube, Throw_If_Incorrect_World_Size) {
    boost::mpi::communicator world;

    int a = 1;

    if (world.size() != 1 && !is2Degree(world.size())) {
        ASSERT_ANY_THROW(sendRecvData(0, 1, 0, &a));
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

