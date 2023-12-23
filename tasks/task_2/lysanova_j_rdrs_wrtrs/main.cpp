// Copyright 2023 Lysanova Julia
#include <gtest/gtest.h>
#include "./rdrs_wrtrs.h"
#include <boost/mpi/environment.hpp>
#include <boost/mpi/communicator.hpp>



TEST(Readers_Writers_Problem, Not_Failing) {
    boost::mpi::communicator world;

    ASSERT_NO_THROW(rdrs_wrtrs_boost());
}



TEST(Readers_Writers_Problem, All_Visited) {
    boost::mpi::communicator world;

    auto res = rdrs_wrtrs_boost();

    if (world.rank() == 0) {
        auto q = res.first;
        ASSERT_EQ(q.size(), world.size() - 1);

        auto vec = res.second;
        ASSERT_EQ(vec.size(), world.size());
    }
}



TEST(Readers_Writers_Problem, Correct_Reading) {
    boost::mpi::communicator world;

    auto res = rdrs_wrtrs_boost();

    if (world.rank() == 0) {
        int shared_data = 0;
        auto q = res.first;
        auto vec = res.second;
        while (!q.empty()) {
            auto e = q.front(); q.pop();
            auto actor = e.first;
            auto role = e.second;

            if (role == 1) {
                shared_data = vec[actor];
            } else {
                ASSERT_EQ(shared_data, vec[actor]);
            }
        }
    }
}



TEST(Readers_Writers_Problem, Correct_Reading2) {
    boost::mpi::communicator world;

    auto res = rdrs_wrtrs_boost();

    if (world.rank() == 0) {
        int shared_data = 0;
        auto q = res.first;
        auto vec = res.second;

        while (!q.empty()) {
            auto e = q.front(); q.pop();
            auto actor = e.first;
            auto role = e.second;

            if (role == 1) {
                shared_data = vec[actor];
            } else {
                ASSERT_EQ(shared_data, vec[actor]);
            }
        }
    }
}



TEST(Readers_Writers_Problem, Correct_Writing) {
    boost::mpi::communicator world;

    auto res = rdrs_wrtrs_boost();

    if (world.rank() == 0) {
        int shared_data = 0;
        auto q = res.first;
        auto vec = res.second;

        while (!q.empty()) {
            auto e = q.front(); q.pop();
            auto actor = e.first;
            auto role = e.second;

            if (role == 1) {
                shared_data = vec[actor];
            }
        }

        ASSERT_EQ(shared_data, vec[0]);
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
