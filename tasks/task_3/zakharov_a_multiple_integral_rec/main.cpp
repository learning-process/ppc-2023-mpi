// Copyright 2023 Zakharov Artem
#include <gtest/gtest.h>
#include <vector>
#include "./multiple_integral.h"
#include <boost/mpi/environment.hpp>
#include <boost/mpi/communicator.hpp>
#include <boost/mpi/timer.hpp>

TEST(Sequential_Integral, Test_One_Var) {
    boost::mpi::communicator world;
    if (world.rank() == 0) {
        double true_val = 1.42823590869;
        double epsilon = 1e-3;
        std::size_t num_vars = 1;
        std::vector<std::size_t> num_parts = {1000};
        std::vector<std::pair<double, double>> lim_integ = {{-1, 15.23}};

        double result = calc_integ_seq(num_vars, one_var_func,
                                       num_parts, lim_integ);
        ASSERT_LT(std::abs(result - true_val), epsilon);
    }
}

TEST(Sequential_Integral, Test_Two_Var) {
    boost::mpi::communicator world;
    if (world.rank() == 0) {
        double true_val = 12062.2623333;
        double epsilon = 1e-1;
        std::size_t num_vars = 2;
        std::vector<std::size_t> num_parts = {1000, 1000};
        std::vector<std::pair<double, double>> lim_integ = {{-2.3, 4.1},
                                                            {0, 6.5}
        };

        double result = calc_integ_seq(num_vars, two_var_func,
                                       num_parts, lim_integ);
        ASSERT_LT(std::abs(result - true_val), epsilon);
    }
}

TEST(Sequential_Integral, Test_Three_Var) {
    boost::mpi::communicator world;
    if (world.rank() == 0) {
        double true_val = 6.23202211038;
        double epsilon = 1e-1;
        std::size_t num_vars = 3;
        std::vector<std::size_t> num_parts = {100, 100, 100};
        std::vector<std::pair<double, double>> lim_integ = {{1, 2},
                                                            {3.2, 5},
                                                            {6.743, 8}
        };

        double result = calc_integ_seq(num_vars, three_var_func,
                                       num_parts, lim_integ);
        ASSERT_LT(std::abs(result - true_val), epsilon);
    }
}

TEST(Parallel_Integral, Test_One_Var) {
    boost::mpi::communicator world;
    std::size_t num_vars = 1;
    std::vector<std::size_t> num_parts = {1000};
    std::vector<std::pair<double, double>> lim_integ = {};
    double epsilon = 1e-6;
    if (world.rank() == 0) {
        lim_integ = {{-134.4, 45.231}};
    }

    double par_result = calc_integ_par(num_vars, one_var_func,
                                            num_parts, &lim_integ);
    if (world.rank() == 0) {
        double seq_result = calc_integ_seq(num_vars, one_var_func,
                                           num_parts, lim_integ);
        ASSERT_LT(std::abs(par_result - seq_result), epsilon);
    }
}

TEST(Parallel_Integral, Test_Two_Var) {
    boost::mpi::communicator world;
    std::size_t num_vars = 2;
    std::vector<std::size_t> num_parts = {500, 500};
    std::vector<std::pair<double, double>> lim_integ = {};
    double epsilon = 1e-6;
    if (world.rank() == 0) {
        lim_integ = {get_random_lim(-3, 13), get_random_lim(5, 19)};
    }

    double par_result = calc_integ_par(num_vars, two_var_func,
                                       num_parts, &lim_integ);
    if (world.rank() == 0) {
        double seq_result = calc_integ_seq(num_vars, two_var_func,
                                           num_parts, lim_integ);
        ASSERT_LT(std::abs(par_result - seq_result), epsilon);
    }
}

TEST(Parallel_Integral, Test_Two_Var_2) {
    boost::mpi::communicator world;
    std::size_t num_vars = 2;
    std::vector<std::size_t> num_parts = {500, 500};
    std::vector<std::pair<double, double>> lim_integ = {};
    double epsilon = 1e-6;
    if (world.rank() == 0) {
        lim_integ = {{13.489, 19.25}, {2.148, 4.015}};
    }

    double par_result = calc_integ_par(num_vars, two_var_func_2,
                                       num_parts, &lim_integ);
    if (world.rank() == 0) {
        double seq_result = calc_integ_seq(num_vars, two_var_func_2,
                                           num_parts, lim_integ);
        ASSERT_LT(std::abs(par_result - seq_result), epsilon);
    }
}

TEST(Parallel_Integral, Test_Three_Var) {
    boost::mpi::communicator world;
    std::size_t num_vars = 3;
    std::vector<std::size_t> num_parts = {100, 100, 100};
    std::vector<std::pair<double, double>> lim_integ = {};
    double epsilon = 1e-6;
    if (world.rank() == 0) {
        lim_integ = {get_random_lim(0, 7),
                     get_random_lim(5, 9),
                     get_random_lim(23, 28)
        };
    }

    double par_result = calc_integ_par(num_vars, three_var_func,
                                       num_parts, &lim_integ);
    if (world.rank() == 0) {
        double seq_result = calc_integ_seq(num_vars, three_var_func,
                                           num_parts, lim_integ);
        ASSERT_LT(std::abs(par_result - seq_result), epsilon);
    }
}

TEST(Parallel_Integral, Test_Four_Var) {
    boost::mpi::communicator world;
    std::size_t num_vars = 4;
    std::vector<std::size_t> num_parts = {28, 28, 28, 28};
    std::vector<std::pair<double, double>> lim_integ = {};
    double epsilon = 1e-6;
    if (world.rank() == 0) {
        lim_integ = {{1.32, 1.98}, {72.4, 73}, {-3.2, -2.89}, {3.43, 4}};
    }

    double par_result = calc_integ_par(num_vars, four_var_func,
                                       num_parts, &lim_integ);
    if (world.rank() == 0) {
        double seq_result = calc_integ_seq(num_vars, four_var_func,
                                           num_parts, lim_integ);
        ASSERT_LT(std::abs(par_result - seq_result), epsilon);
    }
}

int main(int argc, char** argv) {
    boost::mpi::environment env(argc, argv);
    boost::mpi::communicator world;
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::TestEventListeners& listeners =
            ::testing::UnitTest::GetInstance()->listeners();
    if (world.rank() != 0) {
        delete listeners.Release(listeners.default_result_printer());
    }
    return RUN_ALL_TESTS();
}
