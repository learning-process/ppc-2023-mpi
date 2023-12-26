// Copyright 2023 Kudinov Nikita
#include <gtest/gtest.h>
#include <boost/mpi.hpp>

#include "task_3/kudinov_n_image_filter_gauss_block/image_filter_gauss_block.h"

#define ASSERT_EQ_IMAGES(image1, image2)                                \
    do {                                                                \
        ASSERT_EQ(image1.size(), image2.size());                        \
        if (image1.size() != 0) {                                       \
            ASSERT_EQ(image1[0].size(), image2[0].size());              \
                                                                        \
            for (std::size_t y = 0; y < image1.size(); y += 1) {        \
                for (std::size_t x = 0; x < image1[0].size(); x += 1) { \
                    ASSERT_EQ(image1[y][x], image2[y][x]);              \
                }                                                       \
            }                                                           \
        }                                                               \
    } while (0)

void base_test(const Image& image, std::size_t kernel_radius, double sigma) {
    boost::mpi::communicator world;

    Image result_parallel = image_gauss_filter_parallel(image, kernel_radius, sigma);

    if (world.rank() == 0) {
        Image result_sequential = image_gauss_filter_sequential(image, kernel_radius, sigma);

        ASSERT_EQ_IMAGES(result_sequential, result_parallel);
    }
}

TEST(Matrix_Multiplication_Vertical, Small_Image_Square) {
    Image image = {{ 59, 2, 130, 50, 76, 17, 226, 35, 105, 183, 237, 212 },
        { 227, 160, 247, 94, 104, 59, 178, 250, 234, 91, 108, 12 },
        { 134, 197, 22, 104, 72, 21, 67, 11, 128, 31, 11, 243 },
        { 167, 95, 213, 96, 180, 17, 93, 25, 179, 235, 178, 163 },
        { 101, 119, 160, 237, 232, 181, 229, 237, 100, 204, 160, 76 },
        { 229, 56, 61, 48, 222, 34, 27, 85, 254, 44, 232, 128 },
        { 217, 254, 85, 115, 135, 72, 225, 1, 90, 66, 4, 221 },
        { 21, 252, 198, 24, 80, 246, 237, 92, 162, 177, 102, 130 },
        { 177, 32, 61, 234, 167, 178, 109, 209, 55, 134, 182, 222 },
        { 77, 17, 145, 22, 192, 58, 65, 39, 177, 90, 219, 88 },
        { 185, 140, 142, 201, 238, 252, 33, 41, 3, 152, 126, 176 },
        { 164, 44, 8, 207, 204, 81, 202, 72, 175, 116, 128, 161}};

    base_test(image, 2, 1.4);
}

TEST(Matrix_Multiplication_Vertical, Small_Random_Image_Square) {
    Image image = generate_random_image(9, 9);

    base_test(image, 3, 1.2);
}

TEST(Matrix_Multiplication_Vertical, Small_Random_Image_Rect) {
    Image image = generate_random_image(9, 13);

    base_test(image, 1, 2.2);
}

TEST(Matrix_Multiplication_Vertical, Medium_Random_Image_Rect) {
    Image image = generate_random_image(63, 68);

    base_test(image, 3, 1.8);
}

TEST(Matrix_Multiplication_Vertical, Medium_Random_Image_Square) {
    Image image = generate_random_image(70, 70);

    base_test(image, 3, 1.8);
}

TEST(Matrix_Multiplication_Vertical, Big_Random_Matrix_Rect) {
    Image image = generate_random_image(201, 203);

    base_test(image, 2, 2.0);
}

TEST(Matrix_Multiplication_Vertical, Big_Random_Matrix_Square) {
    Image image = generate_random_image(202, 202);

    base_test(image, 2, 2.0);
}

TEST(Matrix_Multiplication_Vertical, Empty_Image) {
    Image image = generate_random_image(0, 0);

    base_test(image, 1, 1.0);
}

int main(int argc, char** argv) {
    boost::mpi::environment env(argc, argv);
    boost::mpi::communicator world;
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::TestEventListeners& listeners = ::testing::UnitTest::GetInstance()->listeners();

    if (world.rank() != 0) {
        delete listeners.Release(listeners.default_result_printer());
    }

    return RUN_ALL_TESTS();
}
