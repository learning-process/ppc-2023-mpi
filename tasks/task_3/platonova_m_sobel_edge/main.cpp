// Copyright 2023 Platonova Maria
#include <gtest/gtest.h>
#include <random>
#include <vector>
#include "task_3/platonova_m_sobel_edge/sobel_edge.h"
#include <boost/mpi/environment.hpp>
#include <boost/mpi/communicator.hpp>

ImageMatrix GenerateRandomImage(size_t width, size_t height) {
    ImageMatrix image(width * height);

    std::random_device dev;
    std::mt19937 gen(dev());

    for (int j = 0; j < height; ++j) {
        for (int i = 0; i < width; ++i) {
            image[j * width + i] = gen() % 256;
        }
    }

    return std::move(image);
}

TEST(sequential_sobel, image_3x3) {
    ImageMatrix matrix = {
        100, 200, 60,
        81, 21, 240,
        180, 65, 96
    };

    ImageMatrix correct = {
        247
    };

    auto ourSobelSequential = SequentialSobelOperator(matrix, 3, 3);

    ASSERT_EQ(correct, ourSobelSequential);
}

TEST(sequential_sobel, image_10x10) {
    ImageMatrix matrix = {
        41, 104,   6, 208, 106,  57, 207,  53, 236, 222,
        253,  49,  52,  21,  58,  94,  26, 222,  15, 186,
        8, 137,  81, 195, 185, 197, 212,   3, 224, 124,
        241, 231, 222, 212,  44,  86, 151,  64, 244, 153,
        186,  27, 207,  78,  27, 210,  13,  61, 184,  79,
        166, 255, 110,  39, 245, 120, 168,   5,   5, 240,
        3, 196,  53, 139, 134,  17,  99, 102, 184,  63,
        28, 187,   8,  13, 124, 107, 179,  78,  67, 107,
        85,  21,  80, 251, 214, 220,  80,  25, 192,  26,
        88, 206,  25, 200, 128,  23, 187,  59,  55, 251,
    };

    ImageMatrix correctSobelApplication = {
        255, 200, 251, 255, 255, 115, 108, 255,
        255, 255, 255, 162, 160, 255, 112, 255,
        100, 75, 255, 255, 255, 255, 255, 255,
        142, 255, 255, 255, 255, 255, 255, 255,
        41, 255, 186, 190, 255, 255, 182, 255,
        255, 255, 255, 255, 164, 86, 242, 196,
        241, 175, 255, 255, 255, 189, 167, 100,
        147, 255, 255, 182, 219, 255, 46, 244,
    };

    auto ourSobelSequential = SequentialSobelOperator(matrix, 10, 10);

    ASSERT_EQ(correctSobelApplication, ourSobelSequential);
}

TEST(sequential_sobel, image_5x15) {
    ImageMatrix matrix = {
        126, 169, 252,  71, 131,
        228,  69,  93,   8,  85,
        142,  12, 101,  97,  63,
        20,   9, 237,  57, 107,
        152, 223, 230,  83, 220,
        37, 102,  26, 115, 118,
        221,  23, 104, 161, 159,
        239,  10, 106, 146, 242,
        116, 239, 233, 110,  13,
        93,  48, 182, 175,  20,
        15, 208,   1,  31, 101,
        218, 154, 245, 127,  57,
        93, 241,  32, 150,  83,
        48,  20,  97,  49, 137,
        114,   6,  11, 156, 121
    };

    ImageMatrix correctSobelApplication = {
        255, 255, 241,
        184, 255, 255,
        255, 255, 255,
        255, 255, 102,
        255, 255, 231,
        255, 255, 255,
        255, 255, 160,
        190, 219, 255,
        255, 255, 255,
        255, 255, 150,
        176, 255, 255,
        255, 255, 228,
        255, 255, 242,
    };

    auto ourSobelSequential = SequentialSobelOperator(matrix, 5, 15);

    ASSERT_EQ(correctSobelApplication, ourSobelSequential);
}


TEST(parallel_sobel, image_5x7) {
    boost::mpi::communicator world;

    size_t height = 5;
    size_t width = 7;

    ImageMatrix image;

    if (world.rank() == 0) {
        image = {
            1, 3, 5, 6, 7, 8, 1,
            1, 5, 1, 6, 1, 5, 8,
            9, 9, 9, 9, 9, 9, 9,
            7, 7, 6, 0, 7, 7, 7,
            4, 4, 4, 6, 1, 7, 8
        };
    }

    auto parallelResult = ParallelSobelOperator(image, width, height);

    if (world.rank() == 0) {
        auto sequentialResult = SequentialSobelOperator(image, width, height);
        ASSERT_EQ(sequentialResult, parallelResult);
    }
}

TEST(parallel_sobel, random_image_3x3) {
    boost::mpi::communicator world;

    size_t height = 3;
    size_t width = 3;

    ImageMatrix image;

    if (world.rank() == 0) {
        image = GenerateRandomImage(width, height);
    }

    auto parallelResult = ParallelSobelOperator(image, width, height);

    if (world.rank() == 0) {
        auto sequentialResult = SequentialSobelOperator(image, width, height);
        ASSERT_EQ(sequentialResult, parallelResult);
    }
}

TEST(parallel_sobel, random_image_200x100) {
    boost::mpi::communicator world;

    size_t height = 200;
    size_t width = 100;

    ImageMatrix image;

    if (world.rank() == 0) {
        image = GenerateRandomImage(width, height);
    }

    auto parallelResult = ParallelSobelOperator(image, width, height);

    if (world.rank() == 0) {
        auto sequentialResult = SequentialSobelOperator(image, width, height);
        ASSERT_EQ(sequentialResult, parallelResult);
    }
}

TEST(parallel_sobel, random_image_300x400) {
    boost::mpi::communicator world;

    size_t height = 300;
    size_t width = 400;

    ImageMatrix image;

    if (world.rank() == 0) {
        image = GenerateRandomImage(width, height);
    }

    auto parallelResult = ParallelSobelOperator(image, width, height);

    if (world.rank() == 0) {
        auto sequentialResult = SequentialSobelOperator(image, width, height);
        ASSERT_EQ(sequentialResult, parallelResult);
    }
}
int main(int argc, char** argv) {
    boost::mpi::environment env(argc, argv);
    boost::mpi::communicator communicator;
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::TestEventListeners& listeners =
        ::testing::UnitTest::GetInstance()->listeners();
    if (communicator.rank()) {
        delete listeners.Release(listeners.default_result_printer());
    }
    return RUN_ALL_TESTS();
}
