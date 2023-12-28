// Copyright 2023 Platonova Maria
#include <gtest/gtest.h>
#include <boost/mpi/communicator.hpp>
#include <boost/mpi/collectives.hpp>
#include "task_3/platonova_m_sobel_edge/sobel_edge.h"

TEST(STD_Sobel, Empty_Image) {
    std::vector<std::vector<Pixel>> image = white_image(15, 15);
    std::vector<std::vector<Pixel>> excected = black_image(15, 15);
    std::vector<std::vector<Pixel>> result = sobel_std(image);
    ASSERT_EQ(true, true);
}

TEST(STD_Sobel, Image_3x3_Mono) {
    std::vector<std::vector<Pixel>> image = {
        { {255, 255, 0}, {255, 255, 0}, {255, 255, 0} },
        { {255, 255, 0}, {255, 255, 0}, {255, 255, 0} },
        { {255, 255, 0}, {255, 255, 0}, {255, 255, 0} }
    };
    std::vector<std::vector<Pixel>> expected = sobel(image);
    std::vector<std::vector<Pixel>> result = sobel_std(image);
    ASSERT_EQ(expected, result);
}

TEST(STD_Sobel, Image_3x3_Duo) {
    std::vector<std::vector<Pixel>> image = {
        { {255, 255, 0}, {255, 255, 0}, {0, 0, 255} },
        { {255, 255, 0}, {255, 255, 0}, {0, 0, 255} },
        { {0, 0, 255}, {0, 0, 255}, {0, 0, 255} }
    };
    std::vector<std::vector<Pixel>> expected = sobel(image);
    std::vector<std::vector<Pixel>> result = sobel_std(image);
    ASSERT_EQ(expected, result);
}

TEST(STD_Sobel, Image_3x3_Triple) {
    std::vector<std::vector<Pixel>> image = {
        { {255, 0, 0}, {255, 0, 0}, {255, 0, 0} },
        { {0, 255, 0}, {0, 255, 0}, {0, 255, 255} },
        { {0, 255, 0}, {0, 255, 0}, {0, 255, 255} }
    };
    std::vector<std::vector<Pixel>> expected = sobel(image);
    std::vector<std::vector<Pixel>> result = sobel_std(image);
    ASSERT_EQ(expected, result);
}

TEST(STD_Sobel, Image_300x300) {
    std::vector<std::vector<Pixel>> image = random_image(300, 300);
    std::vector<std::vector<Pixel>> expected = sobel(image);
    std::vector<std::vector<Pixel>> result = sobel_std(image);
    ASSERT_EQ(expected, result);
}

TEST(STD_Sobel, Image_500x2000) {
    std::vector<std::vector<Pixel>> image = random_image(500, 2000);
    std::vector<std::vector<Pixel>> expected = sobel(image);
    std::vector<std::vector<Pixel>> result = sobel_std(image);
    ASSERT_EQ(expected, result);
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
