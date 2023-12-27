// Copyright 2023 Karagodin Andrey
#include <gtest/gtest.h>
#include <mpi.h>
#include <task_2/karagodin_a_yakobi_method/yakobi_method.h>

TEST(Parallel_Operations_MPI, operator_minus_test) {
    int rank = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<double> x = {0.0, 1.0, 3.0, -5.0, -1.1};
    std::vector<double> y = {-3.0, 2.0, -1.1, 2.2, -3.3};
    std::vector<double> answer = {3.0, -1.0, 4.1, -7.2, 2.2};
    std::vector<double> result = x - y;
    if (rank == 0) {
        for (std::size_t i = 0; i < result.size(); i++) {
            ASSERT_DOUBLE_EQ(answer[i], result[i]);
        }
    }
}

TEST(Parallel_Operations_MPI, normalize_test) {
    int rank = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<double> x = {0.0, 1.0, 3.0, -5.0, -1.1};
    std::vector<double> y = {-3.0, 2.0, -1.1, 2.2, -3.3};
    std::vector<double> answer = {3.0, -1.0, 4.1, -7.2, 2.2};
    std::vector<double> result = x - y;
    if (rank == 0) {
        double norms[4] = {5.0, 3.3, 7.2, 7.2};
        ASSERT_DOUBLE_EQ(norms[0], normalize(x));
        ASSERT_DOUBLE_EQ(norms[1], normalize(y));
        ASSERT_DOUBLE_EQ(norms[2], normalize(answer));
        ASSERT_DOUBLE_EQ(norms[3], normalize(result));
    }
}

TEST(Parallel_Operations_MPI, VectorIncrease_test) {
    int rank = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<double> x = {0.0, 1.0, 3.0, -5.0, -1.1};
    std::vector<double> y = VectorIncrease(x, 10);
    if (rank == 0) {
        for (std::size_t i = 0; i < x.size(); i++) {
            ASSERT_DOUBLE_EQ(x[i], y[i]);
        }
        for (std::size_t i = x.size(); i < x.size() + 10; i++) {
            ASSERT_DOUBLE_EQ(0.0, y[i]);
        }
    }
}

TEST(Parallel_Operations_MPI, MatrixIncrease_test) {
    int rank = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<std::vector<double>> x =
     {{1.1, 1.2}, {2.1, 2.2}};
    std::vector<std::vector<double>> y =
     MatrixIncrease(x, 2);
    if (rank == 0) {
        for (std::size_t i = 0; i < x.size(); i++) {
            for (std::size_t j = 0; j < x.size(); j++) {
                ASSERT_DOUBLE_EQ(x[i][j], y[i][j]);
            }
            for (std::size_t j = x.size(); j < x.size() + 2; j++) {
                ASSERT_DOUBLE_EQ(0.0, y[i][j]);
            }
        }
        for (std::size_t i = x.size(); i < x.size() + 2; i++) {
            for (std::size_t j = 0; j < y.size(); j++) {
                if (i == j)
                    ASSERT_DOUBLE_EQ(1.0, y[i][j]);
                else
                    ASSERT_DOUBLE_EQ(0.0, y[i][j]);
            }
        }
    }
}

TEST(Parallel_Operations_MPI, YakobiSequential_test) {
    int rank = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<std::vector<double>> a = {{20.0, 1.0,  -2.0},
                {3.0,  20.0, -1.0},
                {2.0,  -3.0, 20.0}};
    std::vector<double> b = {17.0, -18.0, 25.0};
    std::vector<double> answer = {1.0, -1.0, 1.0};
    if (rank == 0) {
        std::vector<double> x = YakobiSequential(a, b);
        for (std::size_t i = 0; i < x.size(); i++) {
            ASSERT_DOUBLE_EQ(answer[i], x[i]);
        }
    }
}

TEST(Parallel_Operations_MPI, YakobiParallel_test) {
    int rank = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<std::vector<double>> a = {{20.0, 1.0,  -2.0},
                {3.0,  20.0, -1.0},
                {2.0,  -3.0, 20.0}};
    const std::vector<double> b = {17.0, -18.0, 25.0};
    std::vector<double> answer = {1.0, -1.0, 1.0};
    std::vector<double> x = YakobiParallel(a, b);
    if (rank == 0) {
        for (std::size_t i = 0; i < x.size(); i++) {
            ASSERT_DOUBLE_EQ(answer[i], x[i]);
        }
    }
}

TEST(Parallel_Operations_MPI, YakobiParallel_AnotherTest) {
    int rank = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<std::vector<double>> a = {{4,  -1, 0,  0},
                {-1, 4,  -1, 0},
                {0,  -1, 4,  -1},
                {0,  0,  -1, 4}};
    const std::vector<double> b = {2, 4, 6, 13};
    std::vector<double> answer = {1, 2, 3, 4};
    std::vector<double> x = YakobiParallel(a, b);
    if (rank == 0) {
        for (std::size_t i = 0; i < x.size(); i++) {
            ASSERT_DOUBLE_EQ(answer[i], x[i]);
        }
    }
}

int main(int argc, char** argv) {
    int result_code = 0;
    int rank = 0;
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::TestEventListeners& listeners =
        ::testing::UnitTest::GetInstance()->listeners();
    if (MPI_Init(&argc, &argv) != MPI_SUCCESS) MPI_Abort(MPI_COMM_WORLD, -1);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank != 0) {
        delete listeners.Release(listeners.default_result_printer());
    }
    result_code = RUN_ALL_TESTS();
    MPI_Finalize();

    return result_code;
}
