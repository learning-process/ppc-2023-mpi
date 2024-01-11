// Copyright 2023 Shemyakina Alesya
#include <gtest/gtest.h>
#include <boost/mpi/communicator.hpp>
#include <boost/mpi/collectives.hpp>
#include "task_3/shemyakina_a_strassen_algorithm/strassen_algorithm.h"

TEST(Strassen_Algorithm, Test1) {
    int size, rank;

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int sqrt_size = static_cast<int>(sqrt(static_cast<double>(size)));
    if (sqrt_size * sqrt_size == size) {
        int Size = 3;
        int sz = Size;
        int extraEls = Size % sqrt_size;
        if (extraEls > 0) Size += sqrt_size - (Size % sqrt_size);

        std::vector<double> Res(Size * Size, 0);
        std::vector<double> ResSeq(Size * Size, 0);
        std::vector<double> pAMatrix(Size * Size, 0);
        std::vector<double> pBMatrix(Size * Size, 0);

        if (rank == 0) {
            getRandMatrix(&pAMatrix, Size, sz);
            getRandMatrix(&pBMatrix, Size, sz);
        }

        Res = StrassenAlgorithm(pAMatrix, pBMatrix, Size);

        if (rank == 0) {
            ResSeq = sequentialMul(pAMatrix, pBMatrix, Size);
            bool check = MatrixEqual(Res, ResSeq, Size);
            ASSERT_EQ(check, true);
        }
    }
}

TEST(Strassen_Algorithm, Test2) {
    int size, rank;

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int sqrt_size = static_cast<int>(sqrt(static_cast<double>(size)));
    if (sqrt_size * sqrt_size == size) {
        int Size = 1;
        int sz = Size;
        int extraEls = Size % sqrt_size;
        if (extraEls > 0) Size += sqrt_size - (Size % sqrt_size);

        std::vector<double> Res(Size * Size, 0);
        std::vector<double> ResSeq(Size * Size, 0);
        std::vector<double> pAMatrix(Size * Size, 0);
        std::vector<double> pBMatrix(Size * Size, 0);

        if (rank == 0) {
            getRandMatrix(&pAMatrix, Size, sz);
            getRandMatrix(&pBMatrix, Size, sz);
        }

        Res = StrassenAlgorithm(pAMatrix, pBMatrix, Size);

        if (rank == 0) {
            ResSeq = sequentialMul(pAMatrix, pBMatrix, Size);
            bool check = MatrixEqual(Res, ResSeq, Size);
            ASSERT_EQ(check, true);
        }
    }
}

TEST(Strassen_Algorithm, Test3) {
    int size, rank;

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int sqrt_size = static_cast<int>(sqrt(static_cast<double>(size)));
    if (sqrt_size * sqrt_size == size) {
        int Size = 20;
        int sz = Size;
        int extraEls = Size % sqrt_size;
        if (extraEls > 0) Size += sqrt_size - (Size % sqrt_size);

        std::vector<double> Res(Size * Size, 0);
        std::vector<double> ResSeq(Size * Size, 0);
        std::vector<double> pAMatrix(Size * Size, 0);
        std::vector<double> pBMatrix(Size * Size, 0);

        Res = StrassenAlgorithm(pAMatrix, pBMatrix, Size);

        if (rank == 0) {
            ResSeq = sequentialMul(pAMatrix, pBMatrix, Size);
            bool check = MatrixEqual(Res, ResSeq, Size);
            ASSERT_EQ(check, true);
        }
    }
}

TEST(Strassen_Algorithm, Test4) {
    int size, rank;

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int sqrt_size = static_cast<int>(sqrt(static_cast<double>(size)));
    if (sqrt_size * sqrt_size == size) {
        int Size = 20;
        int sz = Size;
        int extraEls = Size % sqrt_size;
        if (extraEls > 0) Size += sqrt_size - (Size % sqrt_size);

        std::vector<double> Res(Size * Size, 0);
        std::vector<double> ResSeq(Size * Size, 0);
        std::vector<double> pAMatrix(Size * Size, 0);
        std::vector<double> pBMatrix(Size * Size, 0);

        if (rank == 0) {
            for (int i = 0; i < sz; i++)
                for (int j = 0; j < sz; j++) {
                    pAMatrix[i * Size + j] = 1;
                    pBMatrix[i * Size + j] = 1;
                }
        }

        Res = StrassenAlgorithm(pAMatrix, pBMatrix, Size);

        if (rank == 0) {
            ResSeq = sequentialMul(pAMatrix, pBMatrix, Size);
            bool check = MatrixEqual(Res, ResSeq, Size);
            ASSERT_EQ(check, true);
        }
    }
}

TEST(Strassen_Algorithm, Test5) {
    int size, rank;

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int sqrt_size = static_cast<int>(sqrt(static_cast<double>(size)));
    if (sqrt_size * sqrt_size == size) {
        int Size = 20;
        int sz = Size;
        int extraEls = Size % sqrt_size;
        if (extraEls > 0) Size += sqrt_size - (Size % sqrt_size);

        std::vector<double> Res(Size * Size, 0);
        std::vector<double> ResSeq(Size * Size, 0);
        std::vector<double> pAMatrix(Size * Size, 0);
        std::vector<double> pBMatrix(Size * Size, 0);

        if (rank == 0) {
            getRandMatrix(&pAMatrix, Size, sz);
            getRandMatrix(&pBMatrix, Size, sz);
            for (int i = 0; i < Size; i++)
                for (int j = 0; j < Size; j++) {
                    pAMatrix[i * sz + j] /= static_cast <double>(10);
                    pBMatrix[i * sz + j] /= static_cast <double>(10);
                }
        }

        Res = StrassenAlgorithm(pAMatrix, pBMatrix, Size);

        if (rank == 0) {
            ResSeq = sequentialMul(pAMatrix, pBMatrix, Size);
            bool check = MatrixEqual(Res, ResSeq, Size);
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
