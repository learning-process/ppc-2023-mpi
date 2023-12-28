// Copyright 2023 Kachalov Mikhail
#include <gtest/gtest.h>
#include <vector>
#include "./orderly_violations.h"
#include <boost/mpi/environment.hpp>
#include <boost/mpi/communicator.hpp>

TEST(OrderlyViolationsTests, TestEmptyVector) {
    boost::mpi::communicator world;
    std::vector<int> vec;

    int parallelViolations = parallel_num_of_orderly_violations(vec);

    if (world.rank() == 0) {
        int sequentialViolations = num_of_orderly_violations(vec);
        ASSERT_EQ(sequentialViolations, parallelViolations);
    }
}

TEST(OrderlyViolationsTests, TestIncreasingOrder) {
    boost::mpi::communicator world;
    std::vector<int> vec = {1, 2, 3, 4, 5};

    int parallelViolations = parallel_num_of_orderly_violations(vec);

    if (world.rank() == 0) {
        int sequentialViolations = num_of_orderly_violations(vec);
        ASSERT_EQ(sequentialViolations, parallelViolations);
    }
}

TEST(OrderlyViolationsTests, TestDecreasingOrder) {
    boost::mpi::communicator world;
    std::vector<int> vec = {5, 4, 3, 2, 1};

    int parallelViolations = parallel_num_of_orderly_violations(vec);

    if (world.rank() == 0) {
        int sequentialViolations = num_of_orderly_violations(vec);
        ASSERT_EQ(sequentialViolations, parallelViolations);
    }
}

TEST(OrderlyViolationsTests, TestEqualElements) {
    boost::mpi::communicator world;
    std::vector<int> vec = {1, 1, 1, 1, 1};

    int parallelViolations = parallel_num_of_orderly_violations(vec);

    if (world.rank() == 0) {
        int sequentialViolations = num_of_orderly_violations(vec);
        ASSERT_EQ(sequentialViolations, parallelViolations);
    }
}

TEST(OrderlyViolationsTests, TestRandomVector) {
    boost::mpi::communicator world;
    std::vector<int> vec;

    if (world.rank() == 0) {
        vec = get_random_vector(100);
    }

    int parallelViolations = parallel_num_of_orderly_violations(vec);

    if (world.rank() == 0) {
        int sequentialViolations = num_of_orderly_violations(vec);
        ASSERT_EQ(sequentialViolations, parallelViolations);
    }
}

TEST(OrderlyViolationsTests, TestLargeVector) {
    boost::mpi::communicator world;
    std::vector<int> vec;

    if (world.rank() == 0) {
        vec = get_random_vector(1000);
    }

    int parallelViolations = parallel_num_of_orderly_violations(vec);

    if (world.rank() == 0) {
        int sequentialViolations = num_of_orderly_violations(vec);
        ASSERT_EQ(sequentialViolations, parallelViolations);
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