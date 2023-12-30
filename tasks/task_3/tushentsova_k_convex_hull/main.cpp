// Copyright 2023 Tushentsova Karina
#include <gtest/gtest.h>
#include <mpi.h>

#include "task_3/tushentsova_k_convex_hull/convex_hull.h"

TEST(BuildingConvexHull, Test_1) {
    std::vector<int> vec = getRandomVector();
    int colls = vec.back();
    int rows = vec.back();

    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (size == 1) return;
    if (size > rows) return;

    std::vector<int> vecP = parallBuildingConvexHull(vec, rows, colls);

    if (rank == 0) {
        std::vector<int> expVec = buildingConvexHull(vec, rows, colls);
        for (int i = 0; i < vecP.size(); i++) ASSERT_EQ(vecP[i], expVec[i]);
    }
    MPI_Barrier(MPI_COMM_WORLD);
}

TEST(BuildingConvexHull, Test_2) {
    std::vector<int> vec = getRandomVector();
    int colls = vec.back();
    int rows = vec.back();

    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (size == 1) return;
    if (size > rows) return;

    std::vector<int> vecP = parallBuildingConvexHull(vec, rows, colls);

    if (rank == 0) {
        std::vector<int> expVec = buildingConvexHull(vec, rows, colls);
        for (int i = 0; i < vecP.size(); i++) ASSERT_EQ(vecP[i], expVec[i]);
    }
    MPI_Barrier(MPI_COMM_WORLD);
}

TEST(BuildingConvexHull, Test_3) {
    std::vector<int> vec = getRandomVector();
    int colls = vec.back();
    int rows = vec.back();

    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (size == 1) return;
    if (size > rows) return;

    std::vector<int> vecP = parallBuildingConvexHull(vec, rows, colls);

    if (rank == 0) {
        std::vector<int> expVec = buildingConvexHull(vec, rows, colls);
        for (int i = 0; i < vecP.size(); i++) ASSERT_EQ(vecP[i], expVec[i]);
    }
    MPI_Barrier(MPI_COMM_WORLD);
}

TEST(BuildingConvexHull, Test_4) {
    std::vector<int> vec = getRandomVector();
    int colls = vec.back();
    int rows = vec.back();

    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (size == 1) return;
    if (size > rows) return;

    std::vector<int> vecP = parallBuildingConvexHull(vec, rows, colls);

    if (rank == 0) {
        std::vector<int> expVec = buildingConvexHull(vec, rows, colls);
        for (int i = 0; i < vecP.size(); i++) ASSERT_EQ(vecP[i], expVec[i]);
    }
    MPI_Barrier(MPI_COMM_WORLD);
}

TEST(BuildingConvexHull, Test_5) {
    std::vector<int> vec = getRandomVector();
    int colls = vec.back();
    int rows = vec.back();

    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (size == 1) return;
    if (size > rows) return;

    std::vector<int> vecP = parallBuildingConvexHull(vec, rows, colls);

    if (rank == 0) {
        std::vector<int> expVec = buildingConvexHull(vec, rows, colls);
        for (int i = 0; i < vecP.size(); i++) ASSERT_EQ(vecP[i], expVec[i]);
    }
    MPI_Barrier(MPI_COMM_WORLD);
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
