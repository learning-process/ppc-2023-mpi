// Copyright 2023 Kruglov Alexey
#include <gtest/gtest.h>
#include "task_3/kruglov_a_linear_hyst_stretch/linear_hyst_stretch.h"

TEST(MPI_TESTS, Test_Big_Random_Matrix) {
    int rank = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    const size_t m = 1000;
    const size_t n = 1000;
    const uint8_t new_min = 100;
    const uint8_t new_max = 200;

    std::vector<uint8_t> image;
    if (rank == 0) image = create_random_image(m * n, 0, 255);
    std::vector copyImage(image);

    par_increase_contrast(&image, m * n, new_min, new_max);

    if (rank == 0) {
        uint8_t old_min = *std::min_element(copyImage.begin(), copyImage.end());
        uint8_t old_max = *std::max_element(copyImage.begin(), copyImage.end());

        seq_increase_contrast(&copyImage, old_min, old_max, new_min, new_max);

        ASSERT_EQ(image, copyImage);
    }
}

TEST(MPI_TESTS, Test_Small_Matrix) {
    int rank = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    const size_t m = 4;
    const size_t n = 4;
    const uint8_t new_min = 0;
    const uint8_t new_max = 255;

    std::vector<uint8_t> image;
    if (rank == 0) image = { 26,  111, 23, 43, 230, 67,  0, 9,
                                0, 89, 100, 0, 1, 255, 4, 205 };
    std::vector copyImage(image);

    par_increase_contrast(&image, m * n, new_min, new_max);

    if (rank == 0) {
        uint8_t old_min = *std::min_element(copyImage.begin(), copyImage.end());
        uint8_t old_max = *std::max_element(copyImage.begin(), copyImage.end());

        seq_increase_contrast(&copyImage, old_min, old_max, new_min, new_max);

        ASSERT_EQ(image, copyImage);
    }
}

TEST(MPI_TESTS, Test_LT_CommSize_Matrix) {
    int rank = 0;
    int size;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // const size_t m = 4;
    // const size_t n = 4;
    const size_t image_size = size == 1 ? 4 : size / 2;
    const uint8_t new_min = 0;
    const uint8_t new_max = 255;

    

    std::vector<uint8_t> image;
    if (rank == 0) image = create_random_image(image_size, new_min, new_max);
    std::vector copyImage(image);

    par_increase_contrast(&image, image_size, new_min, new_max);

    if (rank == 0) {
        uint8_t old_min = *std::min_element(copyImage.begin(), copyImage.end());
        uint8_t old_max = *std::max_element(copyImage.begin(), copyImage.end());

        seq_increase_contrast(&copyImage, old_min, old_max, new_min, new_max);

        ASSERT_EQ(image, copyImage);
    }
}

TEST(MPI_TESTS, Test_all_zero_Matrix) {
    int rank = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    const size_t m = 4;
    const size_t n = 4;
    const uint8_t new_min = 0;
    const uint8_t new_max = 255;

    std::vector<uint8_t> image;
    if (rank == 0) image.resize(m * n, 0);
    std::vector copyImage(image);

    par_increase_contrast(&image, m * n, new_min, new_max);

    if (rank == 0) {
        uint8_t old_min = *std::min_element(copyImage.begin(), copyImage.end());
        uint8_t old_max = *std::max_element(copyImage.begin(), copyImage.end());

        seq_increase_contrast(&copyImage, old_min, old_max, new_min, new_max);

        ASSERT_EQ(image, copyImage);
    }
}

TEST(MPI_TESTS, Test_all_255_Matrix) {
    int rank = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    const size_t m = 4;
    const size_t n = 4;
    const uint8_t new_min = 0;
    const uint8_t new_max = 255;

    std::vector<uint8_t> image;
    if (rank == 0) image.resize(m * n, 255);
    std::vector copyImage(image);

    par_increase_contrast(&image, m * n, new_min, new_max);

    if (rank == 0) {
        uint8_t old_min = *std::min_element(copyImage.begin(), copyImage.end());
        uint8_t old_max = *std::max_element(copyImage.begin(), copyImage.end());

        seq_increase_contrast(&copyImage, old_min, old_max, new_min, new_max);

        ASSERT_EQ(image, copyImage);
    }
}

TEST(MPI_TESTS, Test_empty_Matrix) {
    int rank = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    const size_t m = 0;
    const size_t n = 0;
    const uint8_t new_min = 0;
    const uint8_t new_max = 255;

    std::vector<uint8_t> image{};
    std::vector copyImage(image);

    par_increase_contrast(&image, m * n, new_min, new_max);

    if (rank == 0) {
        ASSERT_EQ(image, copyImage);
    }
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::TestEventListeners& listeners =
        ::testing::UnitTest::GetInstance()->listeners();

    if (MPI_Init(&argc, &argv) != MPI_SUCCESS) MPI_Abort(MPI_COMM_WORLD, -1);

    int rank = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank != 0) {
        delete listeners.Release(listeners.default_result_printer());
    }
    int exec_code = RUN_ALL_TESTS();
    MPI_Finalize();
    return exec_code;
}
