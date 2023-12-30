// Copyright 2023 Afanasyev Aleksey
#include <gtest/gtest.h>
#include "./horizontal_gauss_filter.h"

TEST(MPI_TESTS, image_3x3_filtering) {
    const int cols = 3;
    const int rows = 3;

    int rank;
    std::vector<unsigned char> image = {255, 200, 150, 90, 30, 90, 150, 200, 255};
    std::vector<unsigned char> par;
    std::vector<unsigned char> seq;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    par = applyParGaussianFilter(image, rows, cols);

    if (rank == 0) {
        seq = applySeqGaussianFilter(image, rows, cols);
        ASSERT_EQ(compareVectors(par, seq), true);
    }
}

TEST(MPI_TESTS, mono_image_99x99_filtering) {
    const int rows = 99;
    const int cols = 99;
    const int color = 99;

    int rank;
    std::vector<unsigned char> image = generateMonoImage(cols, rows, color);
    std::vector<unsigned char> par;
    std::vector<unsigned char> seq;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    par = applyParGaussianFilter(image, cols, rows);

    if (rank == 0) {
        seq = applySeqGaussianFilter(image, cols, rows);
        ASSERT_EQ(compareVectors(par, seq), true);
    }
}

TEST(MPI_TESTS, rand_image_99x99_filtering) {
    const int rows = 99;
    const int cols = 99;

    int rank;
    std::vector<unsigned char> image = generateRandomImage(cols, rows);
    std::vector<unsigned char> par;
    std::vector<unsigned char> seq;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    par = applyParGaussianFilter(image, cols, rows);

    if (rank == 0) {
        seq = applySeqGaussianFilter(image, cols, rows);
        ASSERT_EQ(compareVectors(par, seq), true);
    }
}

TEST(MPI_TESTS, rand_image_720x360_filtering) {
    const int rows = 720;
    const int cols = 360;

    int rank;
    std::vector<unsigned char> image = generateRandomImage(cols, rows);
    std::vector<unsigned char> par;
    std::vector<unsigned char> seq;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    par = applyParGaussianFilter(image, cols, rows);

    if (rank == 0) {
        seq = applySeqGaussianFilter(image, cols, rows);
        ASSERT_EQ(compareVectors(par, seq), true);
    }
}

TEST(MPI_TESTS, empty_image_filtering) {
    const int rows = 0;
    const int cols = 0;
    const int color = 99;

    int rank;
    std::vector<unsigned char> image = generateMonoImage(cols, rows, color);
    std::vector<unsigned char> par;
    std::vector<unsigned char> seq;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    par = applyParGaussianFilter(image, rows, cols);

    if (rank == 0) {
        seq = applySeqGaussianFilter(image, rows, cols);
        ASSERT_EQ(compareVectors(par, seq), true);
    }
}

int main(int argc, char** argv) {
    int result_code = 0;

    ::testing::InitGoogleTest(&argc, argv);
    ::testing::TestEventListeners& listeners = ::testing::UnitTest::GetInstance()->listeners();

    if (MPI_Init(&argc, &argv) != MPI_SUCCESS)
        MPI_Abort(MPI_COMM_WORLD, -1);
    result_code = RUN_ALL_TESTS();
    MPI_Finalize();

    return result_code;
}
