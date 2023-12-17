// Copyright 2023 Akopyan Zal
#include <gtest/gtest.h>
#include <vector>
#include <boost/mpi/environment.hpp>
#include <boost/mpi/communicator.hpp>
#include "./cannon_multiply.h"

TEST(multiply_tests, first) {
    boost::mpi::communicator world;
    int size = 7;
    std::vector<std::vector<double>> a(size, std::vector<double>(size));
    std::vector<std::vector<double>> b(size, std::vector<double>(size));

    if (world.rank() == 0) {
        for (int i = 0; i < size; i++)
            for (int j = 0; j < size; j++) {
                a[i][j] = i * size + j;
                if (i == j)
                    b[i][j] = 1;
                else
                    b[i][j] = 0;
            }
    }
    std::vector<std::vector<double>> res = Cannon_multiply(a, b, size);
    if (world.rank() == 0) {
        ASSERT_EQ(a, res);
    }
}

TEST(multiply_tests, second) {
    boost::mpi::communicator world;
    int size = 3;
    std::vector<std::vector<double>> a;
    std::vector<std::vector<double>> b;

    if (world.rank() == 0) {
        a = {{1.34, 12.54, -10.2}, {7.09, -234.67, -98.2}, {-11.44, 56.9, -0.01}};
        b = {{-3.56, 14.73, -92.92}, {-29.93, 38.93, -0.01}, {23.92, 98.34, -8.08}};
    }
    std::vector<std::vector<double>> res_par = Cannon_multiply(a, b, size);
    if (world.rank() == 0) {
        std::vector<std::vector<double>> res_seq = Matrix_multiply(a, b, size);
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                res_par[i][j] = round(res_par[i][j] / 10000) * 10000;
                res_seq[i][j] = round(res_seq[i][j] / 10000) * 10000;
            }
        }
        ASSERT_EQ(res_seq, res_par);
    }
}


TEST(multiply_tests, third) {
    boost::mpi::communicator world;
    int size = 10;
    std::vector<std::vector<double>> a;
    std::vector<std::vector<double>> b;

    if (world.rank() == 0) {
        a = GetRandomMatrix(size);
        b = GetRandomMatrix(size);
    }
    std::vector<std::vector<double>> res_par = Cannon_multiply(a, b, size);

    if (world.rank() == 0) {
        std::vector<std::vector<double>> res_seq = Matrix_multiply(a, b, size);
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                res_par[i][j] = round(res_par[i][j] / 10000) * 10000;
                res_seq[i][j] = round(res_seq[i][j] / 10000) * 10000;
            }
        }
        ASSERT_EQ(res_seq, res_par);
    }
}

TEST(multiply_tests, fourth) {
    boost::mpi::communicator world;
    int size = 6;
    std::vector<std::vector<double>> a(size, std::vector<double>(size));
    std::vector<std::vector<double>> b(size, std::vector<double>(size));

    if (world.rank() == 0) {
        a = GetRandomMatrix(size);
        for (int i = 0; i < size; i++)
            for (int j = 0; j < size; j++) {
                if (i == j)
                    b[i][j] = 1;
                else
                    b[i][j] = 0;
            }
    }
    std::vector<std::vector<double>> res = Cannon_multiply(a, b, size);
    if (world.rank() == 0) {
        ASSERT_EQ(a, res);
    }
}

TEST(multiply_tests, fifth) {
    boost::mpi::communicator world;
    int size = 7;
    std::vector<std::vector<double>> a(size, std::vector<double>(size));
    std::vector<std::vector<double>> b(size, std::vector<double>(size));

    if (world.rank() == 0) {
        a = GetRandomMatrix(size);
        for (int i = 0; i < size; i++)
            for (int j = 0; j < size; j++) {
                b[i][j] = 0;
            }
    }
    std::vector<std::vector<double>> res = Cannon_multiply(a, b, size);
    if (world.rank() == 0) {
        ASSERT_EQ(b, res);
    }
}

int main(int argc, char** argv) {
    srand((unsigned int)time(0));
    boost::mpi::environment env(argc, argv);
    boost::mpi::communicator world;
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::TestEventListeners& listeners = ::testing::UnitTest::GetInstance()->listeners();
    if (world.rank() != 0) {
        delete listeners.Release(listeners.default_result_printer());
    }
    return RUN_ALL_TESTS();
}
