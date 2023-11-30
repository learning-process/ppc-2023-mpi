// Copyright 2023 Chuvashov Andrey

#include <gtest/gtest.h>
#include "../tasks/task_2/chuvashov_a_ring_topology/ring_topology.h"

TEST(RingTopologyTests, Test_inc) {
    
}

TEST(RingTopologyTests, Test_dec) {
    
}

TEST(RingTopologyTests, Test_two_processors) {
    
}

TEST(RingTopologyTests, Test_middle_send_inc) {
    
}

TEST(RingTopologyTests, Test_middle_send_dec) {
    
}

int main(int argc, char** argv) {
    int result = 0;
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::TestEventListeners& listeners =
        ::testing::UnitTest::GetInstance()->listeners();
    MPI_Init(&argc, &argv);
    result = RUN_ALL_TESTS();
    MPI_Finalize();
    return result;
}
