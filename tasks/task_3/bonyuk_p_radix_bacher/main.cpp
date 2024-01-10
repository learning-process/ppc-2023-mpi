  //  Copyright 2023 Bonyuk Petr

#include <mpi.h>
#include <gtest/gtest.h>
#include <iostream>
#include "task_3/bonyuk_p_radix_bacher/radix_bacher.h"

TEST(RadixSortTest, OneTest) {
    std::vector<int> v(180);

    std::srand(std::time(nullptr));
    for (int i = 0; i < 180; ++i) {
        v[i] = std::rand() % 2001 - 1000;
    }
    v = radixSort(v);
    for (int i = 0; i < static_cast<int>(v.size()) - 1; ++i) {
        EXPECT_TRUE(v[i] <= v[i + 1]);
    }
}

TEST(RadixSortTest, TwoTest) {
    std::vector<int> v(180);

    std::srand(std::time(nullptr));
    for (int i = 0; i < 180; ++i) {
         v[i] = std::rand() % 11 - 5;
    }
    v = radixSort(v);
    for (int i = 0; i < static_cast<int>(v.size()) - 1; ++i) {
        EXPECT_TRUE(v[i] <= v[i + 1]);
    }
}

TEST(RadixSortTest, ThreeTest) {
    std::vector<int> v(180);

    std::srand(std::time(nullptr));
    for (int i = 0; i < 180; ++i) {
        v[i] = std::rand() % 101 - 50;
    }
    v = radixSort(v);
    for (int i = 0; i < static_cast<int>(v.size()) - 1; ++i) {
        EXPECT_TRUE(v[i] <= v[i + 1]);
    }
}

TEST(RadixSortTest, FourTest) {
    std::vector<int> v(180);

    std::srand(std::time(nullptr));
    for (int i = 0; i < 180; ++i) {
        v[i] = std::rand() % 201 - 100;
    }
    v = radixSort(v);
    for (int i = 0; i < static_cast<int>(v.size()) - 1; ++i) {
        EXPECT_TRUE(v[i] <= v[i + 1]);
    }
}

TEST(RadixSortTest, MixedPositiveAndNegativeTest) {
    std::vector<int> v(180);

    std::srand(std::time(nullptr));
    for (int i = 0; i < 180; ++i) {
        v[i] = std::rand() % 2001 - 1000;
    }
    v = radixSort(v);
    for (int i = 0; i < static_cast<int>(v.size()) - 1; ++i) {
        EXPECT_TRUE(v[i] <= v[i + 1]);
    }
}

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);

    ::testing::InitGoogleTest(&argc, argv);
    int result = RUN_ALL_TESTS();

    MPI_Finalize();
    return result;
}
