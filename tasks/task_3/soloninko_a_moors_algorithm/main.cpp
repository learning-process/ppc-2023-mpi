// Copyright 2023 Soloninko Andrey
#include <gtest/gtest.h>
#include <mpi.h>

#include <iostream>

#include "./moore.h"

TEST(Moore_alg, Test_1) {
    int rankProc = 0;
    int numProc = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);
    MPI_Comm_size(MPI_COMM_WORLD, &numProc);

    int size = 4;
    bool has_negative_cycle = false;
    std::vector<int> matrix(size * size);
    std::vector<int> par_dist(size);
    std::vector<int> ser_dist(size);

    matrix = rand_matr(size);

    bellman_moore_parallel(size, &matrix, &par_dist, has_negative_cycle);

    if (rankProc == 0) {
        has_negative_cycle = false;
        bellman_moore(size, &matrix, &ser_dist, has_negative_cycle);
        /*
        for (int i = 0; i < size; i++) {
            std::cout << par_dist[i] << "\n";
        }
        for (int i = 0; i < size; i++) {
            std::cout << ser_dist[i] << "\n";
        }*/
        for (int i = 0; i < size; i++) {
            ASSERT_EQ(par_dist[i], ser_dist[i]);
        }
    }
}

TEST(Moore_alg, Test_2) {
    int rankProc = 0;
    int numProc = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);
    MPI_Comm_size(MPI_COMM_WORLD, &numProc);

    int size = 10;
    bool has_negative_cycle = false;
    std::vector<int> matrix(size * size);
    std::vector<int> par_dist(size);
    std::vector<int> ser_dist(size);

    matrix = rand_matr(size);

    bellman_moore_parallel(size, &matrix, &par_dist, has_negative_cycle);

    if (rankProc == 0) {
        has_negative_cycle = false;
        bellman_moore(size, &matrix, &ser_dist, has_negative_cycle);
        for (int i = 0; i < size; i++) {
            ASSERT_EQ(par_dist[i], ser_dist[i]);
        }
    }
}

TEST(Moore_alg, Test_3) {
    int rankProc = 0;
    int numProc = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);
    MPI_Comm_size(MPI_COMM_WORLD, &numProc);

    int size = 10;
    bool has_negative_cycle = false;
    std::vector<int> matrix(size * size);
    std::vector<int> par_dist(size);
    std::vector<int> ser_dist(size);

    matrix = rand_matr(size);

    bellman_moore_parallel(size, &matrix, &par_dist, has_negative_cycle);

    if (rankProc == 0) {
        has_negative_cycle = false;
        bellman_moore(size, &matrix, &ser_dist, has_negative_cycle);
        for (int i = 0; i < size; i++) {
            ASSERT_EQ(par_dist[i], ser_dist[i]);
        }
    }
}

TEST(Moore_alg, Test_4) {
    int rankProc = 0;
    int numProc = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);
    MPI_Comm_size(MPI_COMM_WORLD, &numProc);

    int size = 500;
    bool has_negative_cycle = false;
    std::vector<int> matrix(size * size);
    std::vector<int> par_dist(size);
    std::vector<int> ser_dist(size);

    matrix = rand_matr(size);

    bellman_moore_parallel(size, &matrix, &par_dist, has_negative_cycle);

    if (rankProc == 0) {
        has_negative_cycle = false;
        bellman_moore(size, &matrix, &ser_dist, has_negative_cycle);
        for (int i = 0; i < size; i++) {
            ASSERT_EQ(par_dist[i], ser_dist[i]);
        }
    }
}

TEST(Moore_alg, Test_5) {
    int rankProc = 0;
    int numProc = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rankProc);
    MPI_Comm_size(MPI_COMM_WORLD, &numProc);

    int size = 500;
    bool has_negative_cycle = false;
    std::vector<int> matrix(size * size);
    std::vector<int> par_dist(size);
    std::vector<int> ser_dist(size);

    matrix = rand_matr(size);

    bellman_moore_parallel(size, &matrix, &par_dist, has_negative_cycle);

    if (rankProc == 0) {
        has_negative_cycle = false;
        bellman_moore(size, &matrix, &ser_dist, has_negative_cycle);
        for (int i = 0; i < size; i++) {
            ASSERT_EQ(par_dist[i], ser_dist[i]);
        }
    }
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
