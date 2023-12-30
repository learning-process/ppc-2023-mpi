// Copyright 2023 Kiselev Igor
#include <gtest/gtest.h>
#include <mpi.h>
#include <iostream>
#include "./vector_sum.h"

TEST(Parallel_Vector_Mean, Test_only_1) {
    int rankProc = 0;
    int numProc = 0;

    MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);
    MPI_Comm_size(MPI_COMM_WORLD, &numProc);

    std::vector<int> vec{1, 1, 1, 1};

    int result = sumPar(vec);

    if (rankProc == 0) {
        int resSeq = sumSeq(vec);
        ASSERT_EQ(result, resSeq);
    }
}

TEST(Parallel_Vector_Mean, Test_only_0) {
    int rankProc = 0;
    int numProc = 0;

    MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);
    MPI_Comm_size(MPI_COMM_WORLD, &numProc);

    std::vector<int> vec{0, 0, 0, 0};

    int result = sumPar(vec);

    if (rankProc == 0) {
        int resSeq = sumSeq(vec);
        ASSERT_EQ(result, resSeq);
    }
}

TEST(Parallel_Vector_Mean, Test_not_elems) {
    int rankProc = 0;
    int numProc = 0;

    MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);
    MPI_Comm_size(MPI_COMM_WORLD, &numProc);

    std::vector<int> vec{};
    int result = sumPar(vec);

    if (rankProc == 0) {
        int resSeq = sumSeq(vec);
        ASSERT_EQ(result, resSeq);
    }
}

TEST(Parallel_Vector_Mean, Test_random_elems) {
    int rankProc = 0;
    int numProc = 0;

    MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);
    MPI_Comm_size(MPI_COMM_WORLD, &numProc);

    std::vector<int> vec(15);
    for (auto &elem : vec)
        elem = generateRandomNumbers(-1000, 1000);

    int result = sumPar(vec);

    if (rankProc == 0) {
        int resSeq = sumSeq(vec);
        ASSERT_EQ(result, resSeq);
    }
}

TEST(Parallel_Vector_Mean, Test_half_elems) {
    int rankProc = 0;
    int numProc = 0;

    MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);
    MPI_Comm_size(MPI_COMM_WORLD, &numProc);

    std::vector<int> vec{2, 2, 2, 2, 2, 5, 5, 5, 5, 5, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1};

    int result = sumPar(vec);

    if (rankProc == 0) {
        int resSeq = sumSeq(vec);
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
