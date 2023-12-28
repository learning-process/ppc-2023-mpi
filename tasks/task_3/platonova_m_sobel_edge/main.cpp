// Copyright 2023 Krainev Alexander
#include <gtest/gtest.h>
#include "./sobel_edge.h"
#include <boost/mpi/communicator.hpp>
#include <boost/mpi/collectives.hpp>

TEST(STD_Sobel, Empty_Image) {
    Image image = white_image(15, 15);
    Image excected = black_image(15, 15);
    Image result = sobel_std(image);
    ASSERT_EQ(true, true);
}

TEST(STD_Sobel, Image_3x3_Mono) {
    Image image = {
        { {255, 255, 0}, {255, 255, 0}, {255, 255, 0} },
        { {255, 255, 0}, {255, 255, 0}, {255, 255, 0} },
        { {255, 255, 0}, {255, 255, 0}, {255, 255, 0} }
    };
    Image expected = sobel(image);
    Image result = sobel_std(image);
    ASSERT_EQ(expected, result);
}

TEST(STD_Sobel, Image_3x3_Duo) {
    Image image = {
        { {255, 255, 0}, {255, 255, 0}, {0, 0, 255} },
        { {255, 255, 0}, {255, 255, 0}, {0, 0, 255} },
        { {0, 0, 255}, {0, 0, 255}, {0, 0, 255} }
    };
    Image expected = sobel(image);
    Image result = sobel_std(image);
    ASSERT_EQ(expected, result);
}

TEST(STD_Sobel, Image_3x3_Triple) {
    Image image = {
        { {255, 0, 0}, {255, 0, 0}, {255, 0, 0} },
        { {0, 255, 0}, {0, 255, 0}, {0, 255, 255} },
        { {0, 255, 0}, {0, 255, 0}, {0, 255, 255} }
    };
    Image expected = sobel(image);
    Image result = sobel_std(image);
    ASSERT_EQ(expected, result);
}

TEST(STD_Sobel, Image_300x300) {
    Image image = random_image(300, 300);
    Image expected = sobel(image);
    Image result = sobel_std(image);
    ASSERT_EQ(expected, result);
}

TEST(STD_Sobel, Image_500x2000) {
    Image image = random_image(500, 2000);
    Image expected = sobel(image);
    Image result = sobel_std(image);
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
