// Copyright 2023 Kirillov Maxim
#include <gtest/gtest.h>
#include <vector>
#include "./radix_simple_double.h"
#include <boost/mpi/environment.hpp>
#include <boost/mpi/communicator.hpp>

TEST(Radix_Double_Simple_Merge, Test_Radix1) {
    boost::mpi::communicator world;
    std::vector<double> vec;
    int size = 20;
    double min = 1, max = 50;
    if (world.rank() == 0) {
        vec = getRandomVector(size, min, max);
    }
    auto par = getParRadixSortSimpleMerge(vec);
    if (world.rank() == 0) {
        auto seq = getSeqRadixSortSimpleMerge(vec);
        for (size_t i = 0; i < vec.size(); ++i) {
            EXPECT_NEAR(seq[i], par[i], 1e-5);
        }
    }
}

TEST(Radix_Double_Simple_Merge, Test_Radix2) {
    boost::mpi::communicator world;
    std::vector<double> vec;
    int size = 40;
    double min = 1, max = 50;
    if (world.rank() == 0) {
        vec = getRandomVector(size, min, max);
    }
    auto par = getParRadixSortSimpleMerge(vec);
    if (world.rank() == 0) {
        auto seq = getSeqRadixSortSimpleMerge(vec);
        for (size_t i = 0; i < vec.size(); ++i) {
            EXPECT_NEAR(seq[i], par[i], 1e-5);
        }
    }
}

TEST(Radix_Double_Simple_Merge, Test_Radix3) {
    boost::mpi::communicator world;
    std::vector<double> vec;
    int size = 80;
    double min = 1, max = 50;
    if (world.rank() == 0) {
        vec = getRandomVector(size, min, max);
    }
    auto par = getParRadixSortSimpleMerge(vec);
    if (world.rank() == 0) {
        auto seq = getSeqRadixSortSimpleMerge(vec);
        for (size_t i = 0; i < vec.size(); ++i) {
            EXPECT_NEAR(seq[i], par[i], 1e-5);
        }
    }
}

TEST(Radix_Double_Simple_Merge, Test_Radix4) {
    boost::mpi::communicator world;
    std::vector<double> vec;
    int size = 100;
    double min = 1, max = 50;
    if (world.rank() == 0) {
        vec = getRandomVector(size, min, max);
    }
    auto par = getParRadixSortSimpleMerge(vec);
    if (world.rank() == 0) {
        auto seq = getSeqRadixSortSimpleMerge(vec);
        for (size_t i = 0; i < vec.size(); ++i) {
            EXPECT_NEAR(seq[i], par[i], 1e-5);
        }
    }
}

TEST(Radix_Double_Simple_Merge, Test_Radix5) {
    boost::mpi::communicator world;
    std::vector<double> vec, sorted;
    int size = 200;
    double min = 1, max = 50;
    if (world.rank() == 0) {
        vec = getRandomVector(size, min, max);
        sorted = vec;
        std::sort(sorted.begin(), sorted.end());
    }
    auto par = getParRadixSortSimpleMerge(vec);
    if (world.rank() == 0) {
        auto seq = getSeqRadixSortSimpleMerge(vec);
        for (size_t i = 0; i < vec.size(); ++i) {
            EXPECT_NEAR(seq[i], par[i], 1e-5);
        }
    }
}


int main(int argc, char** argv) {
    boost::mpi::environment env(argc, argv);
    boost::mpi::communicator world;
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::TestEventListeners& listeners =
            ::testing::UnitTest::GetInstance()->listeners();
    if (world.rank() != 0) {
        delete listeners.Release(listeners.default_result_printer());
    }
    return RUN_ALL_TESTS();
}
