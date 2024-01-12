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

    int n = 6;
    int m = 6;
    int ref_comp = 1;

    image image(n, m);
    image.data[3][3] = 1;

    ParallelMarkingÑomponent(&image);

    if (ProcRank == 0) {
        ASSERT_EQ(image.count, ref_comp);
    }
}

TEST(component_marking, Test_2) {
    int ProcNum;
    int ProcRank;

    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

    int n = 5;
    int m = 5;
    int ref_comp = 2;

    image image(n, m);

    image.data[1][1] = 1;
    image.data[2][1] = 1;
    image.data[2][3] = 1;
    image.data[3][3] = 1;

    ParallelMarkingÑomponent(&image);

    if (ProcRank == 0) {
        ASSERT_EQ(ref_comp, 2);
    }
}

TEST(component_marking, Test_3) {
    int ProcNum;
    int ProcRank;

    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

    int n = 10;
    int m = 10;
    int ref_comp = 3;

    image image(n, m);

    image.data[2][2] = 1;
    image.data[4][4] = 1;
    image.data[6][6] = 1;

    ParallelMarkingÑomponent(&image);

    if (ProcRank == 0) {
        ASSERT_EQ(image.count, ref_comp);
    }
}

TEST(component_marking, Test_4) {
    int ProcNum;
    int ProcRank;
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    int n = 8;
    int m = 8;
    int ref_comp = 4;
    image image(n, m);
    image.data[1][1] = 1;
    image.data[1][2] = 1;
    image.data[1][6] = 1;
    image.data[2][1] = 1;
    image.data[2][2] = 1;
    image.data[2][6] = 1;
    image.data[3][6] = 1;
    image.data[4][1] = 1;
    image.data[4][2] = 1;
    image.data[4][3] = 1;
    image.data[6][5] = 1;
    image.data[6][6] = 1;
    ParallelMarkingÑomponent(&image);
    if (ProcRank == 0) {
        ASSERT_EQ(ref_comp, 4);
    }
}

TEST(component_marking, Test_5) {
    int ProcNum;
    int ProcRank;
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    int n = 4;
    int m = 4;
    int ref_comp = 0;
    image image(n, m);
    ParallelMarkingÑomponent(&image);
    if (ProcRank == 0) {
        ASSERT_EQ(image.count, ref_comp);
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
