/* Copyright 2023 Belan Vadim */

#include <mpi.h>
#include <gtest/gtest.h>
#include <iostream>
#include "task_2/belan_v_reduce_and_allreduce/allreduce_mpi.h"

// Тест для функции mpi_reduce
TEST(AllReduceMPI, MPIReduce) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int local_value = rank + 1;
    int global_sum;

    mpi_reduce(local_value, &global_sum, 0);

    // Проверяем корректность суммирования на процессе с рангом 0
    if (rank == 0) {
        EXPECT_EQ(global_sum, (size * (size + 1)) / 2);
    }
}

// Тест для функции mpi_all_reduce
TEST(AllReduceMPI, MPIAllReduce) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int local_value = rank + 1;
    int inclusive_sum;

    mpi_all_reduce(local_value, &inclusive_sum);

    // Проверяем корректность суммирования на всех процессах
    EXPECT_EQ(inclusive_sum, (size * (size + 1)) / 2);
}

// Тест для функции mpi_reduce с разными значениями root
TEST(AllReduceMPI, MPIReduceDifferentRoot) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int local_value = rank + 1;
    int global_sum;

    // Проверяем корректный результат при использовании разных значений root
    for (int root = 0; root < size; root++) {
        mpi_reduce(local_value, &global_sum, root);
        if (rank == root) {
            EXPECT_EQ(global_sum, (size * (size + 1)) / 2);
        }
    }
}

// Тест для функции mpi_reduce с одинаковыми значениями на всех процессах
TEST(AllReduceMPI, MPIReduceAllSameValue) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int local_value = 10;
    int global_sum;

    mpi_reduce(local_value, &global_sum, 0);

    // Проверяем корректность суммирования на процессе с рангом 0
    if (rank == 0) {
        EXPECT_EQ(global_sum, local_value * size);
    }
}

// Тест для функции mpi_all_reduce с одинаковыми значениями на всех процессах
TEST(AllReduceMPI, MPIAllReduceAllSameValue) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int local_value = 5;
    int inclusive_sum;

    mpi_all_reduce(local_value, &inclusive_sum);

    // Проверяем корректность суммирования на всех процессах
    EXPECT_EQ(inclusive_sum, local_value * size);
}

// Тест для функции calculate_average
TEST(AllReduceMPI, CalculateAverage) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    double average = calculate_average();

    // Проверяем, что среднее арифметическое значение корректно
    EXPECT_GE(average, 1);
    EXPECT_LE(average, 100);
}

int main(int argc, char* argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    MPI_Init(&argc, &argv);

    int result = RUN_ALL_TESTS();

    MPI_Finalize();
    return result;
}
