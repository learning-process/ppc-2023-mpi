// Copyright 2023 Kriseev Mikhail
#include <gtest/gtest.h>
#include <random>
#include <vector>
#include <boost/mpi/environment.hpp>
#include <boost/mpi/communicator.hpp>
#include <boost/mpi/timer.hpp>
#include "./min_of_vector_elements.h"

TEST(Min_Of_Vector_Elements_MPI, one_element) {
    boost::mpi::communicator world;
    std::vector<int> data = { 1 };

    int parallelResult = MinElementsInVectorParallel(data);

    if (world.rank() == 0) {
        int sequentialResult = MinElementsInVectorSequential(data);
        ASSERT_EQ(parallelResult, sequentialResult);
        ASSERT_EQ(parallelResult, 1);
    }
}

TEST(Min_Of_Vector_Elements_MPI, four_elements) {
    boost::mpi::communicator world;
    std::vector<int> data = { 5, 3, 2, 4 };

    int parallelResult = MinElementsInVectorParallel(data);

    if (world.rank() == 0) {
        int sequentialResult = MinElementsInVectorSequential(data);
        ASSERT_EQ(parallelResult, sequentialResult);
        ASSERT_EQ(parallelResult, 2);
    }
}

TEST(Min_Of_Vector_Elements_MPI, random_100_elements) {
    std::uniform_int_distribution distribution(0, 100);
    std::random_device random;

    boost::mpi::communicator world;
    std::vector<int> data(100);
    for (int i = 0; i < 100; ++i) {
        data.emplace_back(distribution(random));
    }
    int parallelResult = MinElementsInVectorParallel(data);

    if (world.rank() == 0) {
        int sequentialResult = MinElementsInVectorSequential(data);
        ASSERT_EQ(parallelResult, sequentialResult);
    }
}

TEST(Min_Of_Vector_Elements_MPI, random_matrix_divisible_by_world_size) {
    std::uniform_int_distribution distribution(0, 100);
    std::random_device random;

    boost::mpi::communicator world;
    std::vector<int> data(100);
    int vector_size = world.size() * world.size();
    for (int i = 0; i < vector_size; ++i) {
        data.emplace_back(distribution(random));
    }
    int parallelResult = MinElementsInVectorParallel(data);

    if (world.rank() == 0) {
        int sequentialResult = MinElementsInVectorSequential(data);
        ASSERT_EQ(parallelResult, sequentialResult);
    }
}

TEST(Min_Of_Vector_Elements_MPI, random_matrix_non_divisible_by_world_size) {
    std::uniform_int_distribution distribution(0, 100);
    std::random_device random;

    boost::mpi::communicator world;
    std::vector<int> data(100);
    int vector_size = (world.size() + 1) * (world.size() + 1);
    for (int i = 0; i < vector_size; ++i) {
        data.emplace_back(distribution(random));
    }
    int parallelResult = MinElementsInVectorParallel(data);

    if (world.rank() == 0) {
        int sequentialResult = MinElementsInVectorSequential(data);
        ASSERT_EQ(parallelResult, sequentialResult);
    }
}

TEST(Min_Of_Vector_Elements_MPI, last_element_is_min) {
    std::uniform_int_distribution distribution(2, 100);
    std::random_device random;

    boost::mpi::communicator world;
    std::vector<int> data(100);
    int vector_size = (world.size() + 1) * (world.size() + 1);

    for (int i = 0; i < vector_size; ++i) {
        data.emplace_back(distribution(random));
    }
    data.back() = -1;
    int parallelResult = MinElementsInVectorParallel(data);

    if (world.rank() == 0) {
        ASSERT_EQ(parallelResult, -1);
        int sequentialResult = MinElementsInVectorSequential(data);
        ASSERT_EQ(parallelResult, sequentialResult);
    }
}

int main(int argc, char** argv) {
    boost::mpi::environment env(argc, argv);
    boost::mpi::communicator world;
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::TestEventListeners& listeners =
            ::testing::UnitTest::GetInstance()->listeners();
    if (world.rank() != 0) {
        delete listeners.Release(listeners.default_result_printer());
    }
    return RUN_ALL_TESTS();
}
