// Copyright 2023 Kalinin Alexandr
#include <gtest/gtest.h>
#include "task_3/kalinin_a_gradient/gradient.h"

TEST(Parallel_Operations_MPI, Serial_method_gradient_1) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int n = 2;
    Vector V(n);
    Vector M(n * n);
    Vector res2(n);
    V[0] = 17;
    V[1] = 16;
    M[0] = 1;
    M[1] = 2;
    M[2] = 2;
    M[3] = 1;
    res2[0] = 5;
    res2[1] = 6;
    if (rank == 0) {
        Vector res1 = Serial_method_gradient(M, V, n);
        for (int i = 0; i < res1.size(); i++)
            EXPECT_LE(abs(res2[i] - res1[i]), 1);
    }
}
TEST(Parallel_Operations_MPI, Serial_method_gradient_2) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int n = 2;
    Vector V(n);
    Vector M(n * n);
    Vector res2(n);
    V[0] = 3;
    V[1] = 7;
    M[0] = 3;
    M[1] = -1;
    M[2] = -1;
    M[3] = 3;
    res2[0] = 2;
    res2[1] = 3;
    if (rank == 0) {
        Vector res1 = Serial_method_gradient(M, V, n);
        for (int i = 0; i < res1.size(); i++)
            EXPECT_LE(abs(res2[i] - res1[i]), 1);
    }
}
TEST(Parallel_Operations_MPI, Serial_method_gradient_3) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int n = 3;
    Vector V(n);
    Vector M(n * n);
    Vector res2(n);
    V[0] = 6;
    V[1] = 6;
    V[2] = 10;
    M[0] = 1;
    M[1] = 2;
    M[2] = 3;
    M[3] = 2;
    M[4] = 1;
    M[5] = 0;
    M[6] = 3;
    M[7] = 0;
    M[8] = 1;
    res2[0] = 3;
    res2[1] = 0;
    res2[2] = 1;
    if (rank == 0) {
        Vector res1 = Serial_method_gradient(M, V, n);
        for (int i = 0; i < res1.size(); i++)
            EXPECT_LE(abs(res2[i] - res1[i]), 1);
    }
}
TEST(Parallel_Operations_MPI, Paralle_method_gradient_1) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int n = 2;
    Vector V(n);
    Vector M(n * n);
    Vector res2(n);
    V[0] = 17;
    V[1] = 16;
    M[0] = 1;
    M[1] = 2;
    M[2] = 2;
    M[3] = 1;
    res2[0] = 5;
    res2[1] = 6;
    Vector res1 = Paralle_method_gradient(M, V, n);
    if (rank == 0) {
        for (int i = 0; i < res1.size(); i++)
            EXPECT_LE(abs(res2[i] - res1[i]), 1);
    }
}
TEST(Parallel_Operations_MPI, Paralle_method_gradient_2) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int n = 2;
    Vector V(n);
    Vector M(n * n);
    Vector res2(n);
    V[0] = 3;
    V[1] = 7;
    M[0] = 3;
    M[1] = -1;
    M[2] = -1;
    M[3] = 3;
    res2[0] = 2;
    res2[1] = 3;
    Vector res1 = Paralle_method_gradient(M, V, n);
    if (rank == 0) {
        for (int i = 0; i < res1.size(); i++)
            EXPECT_LE(abs(res2[i] - res1[i]), 1);
    }
}

int main(int argc, char** argv) {
    int result_code = 0;
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::TestEventListeners& listeners =
        ::testing::UnitTest::GetInstance()->listeners();

    if (MPI_Init(&argc, &argv) != MPI_SUCCESS)
        MPI_Abort(MPI_COMM_WORLD, -1);
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank != 0) {
        delete listeners.Release(listeners.default_result_printer());
    }
    result_code = RUN_ALL_TESTS();
    MPI_Finalize();

    return result_code;
}
