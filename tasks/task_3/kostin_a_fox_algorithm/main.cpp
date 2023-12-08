// Copyright 2023 Kostin Artem
#include <gtest/gtest.h>
#include <mpi.h>
#include <vector>
#include "./fox_algorithm.h"
#include <boost/mpi/environment.hpp>
#include <boost/mpi/communicator.hpp>

TEST(Fox_Algorithm_MPI, Test1) {
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    int Size = 3;
    double * Res;
    matrCalloc(&Res, Size);
    double * ResSeq;
    matrCalloc(&ResSeq, Size);
    double * pAMatrix;
    matrCalloc(&pAMatrix, Size);
    double * pBMatrix;
    matrCalloc(&pBMatrix, Size);

    if (world_rank == 0) {
        getRandMatrix(pAMatrix, Size);
        getRandMatrix(pBMatrix, Size);
    }

    Res = Fox_algorithm(world_rank, world_size, pAMatrix, pBMatrix, Size);

    if (world_rank == 0) {
        ResSeq = SequentialMul(pAMatrix, pBMatrix, Size);
        bool check = isMatrEqual(Res, ResSeq, Size);
        ASSERT_EQ(check, true);
    }
}

TEST(Fox_Algorithm_MPI, Size_of_matrix_is_1) {
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    int Size = 1;
    double* Res;
    matrCalloc(&Res, Size);
    double* ResSeq;
    matrCalloc(&ResSeq, Size);
    double* pAMatrix;
    matrCalloc(&pAMatrix, Size);
    double* pBMatrix;
    matrCalloc(&pBMatrix, Size);

    if (world_rank == 0) {
        getRandMatrix(pAMatrix, Size);
        getRandMatrix(pBMatrix, Size);
    }

    Res = Fox_algorithm(world_rank, world_size, pAMatrix, pBMatrix, Size);

    if (world_rank == 0) {
        ResSeq = SequentialMul(pAMatrix, pBMatrix, Size);
        bool check = isMatrEqual(Res, ResSeq, Size);
        ASSERT_EQ(check, true);
    }
}

TEST(Fox_Algorithm_MPI, Only_zeroes) {
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    int Size = 20;
    double* Res;
    matrCalloc(&Res, Size);
    double* ResSeq;
    matrCalloc(&ResSeq, Size);
    double* pAMatrix;
    matrCalloc(&pAMatrix, Size);
    double* pBMatrix;
    matrCalloc(&pBMatrix, Size);

    Res = Fox_algorithm(world_rank, world_size, pAMatrix, pBMatrix, Size);

    if (world_rank == 0) {
        ResSeq = SequentialMul(pAMatrix, pBMatrix, Size);
        bool check = isMatrEqual(Res, ResSeq, Size);
        ASSERT_EQ(check, true);
    }
}

TEST(Fox_Algorithm_MPI, Only_ones) {
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    int Size = 20;
    double* Res;
    matrCalloc(&Res, Size);
    double* ResSeq;
    matrCalloc(&ResSeq, Size);
    double* pAMatrix;
    matrCalloc(&pAMatrix, Size);
    double* pBMatrix;
    matrCalloc(&pBMatrix, Size);

    if (world_rank == 0) {
        for (int i = 0; i < Size; i++)
            for (int j = 0; j < Size; j++) {
                pAMatrix[i * Size + j] = 1;
                pBMatrix[i * Size + j] = 1;
            }
    }

    Res = Fox_algorithm(world_rank, world_size, pAMatrix, pBMatrix, Size);

    if (world_rank == 0) {
        ResSeq = SequentialMul(pAMatrix, pBMatrix, Size);
        bool check = isMatrEqual(Res, ResSeq, Size);
        ASSERT_EQ(check, true);
    }
}

TEST(Fox_Algorithm_MPI, All_elements_less_than_one) {
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    int Size = 20;
    double* Res;
    matrCalloc(&Res, Size);
    double* ResSeq;
    matrCalloc(&ResSeq, Size);
    double* pAMatrix;
    matrCalloc(&pAMatrix, Size);
    double* pBMatrix;
    matrCalloc(&pBMatrix, Size);

    if (world_rank == 0) {
        getRandMatrix(pAMatrix, Size);
        getRandMatrix(pBMatrix, Size);
        for (int i = 0; i < Size; i++)
            for (int j = 0; j < Size; j++) {
                pAMatrix[i * Size + j] /= static_cast < double>(10);
                pBMatrix[i * Size + j] /= static_cast < double>(10);
            }
    }

    Res = Fox_algorithm(world_rank, world_size, pAMatrix, pBMatrix, Size);

    if (world_rank == 0) {
        ResSeq = SequentialMul(pAMatrix, pBMatrix, Size);
        bool check = isMatrEqual(Res, ResSeq, Size);
        ASSERT_EQ(check, true);
    }
}



int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::TestEventListeners& listeners = ::testing::UnitTest::GetInstance()->listeners();
    if (MPI_Init(&argc, &argv) != MPI_SUCCESS)
        MPI_Abort(MPI_COMM_WORLD, -1);
    int rcode = RUN_ALL_TESTS();
    MPI_Finalize();
    return rcode;
}
