// Copyright 2023 Moiseev Nikita
#include <gtest/gtest.h>
#include "task_1/moiseev_n_num_of_alternations_signs/num_of_alternations_signs.h"

TEST(MPI_TESTS, Test_int) {
    int rank = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> arr{ 1, -2, 3, -4, 5 , -6 };
    int result_par = par_num_of_alternations(arr);
    if (rank == 0) {
        int result_seq = seq_num_of_alternations(arr);
        ASSERT_EQ(result_par, result_seq);
    }
}


TEST(MPI_TESTS, Test_plus) {
    int rank = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> arr{ 1, 2, 3, 4, 5 };
    int result_par = par_num_of_alternations(arr);
    if (rank == 0) {
        int result_seq = seq_num_of_alternations(arr);
        ASSERT_EQ(result_par, result_seq);
    }
}

TEST(MPI_TESTS, Test_minus) {
    int rank = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> arr{ -1, -2, -3, -4, -5 };
    int result_par = par_num_of_alternations(arr);
    if (rank == 0) {
        int result_seq = seq_num_of_alternations(arr);
        ASSERT_EQ(result_par, result_seq);
    }
}

TEST(MPI_TESTS, Test_null) {
    int rank = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> arr{ };
    int result_par = par_num_of_alternations(arr);
    if (rank == 0) {
        int result_seq = seq_num_of_alternations(arr);
        ASSERT_EQ(result_par, result_seq);
    }
}

TEST(MPI_TESTS, Test_random) {
    int rank = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    const size_t sizeArr = 100;
    std::vector<int> arr;
    arr = create_random_array(sizeArr);
    int result_par = par_num_of_alternations(arr);
    if (rank == 0) {
        int result_seq = seq_num_of_alternations(arr);
        ASSERT_EQ(result_par, result_seq);
    }
}


int main(int argc, char** argv) {
    int result_code = 0;
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::TestEventListeners& listeners =
        ::testing::UnitTest::GetInstance()->listeners();
    MPI_Init(&argc, &argv);
    result_code = RUN_ALL_TESTS();
    MPI_Finalize();

    return result_code;
}
