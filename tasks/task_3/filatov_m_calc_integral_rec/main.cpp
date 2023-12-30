// Copyright 2023 Filatov Maxim

#include <gtest/gtest.h>
#include <iostream>
#include "task_3/filatov_m_calc_integral_rec/calc_integral_rec.h"

double expF(const std::vector<double>& variables) {
    return exp(variables[0]);
}

double expcosF(const std::vector<double>& variables) {
    return sin(variables[0]) + cos(variables[1]);
}

double expcossqrtF(const std::vector<double>& variables) {
    return sin(variables[0]) * cos(variables[1]) + sqrt(variables[2]);
}

TEST(Rect_integ_MPI, Test_single_v) {
    boost::mpi::communicator comm;
    if (!comm.rank()) {
        double true_value = exp(1)- 1 / exp(1);
        double eps = 1e-3;
        int vars = 1;
        std::vector<int> p = {1000};
        std::vector<std::pair<double, double>> l =
            {{-1.0, 1.0}};
        double res =
            integrateSequentially(vars, expF, p, l);
        ASSERT_LT(fabs(true_value - res), eps);
    }
}

TEST(Rect_integ_MPI, Test_two_v) {
    boost::mpi::communicator comm;
    if (!comm.rank()) {
        double true_value = 4 * sin(1);
        double eps = 1e-1;
        int vars = 2;
        std::vector<int> p = {1000, 1000};
        std::vector<std::pair<double, double>> l =
            {{-1.0, 1.0}, {-1.0, 1.0}};
        double res = integrateSequentially(vars, expcosF, p, l);
        ASSERT_LT(fabs(true_value - res), eps);
    }
}

TEST(Rect_integ_MPI, Test_three_v) {
    boost::mpi::communicator comm;
    if (!comm.rank()) {
        double true_value = 0.228538683989507;
        double eps = 1e-1;
        int vars = 3;
        std::vector<int> p = {100, 100, 100};
        std::vector<std::pair<double, double>> l =
            {{2.0, 2.5}, {1.0, 2.0}, {0.5, 1.0}};
        double res = integrateSequentially(vars, expcossqrtF, p, l);
        ASSERT_LT(fabs(true_value - res), eps);
    }
}

TEST(Rectangle_Integrate_MPI_Parallel, Test_single_v) {
    boost::mpi::communicator comm;
    int vars = 1;
    std::vector<int> p = {1000};
    std::vector<std::pair<double, double>> l;
    double eps = 1e-6;
    if (!comm.rank()) {
        l = {getRandomLimits(-1.0, 1.0)};
    }

    double parallel_result = integrateInParallel(vars, expF, p, l);
    if (!comm.rank()) {
        double sequentially_result =
            integrateSequentially(vars, expF, p, l);
        ASSERT_LT(fabs(parallel_result - sequentially_result), eps);
    }
}

TEST(Rectangle_Integrate_MPI_Parallel, Test_two_v) {
    boost::mpi::communicator comm;
    int vars = 2;
    std::vector<int> p = {1000, 1000};
    std::vector<std::pair<double, double>> l;
    double eps = 1e-6;
    if (!comm.rank()) {
        l = {getRandomLimits(-5, 5), getRandomLimits(5, 10)};
    }

    double parallel_result = integrateInParallel(vars, expcosF, p, l);
    if (!comm.rank()) {
        double sequentially_result =
            integrateSequentially(vars, expcosF, p, l);
        ASSERT_LT(fabs(parallel_result - sequentially_result), eps);
    }
}

TEST(Rectangle_Integrate_MPI_Parallel, Test_three_v) {
    boost::mpi::communicator comm;
    int vars = 3;
    std::vector<int> p = {100, 100, 100};
    std::vector<std::pair<double, double>> l;
    double eps = 1e-6;
    if (!comm.rank()) {
        l = {
            getRandomLimits(-2.0, 1.0),
            getRandomLimits(-10.0, 0.0),
            getRandomLimits(0.0, 5.0)
        };
    }
    double parallel_result = integrateInParallel(vars, expcossqrtF, p, l);
    if (!comm.rank()) {
        double sequentially_result =
            integrateSequentially(vars, expcossqrtF, p, l);
        ASSERT_LT(fabs(parallel_result - sequentially_result), eps);
    }
}

int main(int argc, char** argv) {
    boost::mpi::environment env(argc, argv);
    boost::mpi::communicator comm;
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::TestEventListeners& listeners =
        ::testing::UnitTest::GetInstance()->listeners();
    if (comm.rank() != 0) {
        delete listeners.Release(listeners.default_result_printer());
    }
    return RUN_ALL_TESTS();
}
