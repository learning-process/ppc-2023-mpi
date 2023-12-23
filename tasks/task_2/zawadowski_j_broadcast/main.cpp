// Copyright 2023 Zawadowski Jan
#include <gtest/gtest.h>
#include "./broadcast.h"

TEST(Broadcast, Test_MPI_INT) {
    std::vector<int> vector = { 1, 4, 3, 7, 5, 9 }, local;
    int sizeWorld, rank, root = 0, vectorSize = vector.size();
    MPI_Comm_size(MPI_COMM_WORLD, &sizeWorld);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dist(0, sizeWorld - 1);
        root = dist(gen);
    }
    broadcast(&root, 1, MPI_INT, 0, MPI_COMM_WORLD);
    if (rank == root) local = std::vector<int>(vector.begin(), vector.end());
    else
        local.resize(vectorSize);
    broadcast(local.data(), vectorSize, MPI_INT, root, MPI_COMM_WORLD);
    if (rank == root)
        ASSERT_EQ(vector, local);
}

TEST(Broadcast, Test_MPI_FLOAT) {
    std::vector<float> vector = { 0.4, 4.6, 3.2, 7.9, 5.1, 10.3 }, local;
    int sizeWorld, rank, root = 0, vectorSize = vector.size();
    MPI_Comm_size(MPI_COMM_WORLD, &sizeWorld);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dist(0, sizeWorld - 1);
        root = dist(gen);
    }
    broadcast(&root, 1, MPI_FLOAT, 0, MPI_COMM_WORLD);
    if (rank == root) local = std::vector<float>(vector.begin(), vector.end());
    else
        local.resize(vectorSize);
    broadcast(local.data(), vectorSize, MPI_FLOAT, root, MPI_COMM_WORLD);
    if (rank == root)
        ASSERT_EQ(vector, local);
}

TEST(Broadcast, Test_MPI_DOUBLE) {
    std::vector<double> vector = { 1.4, 2.3, 7.1, 4.5, 8.6, 7.0 }, local;
    int sizeWorld, rank, root = 0, vectorSize = vector.size();
    MPI_Comm_size(MPI_COMM_WORLD, &sizeWorld);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dist(0, sizeWorld - 1);
        root = dist(gen);
    }
    broadcast(&root, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    if (rank == root) local = std::vector<double>(vector.begin(), vector.end());
    else
        local.resize(vectorSize);
    broadcast(local.data(), vectorSize, MPI_DOUBLE, root, MPI_COMM_WORLD);
    if (rank == root)
        ASSERT_EQ(vector, local);
}

TEST(Broadcast, Test_time) {
    int sizeWorld, rank, sum = 0,
        root = 0, size = 10;
    double t1, t2, t3, t4;
    MPI_Comm_size(MPI_COMM_WORLD, &sizeWorld);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> vector(size);

    if (rank == root) {
        for (int i = 0; i < size; i++)
            vector[i] = 1;
    }
    t1 = MPI_Wtime();
    broadcast(vector.data(), size, MPI_INT, root, MPI_COMM_WORLD);
    t2 = MPI_Wtime();
    if (rank == 0) {
        for (int i = 0; i < size; i++)
            sum += vector[i];
    }
    t3 = MPI_Wtime();
    MPI_Bcast(vector.data(), size, MPI_INT, root, MPI_COMM_WORLD);
    t4 = MPI_Wtime();
    if (rank == root) {
        // std::cout << "My Broadcast time: " << t2 - t1 <<
        //     '\n' << "MPI_Bcast time: " << t4 - t3 << '\n';
        ASSERT_EQ(sum, size);
    }
}

TEST(Broadcast, Test_sum) {
    int sizeWorld, rank, sum = 0,
        root = 0, size = 15;
    MPI_Comm_size(MPI_COMM_WORLD, &sizeWorld);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> vector(size);

    if (rank == root) {
        for (int i = 0; i < size; i++)
            vector[i] = 1;
    }
    broadcast(vector.data(), size, MPI_INT, root, MPI_COMM_WORLD);
    if (rank == 0) {
        for (int i = 0; i < size; i++)
            sum += vector[i];
    }
    if (rank == root)
        ASSERT_EQ(sum, size);
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
