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
    int sqrtsize = static_cast<int>(sqrt(static_cast<double>(world_size)));
    if (sqrtsize * sqrtsize == world_size) {
        int Size = 3;
        int sz = Size;
        int extra_els = Size % sqrtsize;
        if (extra_els > 0) Size += sqrtsize - (Size % sqrtsize);

        /*
        In case Size % sqrtsize != 0, the matrix is padded with zero values, which do not affect the desired value.
        Example:

        pAMatrix
        -13.9  -28.9    3    0
          9.6    9.8  -21.8  0
         -2.5    7    -13.6  0
          0      0      0    0

        pBMatrix
        -27.9   -4.7  -15.4  0
        -10.9  -25.2   -8.4  0
          5.2   -5.3  -10.1  0
          0      0      0    0

        Res
         718.42   777.71  426.52  0
        -488.02  -176.54   -9.98  0
         -77.27   -92.57  117.06  0
           0        0       0     0

        ResSeq
         718.42   777.71  426.52  0
        -488.02  -176.54   -9.98  0
         -77.27   -92.57  117.06  0
           0        0       0     0
        0 0 0 0
        */

        std::vector<double> Res(Size * Size, 0);
        std::vector<double> ResSeq(Size * Size, 0);
        std::vector<double> pAMatrix(Size * Size, 0);
        std::vector<double> pBMatrix(Size * Size, 0);

        if (world_rank == 0) {
            getRandMatrix(&pAMatrix, Size, sz);
            getRandMatrix(&pBMatrix, Size, sz);
        }

        Res = Fox_algorithm(pAMatrix, pBMatrix, Size);

        if (world_rank == 0) {
            ResSeq = SequentialMul(pAMatrix, pBMatrix, Size);
            bool check = isMatrEqual(Res, ResSeq, Size);
            ASSERT_EQ(check, true);
        }
    }
}

TEST(Fox_Algorithm_MPI, Size_of_matrix_is_1) {
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    int sqrtsize = static_cast<int>(sqrt(static_cast<double>(world_size)));
    if (sqrtsize * sqrtsize == world_size) {
        int Size = 1;
        int sz = Size;
        int extra_els = Size % sqrtsize;
        if (extra_els > 0) Size += sqrtsize - (Size % sqrtsize);

        std::vector<double> Res(Size * Size, 0);
        std::vector<double> ResSeq(Size * Size, 0);
        std::vector<double> pAMatrix(Size * Size, 0);
        std::vector<double> pBMatrix(Size * Size, 0);

        if (world_rank == 0) {
            getRandMatrix(&pAMatrix, Size, sz);
            getRandMatrix(&pBMatrix, Size, sz);
        }

        Res = Fox_algorithm(pAMatrix, pBMatrix, Size);

        if (world_rank == 0) {
            ResSeq = SequentialMul(pAMatrix, pBMatrix, Size);
            bool check = isMatrEqual(Res, ResSeq, Size);
            ASSERT_EQ(check, true);
        }
    }
}

TEST(Fox_Algorithm_MPI, Only_zeroes) {
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    int sqrtsize = static_cast<int>(sqrt(static_cast<double>(world_size)));
    if (sqrtsize * sqrtsize == world_size) {
        int Size = 20;
        int sz = Size;
        int extra_els = Size % sqrtsize;
        if (extra_els > 0) Size += sqrtsize - (Size % sqrtsize);

        std::vector<double> Res(Size * Size, 0);
        std::vector<double> ResSeq(Size * Size, 0);
        std::vector<double> pAMatrix(Size * Size, 0);
        std::vector<double> pBMatrix(Size * Size, 0);

        Res = Fox_algorithm(pAMatrix, pBMatrix, Size);

        if (world_rank == 0) {
            ResSeq = SequentialMul(pAMatrix, pBMatrix, Size);
            bool check = isMatrEqual(Res, ResSeq, Size);
            ASSERT_EQ(check, true);
        }
    }
}

TEST(Fox_Algorithm_MPI, Only_ones) {
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    int sqrtsize = static_cast<int>(sqrt(static_cast<double>(world_size)));
    if (sqrtsize * sqrtsize == world_size) {
        int Size = 20;
        int sz = Size;
        int extra_els = Size % sqrtsize;
        if (extra_els > 0) Size += sqrtsize - (Size % sqrtsize);

        std::vector<double> Res(Size * Size, 0);
        std::vector<double> ResSeq(Size * Size, 0);
        std::vector<double> pAMatrix(Size * Size, 0);
        std::vector<double> pBMatrix(Size * Size, 0);

        if (world_rank == 0) {
            for (int i = 0; i < sz; i++)
                for (int j = 0; j < sz; j++) {
                    pAMatrix[i * Size + j] = 1;
                    pBMatrix[i * Size + j] = 1;
                }
        }

        Res = Fox_algorithm(pAMatrix, pBMatrix, Size);

        if (world_rank == 0) {
            ResSeq = SequentialMul(pAMatrix, pBMatrix, Size);
            bool check = isMatrEqual(Res, ResSeq, Size);
            ASSERT_EQ(check, true);
        }
    }
}

TEST(Fox_Algorithm_MPI, All_elements_less_than_one) {
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    int sqrtsize = static_cast<int>(sqrt(static_cast<double>(world_size)));
    if (sqrtsize * sqrtsize == world_size) {
        int Size = 20;
        int sz = Size;
        int extra_els = Size % sqrtsize;
        if (extra_els > 0) Size += sqrtsize - (Size % sqrtsize);

        std::vector<double> Res(Size * Size, 0);
        std::vector<double> ResSeq(Size * Size, 0);
        std::vector<double> pAMatrix(Size * Size, 0);
        std::vector<double> pBMatrix(Size * Size, 0);

        if (world_rank == 0) {
            getRandMatrix(&pAMatrix, Size, sz);
            getRandMatrix(&pBMatrix, Size, sz);
            for (int i = 0; i < Size; i++)
                for (int j = 0; j < Size; j++) {
                    pAMatrix[i * sz + j] /= static_cast <double>(10);
                    pBMatrix[i * sz + j] /= static_cast <double>(10);
                }
        }

        Res = Fox_algorithm(pAMatrix, pBMatrix, Size);

        if (world_rank == 0) {
            ResSeq = SequentialMul(pAMatrix, pBMatrix, Size);
            bool check = isMatrEqual(Res, ResSeq, Size);
            ASSERT_EQ(check, true);
        }
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
