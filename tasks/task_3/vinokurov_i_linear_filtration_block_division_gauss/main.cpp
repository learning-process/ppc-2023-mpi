// Copyright 2023 Vinokurov Ivan
#include <gtest/gtest.h>
#include <vector>
#include <iostream>
#include "task_3/vinokurov_i_linear_filtration_block_division_gauss/linear_filtration_block_division_gauss.h"

TEST(TESTS, CanUseFunctionTest) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<std::vector<unsigned char>> image = {
        {100, 120, 130, 110},
        {110, 115, 120, 125},
        {90, 100, 110, 120},
        {95, 105, 115, 125}
    };

    std::vector<std::vector<unsigned char>> result;

    ASSERT_NO_THROW(result = applyFilterMPI(image));
}

TEST(TESTS, CanWork4x4Test) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<std::vector<unsigned char>> image = {
        {100, 120, 130, 110},
        {110, 115, 120, 125},
        {90, 100, 110, 120},
        {95, 105, 115, 125}
    };

    std::vector<std::vector<unsigned char>> result;
    std::vector<std::vector<unsigned char>> result2;

    result = applyFilter(image);
    result2 = applyFilterMPI(image);

    for (int i = 0; i < image.size(); i++) {
        for (int j = 0; j < image[0].size(); j++) {
            ASSERT_EQ(result[i][j], result2[i][j]);
        }
    }
}

TEST(TESTS, CanWork8x10Test) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<std::vector<unsigned char>> image = {
        {150, 145, 140, 135, 130, 125, 120, 115, 110, 105},
        {145, 140, 135, 130, 125, 120, 115, 110, 105, 100},
        {140, 135, 130, 125, 120, 115, 110, 105, 100, 95},
        {135, 130, 125, 120, 115, 110, 105, 100, 95, 90},
        {130, 125, 120, 115, 110, 105, 100, 95, 90, 85},
        {125, 120, 115, 110, 105, 100, 95, 90, 85, 80},
        {120, 115, 110, 105, 100, 95, 90, 85, 80, 75},
        {115, 110, 105, 100, 95, 90, 85, 80, 75, 70}
    };

    std::vector<std::vector<unsigned char>> result;
    std::vector<std::vector<unsigned char>> result2;

    result = applyFilter(image);
    result2 = applyFilterMPI(image);

    for (int i = 0; i < image.size(); i++) {
        for (int j = 0; j < image[0].size(); j++) {
            ASSERT_EQ(result[i][j], result2[i][j]);
        }
    }
}

TEST(TESTS, CanWork8x4Test) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<std::vector<unsigned char>> image = {
        {150, 145, 140, 135},
        {145, 140, 135, 130},
        {140, 135, 130, 125},
        {135, 130, 125, 120},
        {130, 125, 120, 115},
        {125, 120, 115, 110},
        {120, 115, 110, 105},
        {115, 110, 105, 100}
    };

    std::vector<std::vector<unsigned char>> result;
    std::vector<std::vector<unsigned char>> result2;

    result = applyFilter(image);
    result2 = applyFilterMPI(image);

    for (int i = 0; i < image.size(); i++) {
        for (int j = 0; j < image[0].size(); j++) {
            ASSERT_EQ(result[i][j], result2[i][j]);
        }
    }
}

TEST(TESTS, CanWorkPitchBlackTest) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<std::vector<unsigned char>> image = {
        {0, 0, 0, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0}
    };

    std::vector<std::vector<unsigned char>> result;
    std::vector<std::vector<unsigned char>> result2;

    result = applyFilter(image);
    result2 = applyFilterMPI(image);

    for (int i = 0; i < image.size(); i++) {
        for (int j = 0; j < image[0].size(); j++) {
            ASSERT_EQ(result[i][j], result2[i][j]);
        }
    }
}

TEST(TESTS, CanWorkWhiteTest) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<std::vector<unsigned char>> image = {
        {255, 255, 255, 255},
        {255, 255, 255, 255},
        {255, 255, 255, 255},
        {255, 255, 255, 255}
    };

    std::vector<std::vector<unsigned char>> result;
    std::vector<std::vector<unsigned char>> result2;

    result = applyFilter(image);
    result2 = applyFilterMPI(image);

    for (int i = 0; i < image.size(); i++) {
        for (int j = 0; j < image[0].size(); j++) {
            ASSERT_EQ(result[i][j], result2[i][j]);
        }
    }
}

TEST(TESTS, CannotWorkWithEmptyTest) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<std::vector<unsigned char>> image;

    std::vector<std::vector<unsigned char>> result;

    ASSERT_ANY_THROW(result = applyFilterMPI(image));
}

int main(int argc, char** argv) {
  MPI_Init(&argc, &argv);
  ::testing::InitGoogleTest(&argc, argv);
  int result = RUN_ALL_TESTS();
  MPI_Finalize();
  return result;
}
