// Copyright 2023 Kulikov Artem
#include <gtest/gtest.h>
#include <vector>
#include <boost/mpi/communicator.hpp>
#include <boost/mpi/environment.hpp>
#include <boost/serialization/vector.hpp>

#include "./most_different_elements_in_vec.h"


TEST(Parallel_Most_Different_Elements_In_Vector_MPI, Random_Test_1) {
    boost::mpi::communicator world;

    std::vector<int> random_vector;
    const size_t n = 100;

    if (world.rank() == 0) {
        random_vector = getRandomVector(n);
    }

    std::vector<std::pair<size_t, size_t>> parallel_most_diff = getParallelMostDifferentElements(random_vector, n);

    if (world.rank() == 0) {
        std::vector<std::pair<size_t, size_t>> sequential_most_diff = getSequentialMostDifferentElements(random_vector);

        ASSERT_EQ(sequential_most_diff, parallel_most_diff);
    }
}

TEST(Parallel_Most_Different_Elements_In_Vector_MPI, Random_Test_2) {
    boost::mpi::communicator world;

    std::vector<int> random_vector;
    const size_t n = 100;

    if (world.rank() == 0) {
        random_vector = getRandomVector(n);
    }

    std::vector<std::pair<size_t, size_t>> parallel_most_diff = getParallelMostDifferentElements(random_vector, n);

    if (world.rank() == 0) {
        std::vector<std::pair<size_t, size_t>> sequential_most_diff = getSequentialMostDifferentElements(random_vector);

        ASSERT_EQ(sequential_most_diff, parallel_most_diff);
    }
}

TEST(Parallel_Most_Different_Elements_In_Vector_MPI, Test_With_Size_0) {
    boost::mpi::communicator world;
    std::vector<int> vec;

    std::vector<std::pair<size_t, size_t>> most_dif = getParallelMostDifferentElements(vec, 0);

    if (world.rank() == 0) {
        ASSERT_EQ(most_dif.size(), 0);
    }
}

TEST(Parallel_Most_Different_Elements_In_Vector_MPI, Test_With_Size_1) {
    boost::mpi::communicator world;
    std::vector<int> vec(1);

    if (world.rank() == 0) {
        vec = std::vector<int>(1);
    }

    std::vector<std::pair<size_t, size_t>> most_dif = getParallelMostDifferentElements(vec, 1);

    if (world.rank() == 0) {
        ASSERT_EQ(most_dif.size(), 0);
    }
}



TEST(Parallel_Most_Different_Elements_In_Vector_MPI, Test_With_Size_2) {
    boost::mpi::communicator world;
    std::vector<int> vec;

    if (world.rank() == 0) {
        vec = std::vector<int>{1, 2};
    }

    std::vector<std::pair<size_t, size_t>> most_dif = getParallelMostDifferentElements(vec, 2);

    if (world.rank() == 0) {
        std::vector<std::pair<size_t, size_t>> expected = getSequentialMostDifferentElements(vec);
        ASSERT_EQ(most_dif, expected);
    }
}

TEST(Sequential_Most_Different_Elements_In_Vector_MPI, manual_test) {
    std::vector<int> vec = {4, 3, 10, 23, 100, 1, 100, 1};

    std::vector<std::pair<size_t, size_t>> most_dif = getSequentialMostDifferentElements(vec);
    std::vector<std::pair<size_t, size_t>> expected = {{4, 5}, {5, 6}, {6, 7}};

    ASSERT_EQ(most_dif, expected);
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

