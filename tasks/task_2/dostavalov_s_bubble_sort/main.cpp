// Copyright 2023 Dostavalov Semen

#include <gtest/gtest.h>
#include <mpi.h>
#include <boost/core/invoke_swap.hpp>
#include <boost/mpi/environment.hpp>
#include <boost/mpi/communicator.hpp>
#include "task_2/dostavalov_s_bubble_sort/bubble_sort.h"

TEST(MPI_TESTS, par_test_2) {
    int th_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &th_rank);

    int mysize = 2;
    int* start_arr = new int[mysize];
    int* ref_array;
    for (int i = 0; i < mysize; i++) {
        start_arr[i] = generateNewArr(mysize);
    }
    int* res_arr_Par = doParallel_BubbleS(start_arr, mysize);
    if (th_rank == 0) {
        int* arr = bubble_s_with_return(start_arr, mysize);
        ASSERT_EQ(res_arr_Par, arr);
    }
}

TEST(MPI_TESTS, par_test_11) {
    int th_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &th_rank);

    int mysize = 11;
    int* start_arr = new int[mysize];
    int* ref_array;

    for (int i = 0; i < mysize; i++) {
        start_arr[i] = generateNewArr(mysize);
    }
    int* res_arr_Par = doParallel_BubbleS(start_arr, mysize);

    if (th_rank == 0) {
        int* arr = bubble_s_with_return(start_arr, mysize);
        ASSERT_EQ(res_arr_Par, arr);
    }
}

TEST(MPI_TESTS, par_test_15) {
    int th_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &th_rank);

    int mysize = 11;
    int* start_arr = new int[mysize];
    int* ref_array;

    for (int i = 0; i < mysize; i++) {
        start_arr[i] = generateNewArr(mysize);
    }
    int* res_arr_Par = doParallel_BubbleS(start_arr, mysize);

    if (th_rank == 0) {
        int* arr = bubble_s_with_return(start_arr, mysize);
        ASSERT_EQ(res_arr_Par, arr);
    }
}

TEST(MPI_TESTS, par_test_20) {
    int th_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &th_rank);

    int mysize = 11;
    int* start_arr = new int[mysize];
    int* ref_array;

    for (int i = 0; i < mysize; i++) {
        start_arr[i] = generateNewArr(mysize);
    }
    int* res_arr_Par = doParallel_BubbleS(start_arr, mysize);

    if (th_rank == 0) {
        int* arr = bubble_s_with_return(start_arr, mysize);
        ASSERT_EQ(res_arr_Par, arr);
    }
}
TEST(MPI_TESTS, par_test_3) {
    int th_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &th_rank);

    int mysize = 3;
    int* start_arr = new int[mysize];
    int* ref_array;

    for (int i = 0; i < mysize; i++) {
        start_arr[i] = generateNewArr(mysize);
    }
    int* res_arr_Par = doParallel_BubbleS(start_arr, mysize);

    if (th_rank == 0) {
        int* arr = bubble_s_with_return(start_arr, mysize);
        ASSERT_EQ(res_arr_Par, arr);
    }
}

int main(int argc, char** argv) {
    boost::mpi::environment env(argc, argv);
    boost::mpi::communicator world;

    ::testing::InitGoogleTest(&argc, argv);
    ::testing::TestEventListeners& listeners
        = ::testing::UnitTest::GetInstance()->listeners();

    if (world.rank() != 0) {
        delete listeners.Release(listeners.default_result_printer());
    }
    return RUN_ALL_TESTS();
}
