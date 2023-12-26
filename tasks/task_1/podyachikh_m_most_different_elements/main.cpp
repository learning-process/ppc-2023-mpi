// Copyright 2023 Podyachikh Mikhail
#include <gtest/gtest.h>
#include <mpi.h>
#include <numeric>
#include "./most_different_elements.h"

TEST(Most_Different_Elements, Test_equals_elements) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<int> elements(11, 123);

    double result = ParallelGetMostDifferentElements(elements);

    if (rank == 0) {
        double resSeq = SequentialGetMostDifferentElements(elements);
        ASSERT_EQ(result, resSeq);
        ASSERT_EQ(result, 0);
    }
}

TEST(Most_Different_Elements, Test_differents_equal_1) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<int> elements(95);
    std::iota(elements.begin(), elements.end(), 1);

    double result = ParallelGetMostDifferentElements(elements);

    if (rank == 0) {
        double resSeq = SequentialGetMostDifferentElements(elements);
        ASSERT_EQ(result, resSeq);
        ASSERT_EQ(result, 1);
    }
}

TEST(Most_Different_Elements, Test_one_number) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<int> elements{11};

    double result = ParallelGetMostDifferentElements(elements);

    if (rank == 0) {
        double resSeq = SequentialGetMostDifferentElements(elements);
        ASSERT_EQ(result, resSeq);
        ASSERT_EQ(result, 0);
    }
}

TEST(Most_Different_Elements, Test_two_number) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<int> elements({15, 198});

    double result = ParallelGetMostDifferentElements(elements);

    if (rank == 0) {
        double resSeq = SequentialGetMostDifferentElements(elements);
        ASSERT_EQ(result, resSeq);
    }
}

TEST(Most_Different_Elements, Test_random) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<int> elements = VectorRandomNumbers(1e6, -1e9, 1e9);

    double result = ParallelGetMostDifferentElements(elements);

    if (rank == 0) {
        double resSeq = SequentialGetMostDifferentElements(elements);
        ASSERT_EQ(result, resSeq);
    }
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::TestEventListeners &listeners =
        ::testing::UnitTest::GetInstance()->listeners();

    if (MPI_Init(&argc, &argv) != MPI_SUCCESS)
        MPI_Abort(MPI_COMM_WORLD, -1);
    int result_code = RUN_ALL_TESTS();
    MPI_Finalize();

    return result_code;
}
