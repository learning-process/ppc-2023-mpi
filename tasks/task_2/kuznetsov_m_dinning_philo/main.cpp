  // Copyright 2023 Kuznetsov Mikhail
#include <gtest/gtest.h>
#include <mpi.h>
#include <cmath>
#include "task_2/kuznetsov_m_dinning_philo/dinning_philo.h"


TEST(dinning_philo, test_1) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if ( rank == 0 ) {
        int64_t* data_ptr = new int64_t;
        *data_ptr = 0;
        dinning(data_ptr);

        EXPECT_EQ(0, *data_ptr);
        delete data_ptr;
    } else {
        dinning(nullptr);
    }
}

TEST(dinning_philo, test_2) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if ( rank == 0 ) {
        int64_t* data_ptr = new int64_t;
        *data_ptr = 100;
        dinning(data_ptr);

        EXPECT_EQ(100, *data_ptr);
        delete data_ptr;
    } else {
        dinning(nullptr);
    }
}

TEST(dinning_philo, test_3) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if ( rank == 0 ) {
        int64_t* data_ptr = new int64_t;
        *data_ptr = -100;
        dinning(data_ptr);

        EXPECT_EQ(-100, *data_ptr);
        delete data_ptr;
    } else {
        dinning(nullptr);
    }
}

TEST(dinning_philo, test_4) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if ( rank == 0 ) {
        int64_t* data_ptr = new int64_t;
        *data_ptr = 0xfff;
        dinning(data_ptr);

        EXPECT_EQ(0xfff, *data_ptr);
        delete data_ptr;
    } else {
        dinning(nullptr);
    }
}

TEST(dinning_philo, test_5) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if ( rank == 0 ) {
        int64_t* data_ptr = new int64_t;
        *data_ptr = 1e+5;
        dinning(data_ptr);

        EXPECT_EQ(1e+5, *data_ptr);
        delete data_ptr;
    } else {
        dinning(nullptr);
    }
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::TestEventListeners& listeners =
        ::testing::UnitTest::GetInstance()->listeners();
    if (MPI_Init(&argc, &argv) != MPI_SUCCESS) MPI_Abort(MPI_COMM_WORLD, -1);
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank != 0) {
        delete listeners.Release(listeners.default_result_printer());
    }
    int e_code = RUN_ALL_TESTS();
    MPI_Finalize();
    return e_code;
}
