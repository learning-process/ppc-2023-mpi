// Copyright 2023 Kashin Stepa
#include <gtest/gtest.h>
#include "task_1/kashin_s_nearest_neighbor_elements/neighbor_elements.h"

TEST(Nearest_Neighbor_Elements, Test_only_one_elem) {
    int rankProc = 0;
    int numProc = 0;

    MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);
    MPI_Comm_size(MPI_COMM_WORLD, &numProc);

    std::vector<int> v = {1, 1, 1, 1, 1, 1, 1, 1};
    std::pair<int,int> result = par_neighbor_elements(v);

    if (rankProc == 0) {
        ASSERT_EQ(result, neighbor_elements(v));
    }
}

TEST(Nearest_Neighbor_Elements, Test_one_different) {
    int rankProc = 0;
    int numProc = 0;

    MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);
    MPI_Comm_size(MPI_COMM_WORLD, &numProc);

    std::vector<int> v = create_random_vector(1000,0,1);
    std::pair<int,int> result = par_neighbor_elements(v);

    if (rankProc == 0) {
    ASSERT_EQ(result, neighbor_elements(v));
    }
}

TEST(Nearest_Neighbor_Elements, Test_not_elems) {
    int rankProc = 0;
    int numProc = 0;

    MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);
    MPI_Comm_size(MPI_COMM_WORLD, &numProc);

    std::vector<int> v = create_random_vector(0,0,1000);
    std::pair<int,int> result = par_neighbor_elements(v);

    if (rankProc == 0) {
    ASSERT_EQ(result, neighbor_elements(v));
    }
}

TEST(Nearest_Neighbor_Elements, Test_normal_situation) {
    int rankProc = 0;
    int numProc = 0;

    MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);
    MPI_Comm_size(MPI_COMM_WORLD, &numProc);

    std::vector<int> v = create_random_vector(100,1,101);
    std::pair<int,int> result = par_neighbor_elements(v);

    if (rankProc == 0) {
    ASSERT_EQ(result, neighbor_elements(v));
    }
}

TEST(Nearest_Neighbor_Elements, Test_big_situation) {
    int rankProc = 0;
    int numProc = 0;

    MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);
    MPI_Comm_size(MPI_COMM_WORLD, &numProc);

    std::vector<int> v = create_random_vector(10000000,0,100);
    std::pair<int,int> result = par_neighbor_elements(v);

    if (rankProc == 0) {
    ASSERT_EQ(result, neighbor_elements(v));
    }
}

int main(int argc, char **argv) {
    int result_code = 0;

    ::testing::InitGoogleTest(&argc, argv);
    ::testing::TestEventListeners &listeners = ::testing::UnitTest::GetInstance()->listeners();

    if (MPI_Init(&argc, &argv) != MPI_SUCCESS)
        MPI_Abort(MPI_COMM_WORLD, -1);
    result_code = RUN_ALL_TESTS();
    MPI_Finalize();

    return result_code;
}
