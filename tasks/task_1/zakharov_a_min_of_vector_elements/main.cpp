// Copyright 2023 Zakharov Artem
#include <gtest/gtest.h>
#include <vector>
#include <boost/mpi/environment.hpp>
#include <boost/mpi/communicator.hpp>
#include "./min_vector_elements.h"

TEST(Min_Vector_Elements, Test_random) {
    boost::mpi::communicator world;
    std::vector<int> vec;
    const size_t vec_size = 101;

    if (world.rank() == 0) {
        vec = get_random_vector(vec_size);
    }

    int parallel_res = get_min_element(vec, vec_size);

    if (world.rank() == 0) {
        int seq_res = *std::min_element(vec.begin(), vec.end());
        ASSERT_EQ(seq_res, parallel_res);
    }
}

TEST(Min_Vector_Elements, Test_random_only_poistive) {
    boost::mpi::communicator world;
    std::vector<int> vec;
    const size_t vec_size = 97;

    if (world.rank() == 0) {
        vec = get_random_vector(vec_size, 1);
    }
    int parallel_res = get_min_element(vec, vec_size);

    if (world.rank() == 0) {
        int seq_res = *std::min_element(vec.begin(), vec.end());
        ASSERT_EQ(seq_res, parallel_res);
    }
}

TEST(Min_Vector_Elements, Test_random_only_negative) {
    boost::mpi::communicator world;
    std::vector<int> vec;
    const size_t vec_size = 105;

    if (world.rank() == 0) {
        vec = get_random_vector(vec_size, -1000, 0);
    }
    int parallel_res = get_min_element(vec, vec_size);

    if (world.rank() == 0) {
        int seq_res = *std::min_element(vec.begin(), vec.end());
        ASSERT_EQ(seq_res, parallel_res);
    }
}

TEST(Min_Vector_Elements, Test_identical_elements) {
    boost::mpi::communicator world;
    std::vector<int> vec;
    const size_t vec_size = 20;

    if (world.rank() == 0) {
        vec.resize(vec_size, 5);
    }
    int parallel_res = get_min_element(vec, vec_size);

    if (world.rank() == 0) {
        int seq_res = *std::min_element(vec.begin(), vec.end());
        ASSERT_EQ(seq_res, parallel_res);
    }
}

TEST(Min_Vector_Elements, Test_several_min) {
    boost::mpi::communicator world;
    std::vector<int> vec;
    const size_t vec_size = 10;

    if (world.rank() == 0) {
        vec = {1, 10, 0, 37, 45, 0, 1, 2, 0, 23};
    }
    int parallel_res = get_min_element(vec, vec_size);

    if (world.rank() == 0) {
        int seq_res = *std::min_element(vec.begin(), vec.end());
        ASSERT_EQ(seq_res, parallel_res);
    }
}

TEST(Min_Vector_Elements, Test_one_element) {
    boost::mpi::communicator world;
    std::vector<int> vec;
    const size_t vec_size = 1;

    if (world.rank() == 0) {
        vec = {3};
    }
    int parallel_res = get_min_element(vec, vec_size);

    if (world.rank() == 0) {
        ASSERT_EQ(3, parallel_res);
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
