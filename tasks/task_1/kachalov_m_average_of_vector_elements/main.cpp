// Copyright 2023 Kachalov Mikhail
#include <gtest/gtest.h>
#include <vector>
#include "./average_of_vector_elements.h"
#include <boost/mpi/environment.hpp>
#include <boost/mpi/communicator.hpp>

TEST(AverageOfVectorElementsTests, TestEmptyVector) {
    boost::mpi::communicator world;
    std::vector<int> vec;

    double parallelAverage = parallel_average_of_vector_elements(vec);

    if (world.rank() == 0) {
        double sequentialAverage = seq_average_of_vector_elements(vec);
        ASSERT_EQ(sequentialAverage, parallelAverage);
    }
}

TEST(AverageOfVectorElementsTests, TestPositiveNumbers) {
    boost::mpi::communicator world;
    std::vector<int> vec = {1, 2, 3, 4, 5};

    double parallelAverage = parallel_average_of_vector_elements(vec);

    if (world.rank() == 0) {
        double sequentialAverage = seq_average_of_vector_elements(vec);
        ASSERT_EQ(sequentialAverage, parallelAverage);
    }
}

TEST(AverageOfVectorElementsTests, TestDecreasingOrder) {
    boost::mpi::communicator world;
    std::vector<int> vec = {5, 4, 3, 2, 1};

    double parallelAverage = parallel_average_of_vector_elements(vec);

    if (world.rank() == 0) {
        double sequentialAverage = seq_average_of_vector_elements(vec);
        ASSERT_EQ(sequentialAverage, parallelAverage);
    }
}

TEST(AverageOfVectorElementsTests, TestNegativeNumbers) {
    boost::mpi::communicator world;
    std::vector<int> vec = {-1, -2, -3, -4, -5};

    double parallelAverage = parallel_average_of_vector_elements(vec);

    if (world.rank() == 0) {
        double sequentialAverage = seq_average_of_vector_elements(vec);
        ASSERT_EQ(sequentialAverage, parallelAverage);
    }
}

TEST(AverageOfVectorElementsTests, TestMixedNumbers) {
    boost::mpi::communicator world;
    std::vector<int> vec = {-1, 2, -3, 4, -5};

    double parallelAverage = parallel_average_of_vector_elements(vec);

    if (world.rank() == 0) {
        double sequentialAverage = seq_average_of_vector_elements(vec);
        ASSERT_EQ(sequentialAverage, parallelAverage);
    }
}

TEST(AverageOfVectorElementsTests, TestRandomVector) {
    boost::mpi::communicator world;
    std::vector<int> vec;

    if (world.rank() == 0) {
        vec = get_random_vector(100);
    }

    double parallelAverage = parallel_average_of_vector_elements(vec);

    if (world.rank() == 0) {
        double sequentialAverage = seq_average_of_vector_elements(vec);
        ASSERT_EQ(sequentialAverage, parallelAverage);
    }
}

TEST(AverageOfVectorElementsTests, TestLargeVector) {
    boost::mpi::communicator world;
    std::vector<int> vec;

    if (world.rank() == 0) {
        vec = get_random_vector(1000);
    }

    double parallelAverage = parallel_average_of_vector_elements(vec);

    if (world.rank() == 0) {
        double sequentialAverage = seq_average_of_vector_elements(vec);
        ASSERT_EQ(sequentialAverage, parallelAverage);
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
