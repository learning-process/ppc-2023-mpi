// Copyright 2023 Pozdnyakov Vasya
#include <gtest/gtest.h>
#include <mpi.h>
#include <cmath>
#include "./allreduce.h"

TEST(Allreduce, test_int_sum) {
    int rank = 0;
    int world_size = 0;
    int ret_code, ret_code2;
    double t1, t2, t3, t4;
    MPI_Comm comm = MPI_COMM_WORLD;

    MPI_Comm_rank(comm, &rank);
    MPI_Comm_size(comm, &world_size);

    int value = rank + 1;
    int res = 0;
    int right_res = 0;

    if (rank == 0)
        t1 = MPI_Wtime();
    ret_code = allreduce(&value, &res, 1, MPI_INT, MPI_SUM, comm);
    if (rank == 0 && ret_code == MPI_SUCCESS) {
        t2 = MPI_Wtime();
        t3 = MPI_Wtime();
    }
    ret_code2 = MPI_Allreduce(&value, &right_res, 1, MPI_INT, MPI_SUM, comm);
    if (rank == 0 && ret_code2 == MPI_SUCCESS) {
        t4 = MPI_Wtime();
        std::cout << "MyAllReduceTime1 = " << t2 - t1 << "\t" <<
        "OriginalAllReduceTime = " << t4 - t3 << std::endl;
        ASSERT_EQ(res, right_res);
    }
}

TEST(Allreduce, test_double_max) {
    int rank = 0;
    int world_size = 0;
    int ret_code, ret_code2;
    double t1, t2, t3, t4;
    MPI_Comm comm = MPI_COMM_WORLD;

    MPI_Comm_rank(comm, &rank);
    MPI_Comm_size(comm, &world_size);

    double value = rank + 1.6;
    double res = 0;
    double right_res = 0;

    if (rank == 0)
        t1 = MPI_Wtime();
    ret_code = allreduce(&value, &res, 1, MPI_INT, MPI_MAX, comm);
    if (rank == 0 && ret_code == MPI_SUCCESS) {
        t2 = MPI_Wtime();
        t3 = MPI_Wtime();
    }
    ret_code2 = MPI_Allreduce(&value, &right_res, 1, MPI_INT, MPI_MAX, comm);
    if (rank == 0 && ret_code2 == MPI_SUCCESS) {
        t4 = MPI_Wtime();
        std::cout << "MyAllReduceTime2 = " << t2 - t1 << "\t" <<
        "OriginalAllReduceTime = " << t4 - t3 << std::endl;
        ASSERT_EQ(res, right_res);
    }
}

TEST(Allreduce, test_float_prod) {
    int rank = 0;
    int world_size = 0;
    int ret_code, ret_code2;
    double t1, t2, t3, t4;
    MPI_Comm comm = MPI_COMM_WORLD;

    MPI_Comm_rank(comm, &rank);
    MPI_Comm_size(comm, &world_size);

    float value = 3.0;
    float res = 0;
    float right_res = 0;

    if (rank == 0)
        t1 = MPI_Wtime();
    ret_code = allreduce(&value, &res, 1, MPI_INT, MPI_PROD, comm);
    if (rank == 0 && ret_code == MPI_SUCCESS) {
        t2 = MPI_Wtime();
        t3 = MPI_Wtime();
    }
    ret_code2 = MPI_Allreduce(&value, &right_res, 1, MPI_INT, MPI_PROD, comm);
    if (rank == 0 && ret_code2 == MPI_SUCCESS) {
        t4 = MPI_Wtime();
        std::cout << "MyAllReduceTime3 = " << t2 - t1 << "\t" <<
        "OriginalAllReduceTime = " << t4 - t3 << std::endl;
        ASSERT_EQ(res, right_res);
    }
}

TEST(Allreduce, test_array_sum) {
    int rank = 0;
    int world_size = 0;
    int ret_code, ret_code2;
    double t1, t2, t3, t4;
    MPI_Comm comm = MPI_COMM_WORLD;

    MPI_Comm_rank(comm, &rank);
    MPI_Comm_size(comm, &world_size);

    int* value = new int[3];
    int* res = new int[3];
    int* right_res = new int[3];

    for (int i = 0; i < 3; i++) {
        value[i] = rank + i;
    }

    if (rank == 0)
        t1 = MPI_Wtime();
    ret_code = allreduce(value, res, 3, MPI_INT, MPI_SUM, comm);
    if (rank == 0 && ret_code == MPI_SUCCESS) {
        t2 = MPI_Wtime();
        t3 = MPI_Wtime();
    }
    ret_code2 = MPI_Allreduce(value, right_res, 3, MPI_INT, MPI_SUM, comm);
    if (rank == 0 && ret_code2 == MPI_SUCCESS) {
        t4 = MPI_Wtime();
        std::cout << "MyAllReduceTime4 = " << t2 - t1 << "\t" <<
        "OriginalAllReduceTime = " << t4 - t3 << std::endl;
        for (int i = 0; i < 3; i++) {
            ASSERT_EQ(res[i], right_res[i]);
        }
    }

    delete[] value;
    delete[] res;
    delete[] right_res;
}

TEST(Allreduce, test_int_land) {
    int rank = 0;
    int world_size = 0;
    int ret_code, ret_code2;
    double t1, t2, t3, t4;
    MPI_Comm comm = MPI_COMM_WORLD;

    MPI_Comm_rank(comm, &rank);
    MPI_Comm_size(comm, &world_size);

    int value = rank;
    int res = 0;
    int right_res = 0;

    if (rank == 0)
        t1 = MPI_Wtime();
    ret_code = allreduce(&value, &res, 1, MPI_INT, MPI_LAND, comm);
    if (rank == 0 && ret_code == MPI_SUCCESS) {
        t2 = MPI_Wtime();
        t3 = MPI_Wtime();
    }
    ret_code2 = MPI_Allreduce(&value, &right_res, 1, MPI_INT, MPI_LAND, comm);
    if (rank == 0 && ret_code2 == MPI_SUCCESS) {
        t4 = MPI_Wtime();
        std::cout << "MyAllReduceTime5 = " << t2 - t1 << "\t" <<
        "OriginalAllReduceTime = " << t4 - t3 << std::endl;
        ASSERT_EQ(res, right_res);
    }
}

int main(int argc, char** argv) {
    int code = 0;
    int rank = 0;

    ::testing::InitGoogleTest(&argc, argv);
    ::testing::TestEventListeners& listeners =
        ::testing::UnitTest::GetInstance()->listeners();

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank != 0) {
        delete listeners.Release(listeners.default_result_printer());
    }

    code = RUN_ALL_TESTS();

    MPI_Finalize();

    return code;
}
