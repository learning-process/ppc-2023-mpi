// Copyright 2023 Khramov Ivan
#include <gtest/gtest.h>
#include <mpi.h>
#include "./global_search.h"

const double E = 0.01;



void GlobalSearchTesting(std::function<double(double)> t_func,
    const double a, const double b, double expectedMin, double mult = 1) {
    int rank, count;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &count);

    double resultMpi = getGlobalSearchParallel(a, b, t_func, E * 0.01 * mult);
    double resultSeq = getGlobalSearchSequantial(a, b, t_func, E * 0.01 * mult);

    if (rank == 0) {
        ASSERT_NEAR(expectedMin, resultMpi, E / mult);
        ASSERT_NEAR(expectedMin, resultSeq, E / mult);
    }
}

TEST(Global_Search_MPI, Cos_Test) {
    auto function = [](double x) { return std::cos(x); };
    GlobalSearchTesting(function, -PI, PI, -1);
}

TEST(Global_Search_MPI, Sin_Test) {
    auto function = [](double x) { return std::sin(x); };
    GlobalSearchTesting(function, -PI/2, PI/2, -1);
}

TEST(Global_Search_MPI, Linear_Test) {
    auto function = [](double x) { return x; };
    GlobalSearchTesting(function, -3, 3, -3);
}

TEST(Global_Search_MPI, Cube_Test) {
    auto function = [](double x) { return pow(x, 3); };
    GlobalSearchTesting(function, 10, 20, 1000, 0.1);
}

TEST(Global_Search_MPI, Sqrt_Test) {
    auto function = [](double x) { return sqrt(x); };
    GlobalSearchTesting(function, 16, 20, 4);
}

TEST(Global_Search_MPI, Square_Test) {
    auto function = [](double x) { return pow(x, 2); };
    GlobalSearchTesting(function, -20, 20, 0);
}

TEST(Global_Search_MPI, Hard_Test) {
    auto func = [](double x) { return std::sin(x * 2) * 3 + cos(x / 2) * 3; };
    GlobalSearchTesting(func, PI, 2*PI, -5.78);
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    ::testing::InitGoogleTest(&argc, argv);
    auto& listeners = ::testing::UnitTest::GetInstance()->listeners();

    if (world_rank != 0) {
        delete listeners.Release(listeners.default_result_printer());
    }
    int result = RUN_ALL_TESTS();
    MPI_Finalize();

    return result;
}
