// Copyright 2023 Travin Maksim
#include <gtest/gtest.h>
#include <mpi.h>
#include <iostream>
#include "./max_of_vector_elements.h"

TEST(Parallel_Vector_Sum, Test_only_1) {
    int rankProc = 0;
    int numProc = 0;

    MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);
    MPI_Comm_size(MPI_COMM_WORLD, &numProc);

    std::vector<int> vec{ 1, 1, 1, 1 };

    int result = get_max_element(vec);

    if (rankProc == 0) {
        int resSeq = MaxSeq(vec);
        ASSERT_EQ(result, resSeq);
    }
}

TEST(Parallel_Vector_Sum, Test_random_elems) {
    int rankProc = 0;
    int numProc = 0;

    MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);
    MPI_Comm_size(MPI_COMM_WORLD, &numProc);

    std::vector<int> vec = get_random_vector(15);
    int result = get_max_element(vec);

    if (rankProc == 0) {
        int resSeq = MaxSeq(vec);
        ASSERT_EQ(result, resSeq);
    }
}

TEST(Parallel_Vector_Sum, Test_only_lower_0) {
    int rankProc = 0;
    int numProc = 0;

    MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);
    MPI_Comm_size(MPI_COMM_WORLD, &numProc);

    std::vector<int> vec = get_random_vector(10, -1000, -1);

    int result = get_max_element(vec);

    if (rankProc == 0) {
        int resSeq = MaxSeq(vec);
        ASSERT_EQ(result, resSeq);
    }
}

TEST(Parallel_Vector_Sum, Test_not_elems) {
    int rankProc = 0;
    int numProc = 0;

    MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);
    MPI_Comm_size(MPI_COMM_WORLD, &numProc);

    std::vector<int> vec{};
    int result = get_max_element(vec);

    if (rankProc == 0) {
        int resSeq = MaxSeq(vec);
        ASSERT_EQ(result, resSeq);
    }
}



TEST(Parallel_Vector_Sum, Test_couple_max_elems) {
    int rankProc = 0;
    int numProc = 0;

    MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);
    MPI_Comm_size(MPI_COMM_WORLD, &numProc);

    std::vector<int> vec{ 1, 2, 3, 2, 6, 7, 1, 0, 0, 3, 9, 2, 4, 1, 7, 5, 9 };

    int result = get_max_element(vec);

    if (rankProc == 0) {
        int resSeq = MaxSeq(vec);
        ASSERT_EQ(result, resSeq);
    }
}

int main(int argc, char** argv) {
    int result_code = 0;

    ::testing::InitGoogleTest(&argc, argv);
    ::testing::TestEventListeners& listeners =
        ::testing::UnitTest::GetInstance()->listeners();

    if (MPI_Init(&argc, &argv) != MPI_SUCCESS)
        MPI_Abort(MPI_COMM_WORLD, -1);
    result_code = RUN_ALL_TESTS();
    MPI_Finalize();

    return result_code;
}
