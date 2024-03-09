// Copyright 2023 Khavronina Alexandra

#include <gtest/gtest.h>
#include "./scatter.h"

void scatter(void* send_buf, int send_count, MPI_Datatype send_type, void* recv_buf,
     int recv_count, MPI_Datatype recv_type, int root, MPI_Comm comm);

TEST(Scatter_MPI_Test, MPI_Char_Test) {
    int rank;
    int size;

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<char> send_buf(size);
    std::vector<char> recv_buf(1);

    for (int i = 0; i < size; i++) {
        send_buf[i] = static_cast<char>('a' + i);
    }

    scatter(send_buf.data(), 1, MPI_CHAR, recv_buf.data(), 1, MPI_CHAR, 0, MPI_COMM_WORLD);

    char res;
    res = (rank == 0) ? 'a' : 'a' + rank;
    ASSERT_EQ(recv_buf[0], res);
}

TEST(Scatter_MPI_Test, MPI_Int_Test) {
    int rank = 0;
    int world_size = 0;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    std::vector<int> global_vector = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 0 };
    std::vector<int> send_counts(world_size);

    int n = global_vector.size();
    int chank = n / world_size;

    for (int i = 0; i < world_size; i++)
        send_counts[i] = chank;

    std::vector<int> scatter_local_vector(send_counts[rank]);
    std::vector<int> MPI_scatter_local_vector(send_counts[rank]);

    scatter(global_vector.data(), chank, MPI_INT, scatter_local_vector.data(),
        send_counts[rank], MPI_INT, 0, MPI_COMM_WORLD);

    MPI_Scatter(global_vector.data(), chank, MPI_INT, MPI_scatter_local_vector.data(), send_counts[rank],
        MPI_INT, 0, MPI_COMM_WORLD);

    ASSERT_EQ(scatter_local_vector, MPI_scatter_local_vector);
}

TEST(Scatter_MPI_Test, MPI_Float_Test) {
    int rank = 0;
    int world_size = 0;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    std::vector<float> global_vector = { 1.1, 2.2, 3.3, 4.4, 5.5, 6.6, 7.7, 8.8, 9.9, 0.5 };
    std::vector<int> send_counts(world_size);

    int n = global_vector.size();
    int chank = n / world_size;

    for (int i = 0; i < world_size; i++)
        send_counts[i] = chank;

    std::vector<float> scatter_local_vector(send_counts[rank]);
    std::vector<float> MPI_scatter_local_vector(send_counts[rank]);

    MPI_Scatter(global_vector.data(), chank, MPI_FLOAT, MPI_scatter_local_vector.data(), send_counts[rank],
        MPI_FLOAT, 0, MPI_COMM_WORLD);

    scatter(global_vector.data(), chank, MPI_FLOAT, scatter_local_vector.data(), send_counts[rank],
        MPI_FLOAT, 0, MPI_COMM_WORLD);

    ASSERT_EQ(scatter_local_vector, MPI_scatter_local_vector);
}

TEST(Scatter_MPI_Test, MPI_Double_Test) {
    int rank = 0;
    int world_size = 0;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    std::vector<double> global_vector = { 1.1, 2.2, 3.3, 4.4, 5.5, 6.6, 7.7, 8.8, 9.9, 0.5 };
    std::vector<int> send_counts(world_size);

    int n = global_vector.size();
    int chank = n / world_size;

    for (int i = 0; i < world_size; i++)
        send_counts[i] = chank;

    std::vector<double> scatter_local_vector(send_counts[rank]);
    std::vector<double> MPI_scatter_local_vector(send_counts[rank]);

    MPI_Scatter(global_vector.data(), chank, MPI_DOUBLE, MPI_scatter_local_vector.data(), send_counts[rank],
        MPI_DOUBLE, 0, MPI_COMM_WORLD);

    scatter(global_vector.data(), chank, MPI_DOUBLE, scatter_local_vector.data(), send_counts[rank],
        MPI_DOUBLE, 0, MPI_COMM_WORLD);

    ASSERT_EQ(scatter_local_vector, MPI_scatter_local_vector);
}

TEST(Scatter_MPI_Test, Test_1) {
    int size, rank;

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<int> send_data;

    if (rank == 0) {
        send_data.resize(size * 3);
        for (int i = 0; i < size * 3; i++) {
            send_data[i] = i;
        }
    }
    std::vector<int> recv_data(3);
    scatter(send_data.data(), 3, MPI_INT, recv_data.data(), 3, MPI_INT, 0, MPI_COMM_WORLD);

    for (int i = 0; i < 3; ++i) {
        ASSERT_EQ(recv_data[i], (rank * 3) + i);
    }
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
