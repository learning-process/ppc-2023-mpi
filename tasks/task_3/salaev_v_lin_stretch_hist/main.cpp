// Copyright 2023 Salaev Vladislav
#include <gtest/gtest.h>
#include "task_3/salaev_v_lin_stretch_hist/lin_stretch_hist.h"

TEST(Lin_Stretch_Hist, Test_MidValueImage) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    const size_t width = 10, height = 10;
    const uint8_t newMin = 50, newMax = 200;
    std::vector<uint8_t> img(width * height, 128);
    std::vector copy(img);
    parallelIncreaseContrast(
        &img, width, height, newMin, newMax, MPI_COMM_WORLD);
    if (rank == 0) {
        increaseContrast(&copy, 128, 128, newMin, newMax);
        ASSERT_EQ(img, copy);
    }
}

TEST(Lin_Stretch_Hist, Test_RepeatValues) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    const size_t width = 4, height = 4;
    const uint8_t newMin = 30, newMax = 220;
    std::vector<uint8_t> img{100, 150, 100, 150, 100, 150, 100, 150,
        100, 150, 100, 150, 100, 150, 100, 150};
    std::vector copy(img);
    parallelIncreaseContrast(
        &img, width, height, newMin, newMax, MPI_COMM_WORLD);
    if (rank == 0) {
        uint8_t oldMin = *std::min_element(copy.begin(), copy.end());
        uint8_t oldMax = *std::max_element(copy.begin(), copy.end());
        increaseContrast(&copy, oldMin, oldMax, newMin, newMax);
        ASSERT_EQ(img, copy);
    }
}

TEST(Lin_Stretch_Hist, Test_LinearDistribution) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    const size_t width = 5, height = 1;
    const uint8_t newMin = 0, newMax = 255;
    std::vector<uint8_t> img{0, 64, 128, 192, 255};
    std::vector copy(img);
    parallelIncreaseContrast(
        &img, width, height, newMin, newMax, MPI_COMM_WORLD);
    if (rank == 0) {
        ASSERT_EQ(img, copy);
    }
}

TEST(Lin_Stretch_Hist, Test_AllMaxValues) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    const size_t width = 10, height = 10;
    const uint8_t newMin = 10, newMax = 245;
    std::vector<uint8_t> img(width * height, 255);
    std::vector copy(img);
    parallelIncreaseContrast(
        &img, width, height, newMin, newMax, MPI_COMM_WORLD);
    if (rank == 0) {
        ASSERT_EQ(img, copy);
    }
}

TEST(Lin_Stretch_Hist, Test_ZeroSizeImage) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    const size_t width = 0, height = 0;
    const uint8_t newMin = 0, newMax = 255;
    std::vector<uint8_t> img;
    std::vector<uint8_t> copy;
    parallelIncreaseContrast(
        &img, width, height, newMin, newMax, MPI_COMM_WORLD);
    if (rank == 0) {
        ASSERT_TRUE(img.empty());
        ASSERT_EQ(img, copy);
    }
}


int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    int result = 0, rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    ::testing::InitGoogleTest(&argc, argv);
    auto& listeners = ::testing::UnitTest::GetInstance()->listeners();
    if (rank != 0)
        delete listeners.Release(listeners.default_result_printer());
    result = RUN_ALL_TESTS();
    MPI_Finalize();
    return result;
}
