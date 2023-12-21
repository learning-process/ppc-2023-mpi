
// Copyright 2023 Pushkarev Ilya
#include <gtest/gtest.h>
#include <boost/mpi/communicator.hpp>
#include <boost/mpi/environment.hpp>
#include <mpi.h>
#include <iostream>
#include "./most_different_neighbor_elements.h"

TEST(Parallel_Vector_Difference, Test_only_1) {
    int rankProc = 0;
    int numProc = 0;

    MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);
    MPI_Comm_size(MPI_COMM_WORLD, &numProc);

    std::vector<int> vec{1, 1, 1, 1};

    int result = maxDifferencePar(vec);

    if (rankProc == 0) {
        int resSeq = maxDifferenceSeq(vec);
        ASSERT_EQ(result, resSeq);
    }
}

TEST(Parallel_Vector_Difference, Test_from_1_to_10) {
    int rankProc = 0;
    int numProc = 0;

    MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);
    MPI_Comm_size(MPI_COMM_WORLD, &numProc);

    std::vector<int> vec{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    int result = maxDifferencePar(vec);

    if (rankProc == 0) {
        int resSeq = maxDifferenceSeq(vec);
        ASSERT_EQ(result, resSeq);
    }
}

TEST(Parallel_Vector_Difference, Test_random_N_10) {
    int rankProc = 0;
    int numProc = 0;

    MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);
    MPI_Comm_size(MPI_COMM_WORLD, &numProc);

    std::vector<int> vec = create_random_vector(10, 0, 1000);

    int result = maxDifferencePar(vec);

    if (rankProc == 0) {
        int resSeq = maxDifferenceSeq(vec);
        ASSERT_EQ(result, resSeq);
    }
}

TEST(Parallel_Vector_Difference, Test_random_N_100) {
    int rankProc = 0;
    int numProc = 0;

    MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);
    MPI_Comm_size(MPI_COMM_WORLD, &numProc);

    std::vector<int> vec = create_random_vector(100, 0, 1000);

    int result = maxDifferencePar(vec);

    if (rankProc == 0) {
        int resSeq = maxDifferenceSeq(vec);
        ASSERT_EQ(result, resSeq);
    }
}

TEST(Parallel_Vector_Difference, Test_random_N_100_negative) {
    int rankProc = 0;
    int numProc = 0;

    MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);
    MPI_Comm_size(MPI_COMM_WORLD, &numProc);

    std::vector<int> vec = create_random_vector(100, -1000, 0);

    int result = maxDifferencePar(vec);

    if (rankProc == 0) {
        int resSeq = maxDifferenceSeq(vec);
        ASSERT_EQ(result, resSeq);
    }
}

TEST(Parallel_Vector_Difference, Test_random_N_1000) {
    int rankProc = 0;
    int numProc = 0;

    MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);
    MPI_Comm_size(MPI_COMM_WORLD, &numProc);

    std::vector<int> vec = create_random_vector(1000, 0, 1000);

    int result = maxDifferencePar(vec);

    if (rankProc == 0) {
        int resSeq = maxDifferenceSeq(vec);
        ASSERT_EQ(result, resSeq);
    }
}

TEST(Parallel_Vector_Difference, Test_random_N_1000_negative) {
    int rankProc = 0;
    int numProc = 0;

    MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);
    MPI_Comm_size(MPI_COMM_WORLD, &numProc);

    std::vector<int> vec = create_random_vector(1000, -1000, 0);

    int result = maxDifferencePar(vec);
    if (rankProc == 0) {
        int resSeq = maxDifferenceSeq(vec);
        ASSERT_EQ(result, resSeq);
    }
}

int main(int argc, char **argv) {
    int result_code = 0;

    ::testing::InitGoogleTest(&argc, argv);
    ::testing::TestEventListeners &listeners =
        ::testing::UnitTest::GetInstance()->listeners();

    if (MPI_Init(&argc, &argv) != MPI_SUCCESS)
        MPI_Abort(MPI_COMM_WORLD, -1);
    result_code = RUN_ALL_TESTS();
    MPI_Finalize();

    return result_code;
}

