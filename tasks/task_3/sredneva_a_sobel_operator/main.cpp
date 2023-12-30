// Copyright 2023 Sredneva Anastasiya
#include <gtest/gtest.h>
#include "./sobel_operator.h"

TEST(TEST_MPI, Test_rand) {
    int ProcRank;
    int n = 5;
    int m = 5;
    uint8_t l = 0;
    uint8_t r = 255;
    std::vector<uint8_t> pict;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    if (ProcRank == 0) {
        pict = getRandomPicture(n, m, l, r);
    }
    std::vector<uint8_t> ParallelSobel = getParallelSobel(pict, n, m);
    if (ProcRank == 0) {
        std::vector<uint8_t> SequentialSobel = getSequentialSobel(pict, n, m);
        ASSERT_EQ(SequentialSobel, ParallelSobel);
    }
}

TEST(TEST_MPI, Test_all_null) {
    int ProcRank;
    int n = 4;
    int m = 3;
    std::vector<uint8_t> pict;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    if (ProcRank == 0) {
        pict = { 0, 0, 0, 0, 0, 0,
                 0, 0, 0, 0, 0, 0,
                 0, 0, 0, 0, 0, 0,
                 0, 0, 0, 0, 0, 0,
                 0, 0, 0, 0, 0, 0 };
    }
    std::vector<uint8_t> ParallelSobel = getParallelSobel(pict, n, m);
    if (ProcRank == 0) {
        std::vector<uint8_t> SequentialSobel = getSequentialSobel(pict, n, m);
        ASSERT_EQ(SequentialSobel, ParallelSobel);
    }
}

TEST(TEST_MPI, Test_all_255) {
    int ProcRank;
    int n = 4;
    int m = 3;
    std::vector<uint8_t> pict;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    if (ProcRank == 0) {
        pict = { 0,   0,   0,   0,   0, 0,
                 0, 255, 255, 255, 255, 0,
                 0, 255, 255, 255, 255, 0,
                 0, 255, 255, 255, 255, 0,
                 0,   0,   0,   0,   0, 0 };
    }
    std::vector<uint8_t> ParallelSobel = getParallelSobel(pict, n, m);
    if (ProcRank == 0) {
        std::vector<uint8_t> SequentialSobel = getSequentialSobel(pict, n, m);
        ASSERT_EQ(SequentialSobel, ParallelSobel);
    }
}

TEST(TEST_MPI, Test_rand2) {
    int ProcRank;
    int n = 10;
    int m = 21;
    uint8_t l = 0;
    uint8_t r = 255;
    std::vector<uint8_t> pict;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    if (ProcRank == 0) {
        pict = getRandomPicture(n, m, l, r);
    }
    std::vector<uint8_t> ParallelSobel = getParallelSobel(pict, n, m);
    if (ProcRank == 0) {
        std::vector<uint8_t> SequentialSobel = getSequentialSobel(pict, n, m);
        ASSERT_EQ(SequentialSobel, ParallelSobel);
    }
}

TEST(TEST_MPI, Test_rand3) {
    int ProcRank;
    int n = 17;
    int m = 7;
    uint8_t l = 0;
    uint8_t r = 255;
    std::vector<uint8_t> pict;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    if (ProcRank == 0) {
        pict = getRandomPicture(n, m, l, r);
    }
    std::vector<uint8_t> ParallelSobel = getParallelSobel(pict, n, m);
    if (ProcRank == 0) {
        std::vector<uint8_t> SequentialSobel = getSequentialSobel(pict, n, m);
        ASSERT_EQ(SequentialSobel, ParallelSobel);
    }
}

int main(int argc, char** argv) {
    int result = 0;

    ::testing::InitGoogleTest(&argc, argv);
    ::testing::TestEventListeners& listeners = ::testing::UnitTest::GetInstance()->listeners();

    MPI_Init(&argc, &argv);
    int ProcRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    if (ProcRank != 0) {
        delete listeners.Release(listeners.default_result_printer());
    }
    result = RUN_ALL_TESTS();
    MPI_Finalize();

    return result;
}
