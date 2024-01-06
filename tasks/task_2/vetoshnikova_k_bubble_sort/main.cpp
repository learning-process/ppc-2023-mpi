// Copyright 2023 Vetoshnikova Ekaterina


#include <gtest/gtest.h>
#include <mpi.h>
#include <iostream>
#include "./bubble_sort.h"

TEST(Parallel_Bubble_sort, Test_arr_1) {
    int ProcRank;
    int ProcNum;
    int count_size_vector = 7;

    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);

    int arr_original[] { 0, 6, 3, 8, 1, 5, 4 };
    int* arrPar = new int[count_size_vector];
    int* arrSeq = new int[count_size_vector];

    for (int i = 0; i < count_size_vector; i++) {
        arrPar[i] = arr_original[i];
        arrSeq[i] = arr_original[i];
    }
    ParallelBubbleSort(arrPar, count_size_vector);
    if (ProcRank == 0) {
        SequentialBubbleSort(arrSeq, count_size_vector);
        for (int i = 0; i < count_size_vector; i++) {
            EXPECT_EQ(arrPar[i], arrSeq[i]);
        }
    }
    delete[] arrPar;
    delete[] arrSeq;
}

TEST(Parallel_Bubble_sort, Test_random_arr_1) {
    int ProcRank;
    int ProcNum;
    int count_size_vector = 15;

    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);

    int* arr_original = array_generation(5, 100, count_size_vector);
    int* arrPar = new int[count_size_vector];
    int* arrSeq = new int[count_size_vector];

    for (int i = 0; i < count_size_vector; i++) {
        arrPar[i] = arr_original[i];
        arrSeq[i] = arr_original[i];
    }
    ParallelBubbleSort(arrPar, count_size_vector);
    if (ProcRank == 0) {
        SequentialBubbleSort(arrSeq, count_size_vector);
        for (int i = 0; i < count_size_vector; i++) {
            EXPECT_EQ(arrPar[i], arrSeq[i]);
        }
    }
    delete[] arrPar;
    delete[] arrSeq;
}

TEST(Parallel_Bubble_sort, Test_negative_elements) {
    int ProcRank;
    int ProcNum;
    int count_size_vector = 20;

    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);

    int* arr_original = array_generation(-100, 0, count_size_vector);
    int* arrPar = new int[count_size_vector];
    int* arrSeq = new int[count_size_vector];

    for (int i = 0; i < count_size_vector; i++) {
        arrPar[i] = arr_original[i];
        arrSeq[i] = arr_original[i];
    }
    ParallelBubbleSort(arrPar, count_size_vector);
    if (ProcRank == 0) {
        SequentialBubbleSort(arrSeq, count_size_vector);
        for (int i = 0; i < count_size_vector; i++) {
            EXPECT_EQ(arrPar[i], arrSeq[i]);
        }
    }
    delete[] arr_original;
    delete[] arrPar;
    delete[] arrSeq;
}

TEST(Parallel_Bubble_sort, Test_random_arr_2) {
    int ProcRank;
    int ProcNum;
    int count_size_vector = 12;

    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);

    int* arr_original = array_generation(-50, 50, count_size_vector);
    int* arrPar = new int[count_size_vector];
    int* arrSeq = new int[count_size_vector];

    for (int i = 0; i < count_size_vector; i++) {
        arrPar[i] = arr_original[i];
        arrSeq[i] = arr_original[i];
    }
    ParallelBubbleSort(arrPar, count_size_vector);
    if (ProcRank == 0) {
        SequentialBubbleSort(arrSeq, count_size_vector);
        for (int i = 0; i < count_size_vector; i++) {
            EXPECT_EQ(arrPar[i], arrSeq[i]);
        }
    }
    delete[] arr_original;
    delete[] arrPar;
    delete[] arrSeq;
}

TEST(Parallel_Bubble_sort, Test_arr_2) {
    int ProcRank;
    int ProcNum;
    int count_size_vector = 10;

    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);

    int arr_original[] { 20, 17, 14, 11, 8, 5, 2, -1, -4, -7 };
    int* arrPar = new int[count_size_vector];
    int* arrSeq = new int[count_size_vector];

    for (int i = 0; i < count_size_vector; i++) {
        arrPar[i] = arr_original[i];
        arrSeq[i] = arr_original[i];
    }
    ParallelBubbleSort(arrPar, count_size_vector);
    if (ProcRank == 0) {
        SequentialBubbleSort(arrSeq, count_size_vector);
        for (int i = 0; i < count_size_vector; i++) {
            EXPECT_EQ(arrPar[i], arrSeq[i]);
        }
    }
    delete[] arrPar;
    delete[] arrSeq;
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
