// Copyright 2023 Vetoshnikova Ekaterina


#include <gtest/gtest.h>
#include <mpi.h>
#include <iostream>
#include "./component_marking.h"

TEST(component_marking, Test_1) {
    int ProcNum;
    int ProcRank;
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    int ref_comp = 1;
    image img(6, 6);
    img.data[3][3] = 1;
    ParallelMarkingComponent(&img);
    if (ProcRank == 0) {
        ASSERT_EQ(ref_comp, 1);
    }
}

TEST(component_marking, Test_2) {
    int ProcNum;
    int ProcRank;
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    int ref_comp = 2;
    image img(5, 5);
    img.data[1][1] = 1;
    img.data[2][1] = 1;
    img.data[2][3] = 1;
    img.data[3][3] = 1;
    ParallelMarkingComponent(&img);
    if (ProcRank == 0) {
        ASSERT_EQ(ref_comp, 2);
    }
}

TEST(component_marking, Test_3) {
    int ProcNum;
    int ProcRank;

    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

    int ref_comp = 3;

    image img(10, 10);

    img.data[2][2] = 1;
    img.data[4][4] = 1;
    img.data[6][6] = 1;

    ParallelMarkingComponent(&img);

    if (ProcRank == 0) {
        ASSERT_EQ(ref_comp, 3);
    }
}

TEST(component_marking, Test_4) {
    int ProcNum;
    int ProcRank;
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    int ref_comp = 4;
    image img(8, 8);
    img.data[1][1] = 1;
    img.data[1][2] = 1;
    img.data[1][6] = 1;
    img.data[2][1] = 1;
    img.data[2][2] = 1;
    img.data[2][6] = 1;
    img.data[3][6] = 1;
    img.data[4][1] = 1;
    img.data[4][2] = 1;
    img.data[4][3] = 1;
    img.data[6][5] = 1;
    img.data[6][6] = 1;
    ParallelMarkingComponent(&img);
    if (ProcRank == 0) {
        ASSERT_EQ(ref_comp, 4);
    }
}

TEST(component_marking, Test_5) {
    int ProcNum;
    int ProcRank;
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    int ref_comp = 0;
    image img(4, 4);
    ParallelMarkingComponent(&img);
    if (ProcRank == 0) {
        ASSERT_EQ(ref_comp, 0);
    }
}

int main(int argc, char** argv) {
    int result = 0;

    ::testing::InitGoogleTest(&argc, argv);
    ::testing::TestEventListeners& listeners =
        ::testing::UnitTest::GetInstance()->listeners();

    if (MPI_Init(&argc, &argv) != MPI_SUCCESS)
        MPI_Abort(MPI_COMM_WORLD, -1);
    result = RUN_ALL_TESTS();
    MPI_Finalize();

    return result;
}
