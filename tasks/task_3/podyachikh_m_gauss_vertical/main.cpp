// Copyright 2023 Podyachikh Mikhail
#include <gtest/gtest.h>
#include <mpi.h>
#include "./gauss.h"

TEST(Gauss, Test_Random_Small) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int n = 5, m = 10;
    Image image(n, vi(m));
    Image seq, par;
    FillRandom(&image);
    seq = Gauss(image);
    par = GaussParallel(image);
    if (rank == 0) {
        for (int i = 0; i < n; i++)
            for (int j = 0; j < m; j++) {
                ASSERT_EQ(seq[i][j], par[i][j]);
            }
    }
}

TEST(Gauss, Test_Equal) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int n = 50, m = 100;
    Image image(n, vi(m, 100));

    auto seq = Gauss(image);
    auto par = GaussParallel(image);
    if (rank == 0) {
        for (int i = 0; i < n; i++)
            for (int j = 0; j < m; j++) {
                ASSERT_EQ(seq[i][j], par[i][j]);
                ASSERT_EQ(image[i][j], par[i][j]);
            }
    }
}

TEST(Gauss, Test_Zeros) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int n = 100, m = 100;
    Image image(n, vi(m, 0));

    auto seq = Gauss(image);
    auto par = GaussParallel(image);
    if (rank == 0) {
        for (int i = 0; i < n; i++)
            for (int j = 0; j < m; j++) {
                ASSERT_EQ(seq[i][j], par[i][j]);
                ASSERT_EQ(par[i][j], 0);
            }
    }
}

TEST(Gauss, Test_One) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    uint8_t val = 31;
    Image image = {{val}};

    auto seq = Gauss(image);
    auto par = GaussParallel(image);
    if (rank == 0) {
        ASSERT_EQ(seq[0][0], par[0][0]);
        ASSERT_EQ(par[0][0], val);
    }
}

TEST(Gauss, Test_Random) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int n = 128, m = 512;
    Image image(n, vi(m));

    FillRandom(&image);
    auto seq = Gauss(image);
    auto par = GaussParallel(image);
    if (rank == 0) {
        for (int i = 0; i < n; i++)
            for (int j = 0; j < m; j++) {
                ASSERT_EQ(seq[i][j], par[i][j]);
            }
    }
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::TestEventListeners &listeners =
        ::testing::UnitTest::GetInstance()->listeners();

    if (MPI_Init(&argc, &argv) != MPI_SUCCESS) {
        MPI_Abort(MPI_COMM_WORLD, -1);
    }

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank != 0) {
        delete listeners.Release(listeners.default_result_printer());
    }

    int result_code = RUN_ALL_TESTS();

    MPI_Finalize();
    return result_code;
}
