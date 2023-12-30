// Copyright 2023 Platonova Maria
#include <gtest/gtest.h>
#include <iostream>
#include "./scatter.h"

TEST(MPI_TESTS, TEST_MPI_INT) {
    int rank = 0;
    int worldSize = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &worldSize);

    std::vector<int> globalVec = {1, 2, 3, 4, 5, 6, 7, 8, 9, 0};
    std::vector<int> sendCounts(worldSize);

    int n = globalVec.size();
    int chank = n / worldSize;

    for (int i = 0; i < worldSize; ++i)
        sendCounts[i] = chank;

    std::vector<int> MyScatter_localVec(sendCounts[rank]);
    std::vector<int> MPI_Scatter_localVec(sendCounts[rank]);

    MyScatter(globalVec.data(), chank, MPI_INT, MyScatter_localVec.data(),
        sendCounts[rank], MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Scatter(globalVec.data(), chank, MPI_INT, MPI_Scatter_localVec.data(),
        sendCounts[rank], MPI_INT, 0, MPI_COMM_WORLD);

    ASSERT_EQ(MyScatter_localVec, MPI_Scatter_localVec);
}

TEST(MPI_TESTS, TEST_MPI_FLOAT) {
    int rank = 0;
    int worldSize = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &worldSize);

    std::vector<float> globalVec =
        {1.1, 2.2, 3.3, 4.4, 5.5, 6.6, 7.7, 8.8, 9.9, 0.5};
    std::vector<int> sendCounts(worldSize);

    int n = globalVec.size();
    int chank = n / worldSize;

    for (int i = 0; i < worldSize; ++i)
        sendCounts[i] = chank;

    std::vector<float> MyScatter_localVec(sendCounts[rank]);
    std::vector<float> MPI_Scatter_localVec(sendCounts[rank]);

    MPI_Scatter(globalVec.data(), chank, MPI_FLOAT,
            MPI_Scatter_localVec.data(), sendCounts[rank],
            MPI_FLOAT, 0, MPI_COMM_WORLD);
    MyScatter(globalVec.data(), chank, MPI_FLOAT,
            MyScatter_localVec.data(), sendCounts[rank],
            MPI_FLOAT, 0, MPI_COMM_WORLD);

    ASSERT_EQ(MyScatter_localVec, MPI_Scatter_localVec);
}

TEST(MPI_TESTS, TEST_MPI_DOUBLE) {
    int rank = 0;
    int worldSize = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &worldSize);

    std::vector<double> globalVec =
        {1.1, 2.2, 3.3, 4.4, 5.5, 6.6, 7.7, 8.8, 9.9, 0.5};
    std::vector<int> sendCounts(worldSize);

    int n = globalVec.size();
    int chank = n / worldSize;

    for (int i = 0; i < worldSize; ++i)
        sendCounts[i] = chank;

    std::vector<double> MyScatter_localVec(sendCounts[rank]);
    std::vector<double> MPI_Scatter_localVec(sendCounts[rank]);

    MPI_Scatter(globalVec.data(), chank, MPI_DOUBLE,
            MPI_Scatter_localVec.data(), sendCounts[rank],
            MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MyScatter(globalVec.data(), chank, MPI_DOUBLE,
            MyScatter_localVec.data(), sendCounts[rank],
            MPI_DOUBLE, 0, MPI_COMM_WORLD);

    ASSERT_EQ(MyScatter_localVec, MPI_Scatter_localVec);
}

TEST(MPI_TESTS, TEST_MPI_INT_EMPTY) {
    int rank = 0;
    int worldSize = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &worldSize);

    std::vector<int> globalVec;
    std::vector<int> sendCounts(worldSize);

    int n = globalVec.size();
    int chank = n / worldSize;

    for (int i = 0; i < worldSize; ++i)
        sendCounts[i] = chank;

    std::vector<int> MyScatter_localVec(sendCounts[rank]);
    std::vector<int> MPI_Scatter_localVec(sendCounts[rank]);

    MPI_Scatter(globalVec.data(), chank, MPI_INT,
            MPI_Scatter_localVec.data(), sendCounts[rank],
            MPI_INT, 0, MPI_COMM_WORLD);
    MyScatter(globalVec.data(), chank, MPI_INT,
            MyScatter_localVec.data(), sendCounts[rank],
            MPI_INT, 0, MPI_COMM_WORLD);

    ASSERT_EQ(MyScatter_localVec, MPI_Scatter_localVec);
}

TEST(MPI_TESTS, TEST_MPI_INT_RANDOM) {
    int rank = 0;
    int worldSize = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &worldSize);

    std::vector<int> globalVec = randomVector(100000, -10000, 10000);
    std::vector<int> sendCounts(worldSize);

    int n = globalVec.size();
    int chank = n / worldSize;

    for (int i = 0; i < worldSize; ++i)
        sendCounts[i] = chank;

    std::vector<int> MyScatter_localVec(sendCounts[rank]);
    std::vector<int> MPI_Scatter_localVec(sendCounts[rank]);

    MPI_Scatter(globalVec.data(), chank, MPI_INT,
                MPI_Scatter_localVec.data(), sendCounts[rank],
                MPI_INT, 0, MPI_COMM_WORLD);
    MyScatter(globalVec.data(), chank, MPI_INT,
                MyScatter_localVec.data(), sendCounts[rank],
                MPI_INT, 0, MPI_COMM_WORLD);

    ASSERT_EQ(MyScatter_localVec, MPI_Scatter_localVec);
}

int main(int argc, char** argv) {
    int result = 0;
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::TestEventListeners& listeners =
        ::testing::UnitTest::GetInstance()->listeners();

    MPI_Init(&argc, &argv);
    int worldRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &worldRank);

    if (worldRank != 0)
        delete listeners.Release(listeners.default_result_printer());

    result = RUN_ALL_TESTS();
    MPI_Finalize();

    return result;
}
