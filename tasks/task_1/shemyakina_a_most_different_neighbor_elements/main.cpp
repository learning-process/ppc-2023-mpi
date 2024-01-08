// Copyright 2023 Shemyakina Alesya
#include <gtest/gtest.h>
#include <mpi.h>
#include <numeric>
#include "task_1/shemyakina_a_most_different_neighbor_elements/most_different_neighbor_elements.h"

TEST(Most_Different_Neighbor_Elements, TestDifferents) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<int> elements(95);
    std::iota(elements.begin(), elements.end(), 1);

    double res = ParallelGet(elements);

    if (rank == 0) {
        double resSequent = SequentialGet(elements);
        ASSERT_EQ(res, resSequent);
        ASSERT_EQ(res, 1);
    }
}

TEST(Most_Different_Neighbor_Elements, TestEqualsElements) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> elements(11, 123);
    double res = ParallelGet(elements);

    if (rank == 0) {
        double resSequent = SequentialGet(elements);
        ASSERT_EQ(res, resSequent);
        ASSERT_EQ(res, 0);
    }
}

TEST(Most_Different_Neighbor_Elements, Test1Num) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<int> elements{11};

    double res = ParallelGet(elements);

    if (rank == 0) {
        double resSequent = SequentialGet(elements);
        ASSERT_EQ(res, resSequent);
        ASSERT_EQ(res, 0);
    }
}

TEST(Most_Different_Neighbor_Elements, Test2Num) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<int> elements({15, 198});

    double res = ParallelGet(elements);

    if (rank == 0) {
        double resSequent = SequentialGet(elements);
        ASSERT_EQ(res, resSequent);
    }
}

TEST(Most_Different_Neighbor_Elements, TestRand) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<int> elements = VecRandNum(1e6, -1e9, 1e9);

    double res = ParallelGet(elements);

    if (rank == 0) {
        double resSequent = SequentialGet(elements);
        ASSERT_EQ(res, resSequent);
    }
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::TestEventListeners &listeners =
        ::testing::UnitTest::GetInstance()->listeners();

    if (MPI_Init(&argc, &argv) != MPI_SUCCESS)
        MPI_Abort(MPI_COMM_WORLD, -1);

    int res = RUN_ALL_TESTS();
    MPI_Finalize();

    return res;
}
