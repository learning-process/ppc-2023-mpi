// Copyright 2023 Novostroev Ivan
#include <gtest/gtest.h>
#include <boost/mpi/communicator.hpp>
#include <boost/mpi/collectives.hpp>
#include "task_3/novostroev_i_matrix_mult_strassen/strassen.h"

TEST(StrassenAlg, Test1) {
    int worldSize;
    MPI_Comm_size(MPI_COMM_WORLD, &worldSize);
    int worldRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &worldRank);
    int sqrtSize = static_cast<int>(sqrt(static_cast<double>(worldSize)));
    if (sqrtSize * sqrtSize == worldSize) {
        int Size = 3;
        int sz = Size;
        int extraEls = Size % sqrtSize;
        if (extraEls > 0) Size += sqrtSize - (Size % sqrtSize);

        std::vector<double> Res(Size * Size, 0);
        std::vector<double> ResSeq(Size * Size, 0);
        std::vector<double> pAMatrix(Size * Size, 0);
        std::vector<double> pBMatrix(Size * Size, 0);

        if (worldRank == 0) {
            getRandMatrix(&pAMatrix, Size, sz);
            getRandMatrix(&pBMatrix, Size, sz);
        }

        Res = strassenAlgorithm(pAMatrix, pBMatrix, Size);

        if (worldRank == 0) {
            ResSeq = sequentialMul(pAMatrix, pBMatrix, Size);
            bool check = isMatrEqual(Res, ResSeq, Size);
            ASSERT_EQ(check, true);
        }
    }
}

TEST(StrassenAlg, Test2) {
    int worldSize;
    MPI_Comm_size(MPI_COMM_WORLD, &worldSize);
    int worldRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &worldRank);
    int sqrtSize = static_cast<int>(sqrt(static_cast<double>(worldSize)));
    if (sqrtSize * sqrtSize == worldSize) {
        int Size = 1;
        int sz = Size;
        int extraEls = Size % sqrtSize;
        if (extraEls > 0) Size += sqrtSize - (Size % sqrtSize);

        std::vector<double> Res(Size * Size, 0);
        std::vector<double> ResSeq(Size * Size, 0);
        std::vector<double> pAMatrix(Size * Size, 0);
        std::vector<double> pBMatrix(Size * Size, 0);

        if (worldRank == 0) {
            getRandMatrix(&pAMatrix, Size, sz);
            getRandMatrix(&pBMatrix, Size, sz);
        }

        Res = strassenAlgorithm(pAMatrix, pBMatrix, Size);

        if (worldRank == 0) {
            ResSeq = sequentialMul(pAMatrix, pBMatrix, Size);
            bool check = isMatrEqual(Res, ResSeq, Size);
            ASSERT_EQ(check, true);
        }
    }
}

TEST(StrassenAlg, Test3) {
    int worldSize;
    MPI_Comm_size(MPI_COMM_WORLD, &worldSize);
    int worldRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &worldRank);
    int sqrtSize = static_cast<int>(sqrt(static_cast<double>(worldSize)));
    if (sqrtSize * sqrtSize == worldSize) {
        int Size = 20;
        int sz = Size;
        int extraEls = Size % sqrtSize;
        if (extraEls > 0) Size += sqrtSize - (Size % sqrtSize);

        std::vector<double> Res(Size * Size, 0);
        std::vector<double> ResSeq(Size * Size, 0);
        std::vector<double> pAMatrix(Size * Size, 0);
        std::vector<double> pBMatrix(Size * Size, 0);

        Res = strassenAlgorithm(pAMatrix, pBMatrix, Size);

        if (worldRank == 0) {
            ResSeq = sequentialMul(pAMatrix, pBMatrix, Size);
            bool check = isMatrEqual(Res, ResSeq, Size);
            ASSERT_EQ(check, true);
        }
    }
}

TEST(StrassenAlg, Test4) {
    int worldSize;
    MPI_Comm_size(MPI_COMM_WORLD, &worldSize);
    int worldRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &worldRank);
    int sqrtSize = static_cast<int>(sqrt(static_cast<double>(worldSize)));
    if (sqrtSize * sqrtSize == worldSize) {
        int Size = 20;
        int sz = Size;
        int extraEls = Size % sqrtSize;
        if (extraEls > 0) Size += sqrtSize - (Size % sqrtSize);

        std::vector<double> Res(Size * Size, 0);
        std::vector<double> ResSeq(Size * Size, 0);
        std::vector<double> pAMatrix(Size * Size, 0);
        std::vector<double> pBMatrix(Size * Size, 0);

        if (worldRank == 0) {
            for (int i = 0; i < sz; i++)
                for (int j = 0; j < sz; j++) {
                    pAMatrix[i * Size + j] = 1;
                    pBMatrix[i * Size + j] = 1;
                }
        }

        Res = strassenAlgorithm(pAMatrix, pBMatrix, Size);

        if (worldRank == 0) {
            ResSeq = sequentialMul(pAMatrix, pBMatrix, Size);
            bool check = isMatrEqual(Res, ResSeq, Size);
            ASSERT_EQ(check, true);
        }
    }
}

TEST(StrassenAlg, Test5) {
    int worldSize;
    MPI_Comm_size(MPI_COMM_WORLD, &worldSize);
    int worldRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &worldRank);
    int sqrtSize = static_cast<int>(sqrt(static_cast<double>(worldSize)));
    if (sqrtSize * sqrtSize == worldSize) {
        int Size = 20;
        int sz = Size;
        int extraEls = Size % sqrtSize;
        if (extraEls > 0) Size += sqrtSize - (Size % sqrtSize);

        std::vector<double> Res(Size * Size, 0);
        std::vector<double> ResSeq(Size * Size, 0);
        std::vector<double> pAMatrix(Size * Size, 0);
        std::vector<double> pBMatrix(Size * Size, 0);

        if (worldRank == 0) {
            getRandMatrix(&pAMatrix, Size, sz);
            getRandMatrix(&pBMatrix, Size, sz);
            for (int i = 0; i < Size; i++)
                for (int j = 0; j < Size; j++) {
                    pAMatrix[i * sz + j] /= static_cast <double>(10);
                    pBMatrix[i * sz + j] /= static_cast <double>(10);
                }
        }

        Res = strassenAlgorithm(pAMatrix, pBMatrix, Size);

        if (worldRank == 0) {
            ResSeq = sequentialMul(pAMatrix, pBMatrix, Size);
            bool check = isMatrEqual(Res, ResSeq, Size);
            ASSERT_EQ(check, true);
        }
    }
}

int main(int argc, char** argv) {
    boost::mpi::environment env(argc, argv);
    boost::mpi::communicator communicator;
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::TestEventListeners& listeners =
        ::testing::UnitTest::GetInstance()->listeners();
    if (communicator.rank()) {
        delete listeners.Release(listeners.default_result_printer());
    }
    return RUN_ALL_TESTS();
}
