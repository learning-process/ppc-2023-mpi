// Copyright 2023 Kulikov Artem
#include <gtest/gtest.h>
#include <vector>
#include <iostream>

#include <boost/mpi/communicator.hpp>
#include <boost/mpi/environment.hpp>
#include <boost/serialization/vector.hpp>

#include "./gauss_hor.h"


TEST(Parallel_Gaussian_Filter, Test_Size) {
    boost::mpi::communicator world;
    int n = 4, m = 4;
    std::vector<double> kern = get3x3GaussKernel(1.0);
    std::vector<uint8_t> pic;


    if (world.rank() == 0) {
        pic = getExtPicture(n, m);
    }

    auto filetered_pic = getParallelGauss(pic, kern, n + 2, m + 2);

    if (world.rank() == 0) {
        auto sec_filtered = getSequentialGauss(pic, kern, n + 2);
        ASSERT_EQ(filetered_pic.size(), sec_filtered.size());
    }
}

TEST(Parallel_Gaussian_Filter, Test_Intensive) {
    boost::mpi::communicator world;
    int n = 100, m = 20;
    std::vector<double> kern = get3x3GaussKernel(1.0);
    std::vector<uint8_t> pic;
    std::uint32_t orig_sum, parallel_sum;


    if (world.rank() == 0) {
        pic = getExtPicture(n, m);
        orig_sum = getPxSum(pic, n + 2);
    }

    auto filetered_pic = getParallelGauss(pic, kern, n + 2, m + 2);

    if (world.rank() == 0) {
        parallel_sum = getPxSum(filetered_pic, n + 2);
        ASSERT_NEAR(parallel_sum, orig_sum, orig_sum*0.01);
    }
}

TEST(Parallel_Gaussian_Filter, Test_RandValues4) {
    boost::mpi::communicator world;
    int n = 4, m = 2;
    std::vector<double> kern = get3x3GaussKernel(1.0);
    std::vector<uint8_t> pic;

    if (world.rank() == 0) {
        pic = getExtPicture(n, m);
    }

    auto filetered_pic = getParallelGauss(pic, kern, n + 2, m + 2);

    if (world.rank() == 0) {
        auto sec_filtered = getSequentialGauss(pic, kern, n + 2);
        for (int i = 0; i < n + 2; i++) {
            for (int j = 0; j < m + 2; j++) {
                ASSERT_EQ(filetered_pic[i * (m + 2) + j], sec_filtered[i * (m + 2) + j]);
            }
        }
    }
}

TEST(Parallel_Gaussian_Filter, Test_RandValues8) {
    boost::mpi::communicator world;
    int n = 8, m = 7;
    std::vector<double> kern = get3x3GaussKernel(1.0);
    std::vector<uint8_t> pic;

    if (world.rank() == 0) {
        pic = getExtPicture(n, m);
    }

    auto filetered_pic = getParallelGauss(pic, kern, n + 2, m + 2);

    if (world.rank() == 0) {
        auto sec_filtered = getSequentialGauss(pic, kern, n + 2);
        for (int i = 0; i < n + 2; i++) {
            for (int j = 0; j < m + 2; j++) {
                ASSERT_EQ(filetered_pic[i * (m + 2) + j], sec_filtered[i * (m + 2) + j]);
            }
        }
    }
}

TEST(Parallel_Gaussian_Filter, Test_Horizontal_Crosses_Small_Sd) {
    boost::mpi::communicator world;
    int n = 4, m = 4;
    std::vector<double> kern = get3x3GaussKernel(0.2);
    std::vector<uint8_t> pic;

    if (world.rank() == 0) {
        pic = {
         255, 255, 255, 255, 255, 255,
         255, 255, 255, 255, 255, 255,
         0,     0, 255,   0, 255, 255,
         255, 255,   0, 255, 255, 255,
         0,    0,  255,   0, 255, 255,
         0,    0,  255,   0, 255, 255
        };
    }

    auto filetered_pic = getParallelGauss(pic, kern, n + 2, m + 2);

    if (world.rank() == 0) {
        std::vector<uint8_t> sec_filtered = {
            255, 255, 255, 255, 255, 255,
            255, 254, 254, 254, 254, 255,
            0,     0, 254,   0, 254, 255,
            255, 254,   0, 254, 254, 255,
            0,     0, 254,   0, 254, 255,
            0,     0, 255,   0, 255, 255
        };

        for (int i = 0; i < n + 2; i++) {
            for (int j = 0; j < m + 2; j++) {
                ASSERT_EQ(filetered_pic[i * (m + 2) + j], sec_filtered[i * (m + 2) + j]);
            }
        }
    }
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
