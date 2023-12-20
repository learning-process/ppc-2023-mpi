// Copyright 2023 Smirnova Daria
#include <gtest/gtest.h>
#include <mpi.h>
#include <vector>
#include <cmath>
#include <ctime>
#include <iostream>
#include "./monte_carlo_multiple_integration.h"
#include <boost/mpi/environment.hpp>
#include <boost/mpi/communicator.hpp>

double func_1(std::vector<double> x) {
    return x[0] * x[0];
}

double func_2(std::vector<double> x) {
    return 3 * x[0] * x[0] * x[0] + 2 * x[1] * x[1];
}

double func_3(std::vector<double> x) {
    return sin(x[0]) + 2 * x[1] + x[2] * x[2];
}

double func_4(std::vector<double> x) {
    return x[0] * x[0] + 2 * x[1] - cos(x[2]) + 2 * x[3] * x[3] * x[3] - 3 * x[4];
}


TEST(Monte_carlo_multiple_integration_MPI, N_is_negative) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    double a = 0.0, b = 2.0;
    if (rank == 0) {
        ASSERT_ANY_THROW(Monte_Carlo_Multiple_Integration(func_1, { a }, { b }, -1000));
    }
}

TEST(Monte_carlo_multiple_integration_MPI, Multiplicity_is_1) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    double a = 0.0, b = 1.0;
    double global_res = Monte_Carlo_Multiple_Integration_Parallel(func_1, { a }, { b }, 100000);
    if (rank == 0) {
        double local_res = Monte_Carlo_Multiple_Integration(func_1, { a }, { b }, 100000);
        ASSERT_NEAR(global_res, local_res, 0.05);
    }
}

TEST(Monte_carlo_multiple_integration_MPI, Multiplicity_is_2) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<double> a = { 0.0, 2.5 };
    std::vector<double> b = { 1.534, 3.12 };
    double global_res = Monte_Carlo_Multiple_Integration_Parallel(func_2, a, b, 100000);
    if (rank == 0) {
        double local_res = Monte_Carlo_Multiple_Integration(func_2, a, b, 100000);
        ASSERT_NEAR(global_res, local_res, 0.05);
  }
}

TEST(Monte_carlo_multiple_integration_MPI, Multiplicity_is_3) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<double> a = { 0.5, 1.5, 2.5 };
    std::vector<double> b = { 1.5, 2.5, 3.5 };
    double global_res = Monte_Carlo_Multiple_Integration_Parallel(func_3, a, b, 100000);
    if (rank == 0) {
        double local_res = Monte_Carlo_Multiple_Integration(func_3, a, b, 100000);
        ASSERT_NEAR(global_res, local_res, 0.05);
    }
}

TEST(Monte_carlo_multiple_integration_MPI, Multiplicity_is_4) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  std::vector<double> a = { 0.0, -2.5, 1.5, -5.0 };
  std::vector<double> b = { 1.0, -1.0, 2.5, -3.0 };
  double global_res = Monte_Carlo_Multiple_Integration_Parallel(func_3, a, b, 1000000);
  if (rank == 0) {
    double local_res = Monte_Carlo_Multiple_Integration(func_3, a, b, 1000000);
    ASSERT_NEAR(global_res, local_res, 0.05);
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
