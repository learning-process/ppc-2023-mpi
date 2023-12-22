// Copyright 2023 Pozdnyakov Vasya
#include <gtest/gtest.h>
#include <vector>
#include "./max_of_vector_elements.h"
#include <boost/mpi/environment.hpp>
#include <boost/mpi/communicator.hpp>

TEST(Max_Of_Vector_Elements, Test_random_numbers) {
    boost::mpi::communicator world;
    std::vector<int> global_vec;
    const size_t global_vec_size = 100;
    if (world.rank() == 0) {
        global_vec = get_random_vector(global_vec_size);
    }

    int global_res = get_max_element(global_vec, global_vec_size);

    if (world.rank() == 0) {
        int max = *std::max_element(global_vec.begin(), global_vec.end());
        ASSERT_EQ(max, global_res);
    }
}

TEST(Max_Of_Vector_Elements, Test_only_positive_numbers) {
    boost::mpi::communicator world;
    std::vector<int> global_vec;
    const size_t global_vec_size = 59;
    if (world.rank() == 0) {
        global_vec = get_random_vector(global_vec_size, 1);
    }

    int global_res = get_max_element(global_vec, global_vec_size);

    if (world.rank() == 0) {
        int max = *std::max_element(global_vec.begin(), global_vec.end());
        ASSERT_EQ(max, global_res);
    }
}

TEST(Max_Of_Vector_Elements, Test_only_negative_numbers) {
    boost::mpi::communicator world;
    std::vector<int> global_vec;
    const size_t global_vec_size = 102;
    if (world.rank() == 0) {
        global_vec = get_random_vector(global_vec_size, -1000, -1);
    }

    int global_res = get_max_element(global_vec, global_vec_size);

    if (world.rank() == 0) {
        int max = *std::max_element(global_vec.begin(), global_vec.end());
        ASSERT_EQ(max, global_res);
    }
}

TEST(Max_Of_Vector_Elements, Test_equal_number) {
    boost::mpi::communicator world;
    std::vector<int> global_vec;
    const size_t global_vec_size = 5;
    if (world.rank() == 0) {
        global_vec = {43, 43, 43, 43, 43};
    }

    int global_res = get_max_element(global_vec, global_vec_size);

    if (world.rank() == 0) {
        int max = *std::max_element(global_vec.begin(), global_vec.end());
        ASSERT_EQ(max, global_res);
    }
}

TEST(Max_Of_Vector_Elements, Test_one_number) {
    boost::mpi::communicator world;
    std::vector<int> global_vec;
    const size_t global_vec_size = 1;
    if (world.rank() == 0) {
        global_vec = {10};
    }

    int global_res = get_max_element(global_vec, global_vec_size);

    if (world.rank() == 0) {
        ASSERT_EQ(10, global_res);
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
    return 0;
}
