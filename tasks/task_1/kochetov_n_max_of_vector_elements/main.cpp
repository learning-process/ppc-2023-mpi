// Copyright 2023 Kochetov Nikolay
#include <gtest/gtest.h>
#include <vector>
#include "task_1/kochetov_n_max_of_vector_elements/max_of_vector_elements.h"
#include <boost/mpi/environment.hpp>
#include <boost/mpi/communicator.hpp>

TEST(MaxOfVectorElements, Test_random_numbers) {
    boost::mpi::communicator comm;
    std::vector<int> global_vec;
    const size_t global_vec_size = 100;

    if (comm.rank() == 0) {
        global_vec = GetRandomVector(global_vec_size);
    }

    int result = GetMaxElement(global_vec, global_vec_size);

    if (comm.rank() == 0) {
        int max = *std::max_element(global_vec.begin(), global_vec.end());
        ASSERT_EQ(max, result);
    }
}

TEST(MaxOfVectorElements, Test_only_positive_numbers) {
    boost::mpi::communicator comm;
    std::vector<int> global_vec;
    const size_t global_vec_size = 59;

    if (comm.rank() == 0) {
        global_vec = GetRandomVector(global_vec_size, 1);
    }

    int result = GetMaxElement(global_vec, global_vec_size);

    if (comm.rank() == 0) {
        int max = *std::max_element(global_vec.begin(), global_vec.end());
        ASSERT_EQ(max, result);
    }
}

TEST(MaxOfVectorElements, Test_only_negative_numbers) {
    boost::mpi::communicator comm;
    std::vector<int> global_vec;
    const size_t global_vec_size = 102;

    if (comm.rank() == 0) {
        global_vec = GetRandomVector(global_vec_size, -1000, -1);
    }

    int result = GetMaxElement(global_vec, global_vec_size);

    if (comm.rank() == 0) {
        int max = *std::max_element(global_vec.begin(), global_vec.end());
        ASSERT_EQ(max, result);
    }
}

TEST(MaxOfVectorElements, Test_equal_number) {
    boost::mpi::communicator comm;
    std::vector<int> global_vec;
    const size_t global_vec_size = 5;

    if (comm.rank() == 0) {
        global_vec = {43, 43, 43, 43, 43};
    }

    int result = GetMaxElement(global_vec, global_vec_size);

    if (comm.rank() == 0) {
        int max = *std::max_element(global_vec.begin(), global_vec.end());
        ASSERT_EQ(max, result);
    }
}

TEST(MaxOfVectorElements, Test_one_number) {
    boost::mpi::communicator comm;
    std::vector<int> global_vec;
    const size_t global_vec_size = 1;
    if (comm.rank() == 0) {
        global_vec = {10};
    }

    int result = GetMaxElement(global_vec, global_vec_size);

    if (comm.rank() == 0) {
        ASSERT_EQ(10, result);
    }
}



int main(int argc, char** argv) {
    boost::mpi::environment env(argc, argv);
    boost::mpi::communicator comm;
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::TestEventListeners& listeners =
        ::testing::UnitTest::GetInstance()->listeners();
    if (comm.rank() != 0) {
        delete listeners.Release(listeners.default_result_printer());
    }
    return RUN_ALL_TESTS();
    return 0;
}
