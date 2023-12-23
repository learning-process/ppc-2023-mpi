// Copyright 2023 Makhinya Danil
#include <gtest/gtest.h>
#include <vector>
#include <cmath>
#include "./gradient_method.h"
#include <boost/mpi/environment.hpp>
#include <boost/mpi/communicator.hpp>

TEST(Parallel_Readers_Writes_MPI, Test1) {
    boost::mpi::communicator world;
    matrix A;
    vector b;
    
    if (world.rank() == 0) {
        A = {{ 2,-1, 0},
             {-1, 2,-1},
             { 0,-1, 2}};

        b = {1,2,3};
    }

    vector x = solve_parallel(A, b);

    if (world.rank() == 0) {
        vector reference = {2.5, 4.0, 3.5};
        EXPECT_NEAR(measure(reference - x), 0.0, EPS);
    }
}

TEST(Parallel_Readers_Writes_MPI, Test2) {
    boost::mpi::communicator world;
    matrix A;
    vector b;
    
    if (world.rank() == 0) {
        A = {{ 2,-1, 0},
             {-1, 2,-1},
             { 0,-1, 2}};

        b = {0,0,0};
    }

    vector x = solve_parallel(A, b);

    if (world.rank() == 0) {
        vector reference = {0, 0, 0};
        EXPECT_NEAR(measure(reference - x), 0.0, EPS);
    }
}

TEST(Parallel_Readers_Writes_MPI, Test3) {
    boost::mpi::communicator world;
    matrix A;
    vector b;
    
    if (world.rank() == 0) {
        A = {{ 2,-1, 0},
             {-1, 2,-1},
             { 0,-1, 2}};

        b = {1,0,1};
    }

    vector x = solve_parallel(A, b);

    if (world.rank() == 0) {
        vector reference = {1, 1, 1};
        EXPECT_NEAR(measure(reference - x), 0.0, EPS);
    }
}

TEST(Parallel_Readers_Writes_MPI, Test4) {
    boost::mpi::communicator world;
    matrix A;
    vector b;
    
    if (world.rank() == 0) {
        A = {{-11,6,-6},
             { 6,-6, 3},
             {-6,3, -6}};

        b = {-66,18,-54};
    }

    vector x = solve_parallel(A, b);

    if (world.rank() == 0) {
        vector reference = {6, 6, 6};
        EXPECT_NEAR(measure(reference - x), 0.0, EPS);
    }
}

TEST(Parallel_Readers_Writes_MPI, Test5) {
    boost::mpi::communicator world;
    matrix A;
    vector b;
    
    if (world.rank() == 0) {
        A = {{-11,6,-6},
             { 6,-6, 3},
             {-6,3, -6}};

        b = {-5,3,0};
    }

    vector x = solve_parallel(A, b);

    if (world.rank() == 0) {
        vector reference = {1, 0, -1};
        EXPECT_NEAR(measure(reference - x), 0.0, EPS);
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
    RUN_ALL_TESTS();
    return 0;
}
