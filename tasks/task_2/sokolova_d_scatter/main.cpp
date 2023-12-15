// Copyright 2023 Sokolova Daria
#include <gtest/gtest.h>
#include "./scatter.h"
void scatter(void* sendbuf, int sendcount, MPI_Datatype sendtype,
    void* recvbuf, int recvcount, MPI_Datatype recvtype,
    int root, MPI_Comm comm);

TEST(MPI_TESTS, TEST_MPI_CHAR) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<char> sendbuf(size);
    std::vector<char> recvbuf(1);

    for (int i = 0; i < size; ++i) {
        sendbuf[i] = static_cast<char>('a' + i);
    }

    scatter(sendbuf.data(), 1, MPI_CHAR,
        recvbuf.data(), 1, MPI_CHAR, 0, MPI_COMM_WORLD);

    char expected_result = (rank == 0) ? 'a' : 'a' + rank;
    ASSERT_EQ(recvbuf[0], expected_result);
}
TEST(MPI_TESTS, TEST_MPI_INT) {
    int rank = 0;
    int worldSize = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &worldSize);

    std::vector<int> globalVec = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 0 };
    std::vector<int> sendCounts(worldSize);

    int n = globalVec.size();
    int chank = n / worldSize;

    for (int i = 0; i < worldSize; ++i)
        sendCounts[i] = chank;

    std::vector<int> MyScatter_localVec(sendCounts[rank]);
    std::vector<int> MPI_Scatter_localVec(sendCounts[rank]);

    scatter(globalVec.data(), chank, MPI_INT,
        MyScatter_localVec.data(), sendCounts[rank],
        MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Scatter(globalVec.data(), chank, MPI_INT,
        MPI_Scatter_localVec.data(), sendCounts[rank],
        MPI_INT, 0, MPI_COMM_WORLD);

    ASSERT_EQ(MyScatter_localVec, MPI_Scatter_localVec);
}
TEST(MPI_TESTS, TEST_MPI_FLOAT) {
    int rank = 0;
    int worldSize = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &worldSize);

    std::vector<float> globalVec =
    { 1.1, 2.2, 3.3, 4.4, 5.5, 6.6, 7.7, 8.8, 9.9, 0.5 };
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
    scatter(globalVec.data(), chank, MPI_FLOAT,
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
    { 1.1, 2.2, 3.3, 4.4, 5.5, 6.6, 7.7, 8.8, 9.9, 0.5 };
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
    scatter(globalVec.data(), chank, MPI_DOUBLE,
       MyScatter_localVec.data(), sendCounts[rank],
        MPI_DOUBLE, 0, MPI_COMM_WORLD);

    ASSERT_EQ(MyScatter_localVec, MPI_Scatter_localVec);
}
struct UserStruct {
    float x;
    float y;
    float z;
};
TEST(ScatterTest, Struct3DScatterTest) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<UserStruct> sendbuf(size);
    std::vector<UserStruct> recvbuf(1);

    for (int i = 0; i < size; ++i) {
        sendbuf[i].x = i * 1.5f;
        sendbuf[i].y = i * 2.5f;
        sendbuf[i].z = i * 3.5f;
    }

    scatter(sendbuf.data(), 1, MPI_FLOAT,
        recvbuf.data(),
        1, MPI_FLOAT, 0, MPI_COMM_WORLD);

    UserStruct expected_result;
    expected_result.x = (rank == 0) ? 0.0f : 1.5f * rank;
    expected_result.y = (rank == 0) ? 0.0f : 2.5f * rank;
    expected_result.z = (rank == 0) ? 0.0f : 3.5f * rank;

    ASSERT_FLOAT_EQ(recvbuf[0].x, expected_result.x);
    ASSERT_FLOAT_EQ(recvbuf[0].y, expected_result.y);
    ASSERT_FLOAT_EQ(recvbuf[0].z, expected_result.z);
}
int main(int argc, char** argv) {
    int resultCode = 0;
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::TestEventListeners& listeners =
        ::testing::UnitTest::GetInstance()->listeners();

    if (MPI_Init(&argc, &argv) != MPI_SUCCESS)
        MPI_Abort(MPI_COMM_WORLD, -1);
    resultCode = RUN_ALL_TESTS();
    MPI_Finalize();

    return resultCode;
}
