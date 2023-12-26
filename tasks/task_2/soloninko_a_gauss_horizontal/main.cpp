// Copyright 2023 Soloninko Andrey
#include <gtest/gtest.h>
#include <mpi.h>

#include <iostream>

#include "./Gauss.h"

TEST(Gauss_Horizontal, Test_1) {
    int rankProc = 0;
    int numProc = 0;

    MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);
    MPI_Comm_size(MPI_COMM_WORLD, &numProc);
    int size = 4;
    std::vector<double> matrix(size * (size + 1));
    std::vector<double> res(size);

    rand_matr(&matrix, size);
    gauss_algo(&matrix, &res, size);

    if (rankProc == 0) ASSERT_EQ(check_res(&matrix, &res, size), true);
}

TEST(Gauss_Horizontal, Test_2) {
    int rankProc = 0;
    int numProc = 0;

    MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);
    MPI_Comm_size(MPI_COMM_WORLD, &numProc);
    int size = 5;
    std::vector<double> matrix(size * (size + 1));
    std::vector<double> res(size);

    rand_matr(&matrix, size);
    gauss_algo(&matrix, &res, size);

    if (rankProc == 0) ASSERT_EQ(check_res(&matrix, &res, size), true);
}

TEST(Gauss_Horizontal, Test_3) {
    int rankProc = 0;
    int numProc = 0;

    MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);
    MPI_Comm_size(MPI_COMM_WORLD, &numProc);
    int size = 6;
    std::vector<double> matrix(size * (size + 1));
    std::vector<double> res(size);

    rand_matr(&matrix, size);
    gauss_algo(&matrix, &res, size);

    if (rankProc == 0) ASSERT_EQ(check_res(&matrix, &res, size), true);
}

TEST(Gauss_Horizontal, Test_4) {
    int rankProc = 0;
    int numProc = 0;

    MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);
    MPI_Comm_size(MPI_COMM_WORLD, &numProc);
    int size = 8;
    std::vector<double> matrix(size * (size + 1));
    std::vector<double> res(size);

    rand_matr(&matrix, size);
    gauss_algo(&matrix, &res, size);

    if (rankProc == 0) ASSERT_EQ(check_res(&matrix, &res, size), true);
}

TEST(Gauss_Horizontal, Test_5) {
    int rankProc = 0;
    int numProc = 0;

    MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);
    MPI_Comm_size(MPI_COMM_WORLD, &numProc);
    int size = 8;
    std::vector<double> matrix(size * (size + 1));
    std::vector<double> res(size);

    rand_matr(&matrix, size);
    gauss_algo(&matrix, &res, size);

    if (rankProc == 0) ASSERT_EQ(check_res(&matrix, &res, size), true);
}

int main(int argc, char **argv) {
    int res_c = 0;

    ::testing::InitGoogleTest(&argc, argv);
    ::testing::TestEventListeners &listeners =
        ::testing::UnitTest::GetInstance()->listeners();

    if (MPI_Init(&argc, &argv) != MPI_SUCCESS) MPI_Abort(MPI_COMM_WORLD, -1);
    res_c = RUN_ALL_TESTS();
    MPI_Finalize();

    return res_c;
}
