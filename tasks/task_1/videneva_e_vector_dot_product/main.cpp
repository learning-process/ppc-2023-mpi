// Copyright 2023 Videneva Ekaterina

#include <gtest/gtest.h>
#include <mpi.h>
#include <iostream>
#include "./vector_dot_product.h"


TEST(DotProductTest, TestVectorDifferentSize) {
    int world_size = 0;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    int rank = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<int> vec1, vec2;
    vec1 = {1, 2, 3, 4, 5};
    vec2 = {5, 4, 3, 2, 1};

    int global_product = Parall_product(vec1, vec2);
    

    if (rank == 0) {
        int reference_product = scalar_product(vec1, vec2);
        ASSERT_EQ(reference_product, global_product);
    }
}



TEST(DotProductTest, TestVectorEmpty) {
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> vec1, vec2;

    vec1 = {};
    vec2 = {};
    int global_product = Parall_product(vec1, vec2);

    if (rank == 0) {
        int reference_product = scalar_product(vec1, vec2);
        ASSERT_EQ(reference_product, global_product);
    }
}



TEST(DotProductTest, TestZeroVector) {
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> vec1, vec2;
    vec1 = {0, 0, 0, 0, 0};
    vec2 = {0, 0, 0, 0, 0};


    int global_product = Parall_product(vec1, vec2);

    if (rank == 0) {
        int reference_product = scalar_product(vec1, vec2);
        ASSERT_EQ(reference_product, global_product);
    }
}

TEST(DotProductTest, HalfEmpty) {
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> vec1, vec2;
    const int size_vec = 0;

    vec1 = {1, 2, 3, 4, 5};
    vec2 = {};

    int global_product = Parall_product(vec1, vec2);

    if (rank == 0) {
        int reference_product = scalar_product(vec1, vec2);
        ASSERT_EQ(reference_product, global_product);
    }
}

TEST(DotProductTest, NoEqualyVector) {
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> vec1 = {1, 2, 3, 4, 5, 6, 7, 8};
    std::vector<int> vec2 = {1, 2, 3, 4, 5, 6};
    int global_product = Parall_product(vec1, vec2);

    if (rank == 0) {
        int reference_product = scalar_product(vec1, vec2);
        ASSERT_EQ(reference_product, global_product);
    }
}

int main(int argc, char **argv) {
    int result_code = 0;

    ::testing::InitGoogleTest(&argc, argv);
    ::testing::TestEventListeners &listeners =
        ::testing::UnitTest::GetInstance()->listeners();

    if (MPI_Init(&argc, &argv) != MPI_SUCCESS)
        MPI_Abort(MPI_COMM_WORLD, -1);
    result_code = RUN_ALL_TESTS();
    MPI_Finalize();

    return result_code;
}
