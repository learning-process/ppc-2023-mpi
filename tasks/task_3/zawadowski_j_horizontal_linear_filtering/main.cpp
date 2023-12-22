// Copyright 2023 Zawadowski Jan
#include <gtest/gtest.h>
#include "./horizontal_linear_filtering.h"

TEST(Horizontal_Gauss_MPI, Test_4_rows_4_cols) {
    int rank,
        rows = 4,
        cols = 4;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<unsigned char> image = getRandomImage(rows, cols),
                               resP,
                               resC;

    resP = filterImageP(image, rows, cols);

    if (rank == 0) {
        resC = filterImageC(image, rows, cols);
        ASSERT_EQ(resP, resC);
    }
}

TEST(Horizontal_Gauss_MPI, Test_6_rows_3_cols) {
    int rank,
        rows = 6,
        cols = 3;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<unsigned char> image = getRandomImage(rows, cols),
                               resP,
                               resC;

    resP = filterImageP(image, rows, cols);

    if (rank == 0) {
        resC = filterImageC(image, rows, cols);
        ASSERT_EQ(resP, resC);
    }
}

TEST(Horizontal_Gauss_MPI, Test_4_rows_7_cols) {
    int rank,
        rows = 4,
        cols = 7;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<unsigned char> image = getRandomImage(rows, cols),
                               resP,
                               resC;

    resP = filterImageP(image, rows, cols);

    if (rank == 0) {
        resC = filterImageC(image, rows, cols);
        ASSERT_EQ(resP, resC);
    }
}

TEST(Horizontal_Gauss_MPI, Test_6_rows_1_cols) {
    int rank,
        rows = 6,
        cols = 1;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<unsigned char> image = getRandomImage(rows, cols),
                               resP,
                               resC;

    resP = filterImageP(image, rows, cols);

    if (rank == 0) {
        resC = filterImageC(image, rows, cols);
        ASSERT_EQ(resP, resC);
    }
}

TEST(Horizontal_Gauss_MPI, Test_1_rows_6_cols) {
    int rank,
        rows = 1,
        cols = 6;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<unsigned char> image = getRandomImage(rows, cols),
                               resP,
                               resC;

    resP = filterImageP(image, rows, cols);

    if (rank == 0) {
        resC = filterImageC(image, rows, cols);
        ASSERT_EQ(resP, resC);
    }
}

int main(int argc, char** argv) {
    int resultCode = 0;
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::TestEventListeners& listeners =
        ::testing::UnitTest::GetInstance()->listeners();

    if (MPI_Init(&argc, &argv) != MPI_SUCCESS)
        MPI_Abort(MPI_COMM_WORLD, -1);
    resultCode = RUN_ALL_TESTS();
    MPI_Finalize();

    return resultCode;
}
