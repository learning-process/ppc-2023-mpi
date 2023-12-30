// Copyright 2023 Bakhtiarov Alexander
#include <gtest/gtest.h>
#include <mpi.h>
#include <iostream>
#include <vector>
#include "task_2/bakhtiarov_a_broadcast/broadcast.h"


TEST(broadcast, testInt) {
    int rank, size, root = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int count = 10;
    int* a = new int[count];

    if (rank == root) {
        for (int i = 0; i < count; i++) {
            a[i] = 1;
        }
    } else {
        for (int i = 0; i < count; i++) {
            a[i] = -1;
        }
    }

    improved_broadcast(a, count, MPI_INT, root, MPI_COMM_WORLD);

    for (int i = 0; i < count; i++) {
        EXPECT_EQ(a[i], 1);
    }
}

TEST(broadcast, testDouble) {
    int rank, size, root = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int count = 10;
    double* a = new double[count];

    if (rank == root) {
        for (int i = 0; i < count; i++) {
            a[i] = 1.5;
        }
    } else {
        for (int i = 0; i < count; i++) {
            a[i] = -1.0;
        }
    }

    improved_broadcast(a, count, MPI_DOUBLE, root, MPI_COMM_WORLD);

    for (int i = 0; i < count; i++) {
        EXPECT_DOUBLE_EQ(a[i], 1.5);
    }
}

TEST(broadcast, testFloat) {
    int rank, size, root = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int count = 10;
    float* a = new float[count];

    if (rank == root) {
        for (int i = 0; i < count; i++) {
            a[i] = 1.5f;
        }
    } else {
        for (int i = 0; i < count; i++) {
            a[i] = -1.0f;
        }
    }

    improved_broadcast(a, count, MPI_FLOAT, root, MPI_COMM_WORLD);

    for (int i = 0; i < count; i++) {
        EXPECT_FLOAT_EQ(a[i], 1.5f);
    }
}

TEST(broadcast, testIntVec) {
    int rank, size, root = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    std::vector<int> vector = {1, 2, 3, 4, 5};
    int count = vector.size();

    std::vector<int> localVector;
    if (rank == root) {
        localVector = std::vector<int>(vector.begin(), vector.end());
    } else {
        localVector.resize(count);
    }

    improved_broadcast(localVector.data(), count,
        MPI_INT, root, MPI_COMM_WORLD);

    for (int i = 0; i < count; i++) {
        EXPECT_EQ(localVector[i], vector[i]);
    }
}

TEST(broadcast, testDoubleVec) {
    int rank, size, root = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    std::vector<double> vector = {1.1, 2.2, 3.3, 4.4, 5.5};
    int count = vector.size();

    std::vector<double> localVector;
    if (rank == root) {
        localVector = std::vector<double>(vector.begin(), vector.end());
    } else {
        localVector.resize(count);
    }

    improved_broadcast(localVector.data(), count,
        MPI_DOUBLE, root, MPI_COMM_WORLD);

    for (int i = 0; i < count; i++) {
        EXPECT_DOUBLE_EQ(localVector[i], vector[i]);
    }
}

TEST(broadcast, testFloatVec) {
    int rank, size, root = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    std::vector<float> vector = {1.1f, 2.2f, 3.3f, 4.4f, 5.5f};
    int count = vector.size();

    std::vector<float> localVector;
    if (rank == root) {
        localVector = std::vector<float>(vector.begin(), vector.end());
    } else {
        localVector.resize(count);
    }

    improved_broadcast(localVector.data(), count,
        MPI_FLOAT, root, MPI_COMM_WORLD);

    for (int i = 0; i < count; i++) {
        EXPECT_FLOAT_EQ(localVector[i], vector[i]);
    }
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    int result = 0;
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::TestEventListeners& listeners =
        ::testing::UnitTest::GetInstance()->listeners();
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank != 0) {
        delete listeners.Release(listeners.default_result_printer());
    }
    result = RUN_ALL_TESTS();
    MPI_Finalize();

    return result;
}
