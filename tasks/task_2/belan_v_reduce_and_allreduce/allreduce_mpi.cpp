/* Copyright 2023 Belan Vadim */

#include <mpi.h>
#include <iostream>
#include <random>
#include "task_2/belan_v_reduce_and_allreduce/allreduce_mpi.h"

/*
Допустим, у нас есть задача, 
в которой каждый процесс генерирует случайное число, 
а затем все процессы должны вычислить среднее арифметическое этих чисел.

Для решения можно использовать mpi_all_reduce из моей реализации. 
Каждый процесс будет генерировать случайное число, 
а затем все процессы вызывают mpi_all_reduce, 
передавая свои значения для суммирования. 

Результатом работы функции будет сумма всех значений, 
которую затем нужно поделить на общее количество процессов, 
чтобы получить среднее арифметическое.
*/

double calculate_average() {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Генерируем случайное число
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 100);
    int local_value = dis(gen);

    int sum;
    // Вычисляем сумму всех значений
    mpi_all_reduce(local_value, &sum);

    // Вычисляем среднее арифметическое
    double average = static_cast<double>(sum) / size;

    return average;
}

void mpi_reduce(int local_value, int* global_sum, int root) {
    MPI_Status status;
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int* local_values = new int[size];
    int* received_sums = new int[size];

    MPI_Gather(&local_value, 1, MPI_INT,
               local_values, 1, MPI_INT,
               root, MPI_COMM_WORLD);

    if (rank == root) {
        *global_sum = local_values[0];
        for (int i = 1; i < size; i++) {
            *global_sum += local_values[i];
        }
        for (int i = 0; i < size; i++) {
            if (i != root) {
                MPI_Send(global_sum, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            }
        }
    } else {
        // Принять глобальную сумму от root процесса
        MPI_Recv(global_sum, 1, MPI_INT,
                 root, MPI_ANY_TAG,
                 MPI_COMM_WORLD, &status);
    }

    delete[] local_values;
    delete[] received_sums;
}

void mpi_all_reduce(int local_value, int* inclusive_sum) {
    MPI_Status status;
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int* local_values = new int[size];

    MPI_Allgather(&local_value, 1, MPI_INT,
                  local_values, 1, MPI_INT,
                  MPI_COMM_WORLD);

    *inclusive_sum = local_values[0];
    for (int i = 1; i < size; i++) {
        *inclusive_sum += local_values[i];
    }

    delete[] local_values;
}
