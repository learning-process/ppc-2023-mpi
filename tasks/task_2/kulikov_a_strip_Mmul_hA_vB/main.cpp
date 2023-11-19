// Copyright 2023 Kulikov Artem
#include <gtest/gtest.h>

#include <boost/mpi/communicator.hpp>
#include <boost/mpi/environment.hpp>
#include <boost/serialization/vector.hpp>

#include "./strip_Mmul_hA_vB.h"

TEST(Parallel_Matrix_Multiplication_MPI, Test_Random_N4) {
    boost::mpi::communicator world;
    std::vector<int> A, B, C;
    const int n = 4;

    if (world.rank() == 0) {
        A = getRandomMatrix(n);
        B = getRandomMatrix(n);
    }

    C = getParallelMatrMul(A, B, n);

    if (world.rank() == 0) {
        std::vector<int> reference_C = getSequentialMatrMul(A, B, n);
        for (int i = 0; i < reference_C.size(); ++i)
            ASSERT_EQ(reference_C[i], C[i]);
    }
}

TEST(Parallel_Matrix_Multiplication_MPI, Test_Random_N8) {
    boost::mpi::communicator world;
    std::vector<int> A, B, C;
    const int n = 8;

    if (world.rank() == 0) {
        A = getRandomMatrix(n);
        B = getRandomMatrix(n);
    }

    C = getParallelMatrMul(A, B, n);

    if (world.rank() == 0) {
        std::vector<int> reference_C = getSequentialMatrMul(A, B, n);
        for (int i = 0; i < reference_C.size(); ++i)
            ASSERT_EQ(reference_C[i], C[i]);
    }
}

TEST(Parallel_Matrix_Multiplication_MPI, Test_Random_N16) {
    boost::mpi::communicator world;
    std::vector<int> A, B, C;
    const int n = 8;

    if (world.rank() == 0) {
        A = getRandomMatrix(n);
        B = getRandomMatrix(n);
    }

    C = getParallelMatrMul(A, B, n);

    if (world.rank() == 0) {
        std::vector<int> reference_C = getSequentialMatrMul(A, B, n);
        for (int i = 0; i < reference_C.size(); ++i)
            ASSERT_EQ(reference_C[i], C[i]);
    }
}

TEST(Parallel_Min_In_Matrix_MPI, Test_Manual4) {
    boost::mpi::communicator world;
    std::vector<int> A, B, C;
    const int n = 4;

    if (world.rank() == 0) {
        A = {0, -1, 3, 42,
             6, 2, 2, 3,
             -1, -3, 1, 3,
             100, 50, 20, 15};
        B = {
            1, 14, 2, 7,
            8, 10, 11, -2,
            -1, -100, 3, 2,
            52, 42, 13, 22};
    }

    C = getParallelMatrMul(A, B, n);

    if (world.rank() == 0) {
        ASSERT_EQ(286, C[0]);
        ASSERT_EQ(-61, C[1]);
        ASSERT_EQ(193, C[2]);
        ASSERT_EQ(921, C[3]);
        ASSERT_EQ(59, C[4]);
        ASSERT_EQ(84, C[5]);
        ASSERT_EQ(-194, C[6]);
        ASSERT_EQ(488, C[7]);
        ASSERT_EQ(-20, C[8]);
        ASSERT_EQ(-33, C[9]);
        ASSERT_EQ(310, C[10]);
        ASSERT_EQ(-99, C[11]);
        ASSERT_EQ(945, C[12]);
        ASSERT_EQ(1490, C[13]);
        ASSERT_EQ(-5010, C[14]);
        ASSERT_EQ(7890, C[15]);
    }
}

TEST(Parallel_Min_In_Matrix_MPI, Test_Manual8) {
    boost::mpi::communicator world;
    std::vector<int> A, B, C;
    const int n = 8;

    if (world.rank() == 0) {
        A = {
            74, -68, 88, -40, 1, -59, -97, -66,
            60, -86, -80, 28, 4, -51, 52, 27,
            23, -37, 55, 3, 69, -40, -34, 98,
            -27, -69, 90, -57, 21, 67, 81, 61,
            -96, -100, -15, -57, 19, 46, 31, 68,
            -54, -89, -63, 75, 74, 56, -52, -25,
            68, -32, 56, 25, 97, -68, 37, 34,
            17, 8, 17, -89, -4, 22, 61, 69};
        B = {
            -83, 49, -15, -20, -55, 60, 9, -96,
            -23, 69, -70, 70, -16, 70, 20, -40,
            71, -28, 70, 20, 92, 32, 25, -30,
            5, 66, -96, -92, 51, 6, 70, -96,
            38, 37, -79, -21, 54, 38, -45, -55,
            25, 21, 55, -12, -89, -67, 51, 48,
            20, 72, -18, -37, 88, -82, -10, -10,
            -73, -22, -72, -81, -19, 98, -57, -52};
    }

    C = getParallelMatrMul(A, B, n);

    if (world.rank() == 0) {
        ASSERT_EQ(-8126, C[0]);
        ASSERT_EQ(-18800, C[1]);
        ASSERT_EQ(10277, C[2]);
        ASSERT_EQ(-9643, C[3]);
        ASSERT_EQ(-9, C[4]);
        ASSERT_EQ(1491, C[5]);
        ASSERT_EQ(3036, C[6]);
        ASSERT_EQ(-3842, C[7]);
        ASSERT_EQ(-13958, C[8]);
        ASSERT_EQ(-3428, C[9]);
        ASSERT_EQ(854, C[10]);
        ASSERT_EQ(674, C[11]);
        ASSERT_EQ(-717, C[12]);
        ASSERT_EQ(1967, C[13]);
        ASSERT_EQ(-844, C[14]);
        ASSERT_EQ(-8438, C[15]);
        ASSERT_EQ(-20516, C[16]);
        ASSERT_EQ(-15226, C[17]);
        ASSERT_EQ(7857, C[18]);
        ASSERT_EQ(-16392, C[19]);
        ASSERT_EQ(-6557, C[20]);
        ASSERT_EQ(2296, C[21]);
        ASSERT_EQ(5407, C[22]);
        ASSERT_EQ(-13457, C[23]);
        ASSERT_EQ(-3612, C[24]);
        ASSERT_EQ(-10896, C[25]);
        ASSERT_EQ(9446, C[26]);
        ASSERT_EQ(-6798, C[27]);
        ASSERT_EQ(-12812, C[28]);
        ASSERT_EQ(4211, C[29]);
        ASSERT_EQ(-10085, C[30]);
        ASSERT_EQ(4, C[31]);
        ASSERT_EQ(-101, C[32]);
        ASSERT_EQ(-6816, C[33]);
        ASSERT_EQ(-4251, C[34]);
        ASSERT_EQ(-3509, C[35]);
        ASSERT_EQ(-7327, C[36]);
        ASSERT_EQ(-4569, C[37]);
        ASSERT_EQ(-9831, C[38]);
        ASSERT_EQ(13749, C[39]);
        ASSERT_EQ(788, C[40]);
        ASSERT_EQ(7457, C[41]);
        ASSERT_EQ(3798, C[42]);
        ASSERT_EQ(-4126, C[43]);
        ASSERT_EQ(7896, C[44]);
        ASSERT_EQ(-21774, C[45]);
        ASSERT_EQ(-6439, C[46]);
        ASSERT_EQ(12707, C[47]);
        ASSERT_EQ(-20898, C[48]);
        ASSERT_EQ(-12874, C[49]);
        ASSERT_EQ(16797, C[50]);
        ASSERT_EQ(-5583, C[51]);
        ASSERT_EQ(-4430, C[52]);
        ASSERT_EQ(3250, C[53]);
        ASSERT_EQ(10525, C[54]);
        ASSERT_EQ(-22701, C[55]);
        ASSERT_EQ(-4029, C[56]);
        ASSERT_EQ(-7195, C[57]);
        ASSERT_EQ(184, C[58]);
        ASSERT_EQ(4743, C[59]);
        ASSERT_EQ(-4452, C[60]);
        ASSERT_EQ(7901, C[61]);
        ASSERT_EQ(447, C[62]);
        ASSERT_EQ(-265, C[63]);
    }
}

int main(int argc, char** argv) {
    boost::mpi::environment env(argc, argv);
    boost::mpi::communicator world;
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::TestEventListeners& listeners = ::testing::UnitTest::GetInstance()->listeners();
    if (world.rank() != 0) {
        delete listeners.Release(listeners.default_result_printer());
    }
    return RUN_ALL_TESTS();
}
