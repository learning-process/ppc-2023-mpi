// Copyright 2023 Lysanova Julia
#include <gtest/gtest.h>
#include <iostream>
#include "./max_elem.h"
#include <boost/mpi/environment.hpp>
#include <boost/mpi/communicator.hpp>



TEST(Max_Vec_Elem, Test_Random) {
    boost::mpi::communicator world;
    std::vector<int> vec;
    int vec_size = 42;

    if (world.rank() == 0) {
        vec = RandomVector(vec_size);
    }

    int parallel_max = MaxElem(vec, vec_size);

    if (world.rank() == 0) {
        int seq_max = *std::max_element(vec.begin(), vec.end());
        ASSERT_EQ(parallel_max, seq_max);
    }
}



TEST(Max_Vec_Elem, Test_Negative_Only) {
    boost::mpi::communicator world;
    std::vector<int> vec;
    int vec_size = 17;

    if (world.rank() == 0) {
        vec = RandomVector(vec_size, -100, -1);
    }
    int parallel_max = MaxElem(vec, vec_size);

    if (world.rank() == 0) {
        int seq_max = *std::max_element(vec.begin(), vec.end());
        ASSERT_EQ(parallel_max, seq_max);
    }
}



TEST(Max_Vec_Elem, Test_Non_Negative_Only) {
    boost::mpi::communicator world;
    std::vector<int> vec;
    int vec_size = 13;

    if (world.rank() == 0) {
        vec = RandomVector(vec_size, 0, 100);
    }
    int parallel_max = MaxElem(vec, vec_size);

    if (world.rank() == 0) {
        int seq_max = *std::max_element(vec.begin(), vec.end());
        ASSERT_EQ(parallel_max, seq_max);
    }
}



TEST(Max_Vec_Elem, Test_Equal_Only) {
    boost::mpi::communicator world;
    std::vector<int> vec;
    int vec_size = 10;

    if (world.rank() == 0) {
        vec.resize(vec_size, 42);
    }
    int parallel_max = MaxElem(vec, vec_size);

    if (world.rank() == 0) {
        ASSERT_EQ(parallel_max, 42);
    }
}



TEST(Max_Vec_Elem, Test_Some_Equal) {
    boost::mpi::communicator world;
    std::vector<int> vec;
    int vec_size = 7;

    if (world.rank() == 0) {
        vec = {3, 2, 1, 4, 2, 4, -1};
    }
    int parallel_max = MaxElem(vec, vec_size);

    if (world.rank() == 0) {
        ASSERT_EQ(parallel_max, 4);
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
