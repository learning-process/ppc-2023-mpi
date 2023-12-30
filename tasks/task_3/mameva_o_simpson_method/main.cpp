// Copyright 2023 Mamaeva Olga
#include <mpi.h>
#include <gtest/gtest.h>
#include <math.h>
#include <utility>
#include <vector>
#include "../tasks/task_3/mameva_o_simpson_method/kurguzikov_simpson_method.h"

double func1(std::vector<double> v) {
    double x = v[0];
    double y = v[1];
    return (x * x - 2 * y);
}

double func2(std::vector<double> v) {
    double x = v[0];
    double y = v[1];
    double z = v[2];
    return (x + y * y + z * z * z);
}

double func3(std::vector<double> v) {
    double x = v[0];
    double y = v[1];
    double z = v[2];
    return (log10(2 * x * x) + sqrt(z) + 5 * y);
}

double func4(std::vector<double> v) {
    double x = v[0];
    double y = v[1];
    double z = v[2];
    return (exp(x) - sqrt(10) * 5 * sin(y) + cos(-2 * z * z));
}

double func5(std::vector<double> v) {
    double x = v[0];
    double y = v[1];
    double z = v[2];
    double t = v[3];
    return x + y + z + t;
}

double func6(std::vector<double> v) {
    double x = v[0];
    double y = v[1];
    double z = v[2];
    double t = v[3];
    return cos(5 * x) + exp(y) + 2.9 * sin(z) - t * t;
}

double func7(std::vector<double> v) {
    double x = v[0];
    double y = v[1];
    return (x * x - 3 * y);
}

TEST(MultipleIntegraion, Integral_with_2_dimension) {
    int procRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
    int n = 2;
    std::vector<std::pair<double, double>> dist(n);
    std::vector<int> distr(n);
    if (procRank == 0) {
        dist[0] = { 4, 10 }; distr[0] = 100;
        dist[1] = { 1, 56 }; distr[1] = 100;
    }
    double result = ParallelVersion(func1, dist, distr);
    if (procRank == 0) {
        double seq = SequentialVersion(func1, dist, distr);
        double error = 0.0001;
        ASSERT_NEAR(result, seq, error);
    }
}

TEST(MultipleIntegraion, Integral_with_2_dimension_again) {
    int procRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
    int n = 2;
    std::vector<std::pair<double, double>> dist(n);
    std::vector<int> distr(n);
    if (procRank == 0) {
        dist[0] = { 4, 10 }; distr[0] = 100;
        dist[1] = { 1, 56 }; distr[1] = 100;
    }
    double result = ParallelVersion(func7, dist, distr);
    if (procRank == 0) {
        double seq = SequentialVersion(func7, dist, distr);
        double error = 0.0001;
        ASSERT_NEAR(result, seq, error);
    }
}

TEST(MultipleIntegraion, Integral_with_3_dimension) {
    int procRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
    int n = 3;
    std::vector<std::pair<double, double>> dist(n);
    std::vector<int> distr(n);
    if (procRank == 0) {
        dist[0] = { 0, 1 }; distr[0] = 100;
        dist[1] = { -13, 5 }; distr[1] = 10;
        dist[2] = { 3, 7 }; distr[2] = 10;
    }
    double result = ParallelVersion(func2, dist, distr);
    if (procRank == 0) {
        double seq = SequentialVersion(func2, dist, distr);
        double error = 0.0001;
        ASSERT_NEAR(result, seq, error);
    }
}

TEST(MultipleIntegraion, Integral_with_3_dimension_and_use_log_function) {
    int procRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
    int n = 3;
    std::vector<std::pair<double, double>> dist(n);
    std::vector<int> distr(n);
    if (procRank == 0) {
        dist[0] = { 1, 2 }; distr[0] = 100;
        dist[1] = { -13, 5 }; distr[1] = 10;
        dist[2] = { 3, 7 }; distr[2] = 10;
    }
    double result = ParallelVersion(func3, dist, distr);
    if (procRank == 0) {
        double seq = SequentialVersion(func3, dist, distr);
        double error = 0.0001;
        ASSERT_NEAR(result, seq, error);
    }
}

TEST(MultipleIntegraion,
        Integral_with_3_dimension_and_use_sin_and_cos_functions) {
    int procRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
    int n = 3;
    std::vector<std::pair<double, double>> dist(n);
    std::vector<std::pair<double, double>> dist_seq(n);
    std::vector<int> distr(n);
    if (procRank == 0) {
        dist[0] = { -9, 1 }; distr[0] = 100;
        dist[1] = { -100, 100 }; distr[1] = 10;
        dist[2] = { -2, 2 }; distr[2] = 10;
    }
    double result = ParallelVersion(func4, dist, distr);
    if (procRank == 0) {
        double seq = SequentialVersion(func4, dist, distr);
        double error = 0.0001;
        ASSERT_NEAR(result, seq, error);
    }
}

/*TEST(MultipleIntegraion, First_Integral_with_4_dimension_easy_version) {
    int procRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
    int n = 4;
    std::vector<std::pair<double, double>> dist(n);
    std::vector<int> distr(n);
    if (procRank == 0) {
        dist[0] = { -2, 2 }; distr[0] = 10;
        dist[1] = { -3.3, 5 }; distr[1] = 10;
        dist[2] = { -9, 1 }; distr[2] = 10;
        dist[3] = { 5, 7 }; distr[3] = 10;
    }
    double time1 = MPI_Wtime();
    double result = ParallelVersion(func5, dist, distr);
    double time2 = MPI_Wtime();
    if (procRank == 0) {
        std::cout << "Parallel time : "<< time2 - time1 << '\n';
        double time3 = MPI_Wtime();
        double seq = SequentialVersion(func5, dist, distr);
        double time4 = MPI_Wtime();
        std::cout << "Sequential time : " << time4 - time3 << '\n';
        double error = 0.0001;
        ASSERT_NEAR(result, seq, error);
    }
}

TEST(MultipleIntegraion, Second_Integral_with_4_dimension_hard_version) {
    int procRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
    int n = 4;
    std::vector<std::pair<double, double>> dist(n);
    std::vector<int> distr(n);
    if (procRank == 0) {
        dist[0] = { -4.5, 0 }; distr[0] = 10;
        dist[1] = { 0, 5 }; distr[1] = 10;
        dist[2] = { -58, 12 }; distr[2] = 10;
        dist[3] = { 5, 73 }; distr[3] = 10;
    }
    double time1 = MPI_Wtime();
    double result = ParallelVersion(func6, dist, distr);
    double time2 = MPI_Wtime();
    if (procRank == 0) {
        std::cout << "Parallel time : " << time2 - time1 << '\n';
        double time3 = MPI_Wtime();
        double seq = SequentialVersion(func6, dist, distr);
        double time4 = MPI_Wtime();
        std::cout << "Sequential time : " << time4 - time3 << '\n';
        double error = 0.0001;
        ASSERT_NEAR(result, seq, error);
    }
}*/

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
     int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::TestEventListeners& listeners = ::testing::UnitTest::GetInstance()->listeners();
    if (rank != 0) {
        delete listeners.Release(listeners.default_result_printer());
    }
    int result = RUN_ALL_TESTS();

    MPI_Finalize();

    return result;
}
