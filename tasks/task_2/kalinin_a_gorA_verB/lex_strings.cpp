// Copyright 2023 Kalinin Alexandr
#include "task_1/kalinin_a_gorA_verB/gorA_verB.h"

std::vector<int> getRandomMatrix(std::vector<int>::size_type row_count, std::vector<int>::size_type column_count) {
    std::random_device dev;
    std::mt19937 gen(dev());
    std::vector<int> vec(row_count * column_count);
    for (std::vector<int>::size_type i = 0; i < row_count; i++) {
        for (std::vector<int>::size_type j = 0; j < column_count; j++) {
            vec[i * column_count + j] = gen() % 100;
        }
    }
    return vec;
}

std::vector<int> getSequentialOperations(const std::vector<int>& matrix1, const std::vector<int>& matrix2,
    std::vector<int>::size_type row_count_matrix1, std::vector<int>::size_type column_count_matrix1,
    std::vector<int>::size_type column_count_matrix2) {

    std::vector<int>::size_type row_count_matrix2 = column_count_matrix1;

    std::vector<int>::size_type res_size = row_count_matrix1 * column_count_matrix2;
    std::vector<int> res(res_size, 0);

    for (std::vector<int>::size_type i = 0; i < row_count_matrix1; i++) {
        for (std::vector<int>::size_type j = 0; j < column_count_matrix2; j++) {
            for (std::vector<int>::size_type k = 0; k < column_count_matrix1; k++) {
                res[i * column_count_matrix2 + j] +=
                    matrix1[i * row_count_matrix2 + k] * matrix2[k * column_count_matrix2 + j];
            }
        }
    }
    return res;
}

std::vector<int> getParallelOperations(const std::vector<int>& matrix1, const std::vector<int>& matrix2,
    std::vector<int>::size_type row_count_matrix1, std::vector<int>::size_type column_count_matrix1) {

    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (size > static_cast<int>(row_count_matrix1) || size == 1) {
        return rank == 0 ?
            getSequentialOperations(matrix1, matrix2, row_count_matrix1, column_count_matrix1, row_count_matrix1) :
            std::vector<int>{};
    }

    std::vector<int>::size_type row_count_matrix2 = column_count_matrix1;
    std::vector<int>::size_type column_count_matrix2 = row_count_matrix1;
    std::vector<int>::size_type local_count = row_count_matrix1 / size;
    std::vector<int>::size_type remaining = row_count_matrix1 % size;

    if (rank == 0) {
        for (int proc = 1; proc < size; proc++) {
            std::vector<int>::size_type step_matrix1 = proc * local_count * column_count_matrix1;
            std::vector<int>::size_type step_matrix2 = proc * local_count;

            if (remaining != 0) {
                step_matrix1 += remaining * column_count_matrix1;
                step_matrix2 += remaining;
            }

            std::vector<int> send_vector_b(local_count * row_count_matrix2, 0);
            for (std::vector<int>::size_type i = 0; i < send_vector_b.size(); i++) {
                std::vector<int>::size_type step_row = i % local_count;
                std::vector<int>::size_type step_column = (i / local_count) * column_count_matrix2;

                send_vector_b[i] = matrix2[step_matrix2 + step_row + step_column];
            }

            MPI_Send(matrix1.data() + step_matrix1, static_cast<int>(local_count * column_count_matrix1),
                MPI_INT, proc, 1, MPI_COMM_WORLD);
            MPI_Send(send_vector_b.data(), static_cast<int>(send_vector_b.size()),
                MPI_INT, proc, 2, MPI_COMM_WORLD);
        }
    }
    std::vector<int>::size_type local_size = (local_count + remaining) * column_count_matrix1;

    std::vector<int> local_vector_a(local_size, 0);
    std::vector<int> local_vector_b(local_size + 1, 0);

    if (rank == 0) {
        for (std::vector<int>::size_type i = 0; i < local_size; i++) {
            std::vector<int>::size_type step_row = i % (local_count + remaining);
            std::vector<int>::size_type step_column = (i / (local_count + remaining)) * column_count_matrix2;
            local_vector_b[i] = matrix2[step_row + step_column];

            local_vector_a[i] = matrix1[i];
        }
    }
    else {
        MPI_Status status;
        MPI_Recv(local_vector_a.data(), static_cast<int>(local_size - remaining * column_count_matrix1),
            MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
        MPI_Recv(local_vector_b.data(), static_cast<int>(local_size - remaining * column_count_matrix1),
            MPI_INT, 0, 2, MPI_COMM_WORLD, &status);
    }
    local_vector_b[local_size] = rank == 0 ? 0 : rank * local_count + remaining;  // step from 0-column

    std::vector<int> local_res(row_count_matrix1 * column_count_matrix2, 0);

    int sender = (rank + 1) % size;
    int reciever = (rank - 1) < 0 ? size - 1 : rank - 1;

    for (int i = 0; i < size; i++) {
        std::vector<int>::size_type local_row_count_matrix1 = rank == 0 ? local_count + remaining : local_count;
        std::vector<int>::size_type local_column_count_matrix2 =
            (rank + i) % size == 0 ? local_count + remaining : local_count;

        std::vector<int> tmp_res = getSequentialOperations(local_vector_a, local_vector_b,
            local_row_count_matrix1, column_count_matrix1, local_column_count_matrix2);

        std::vector<int>::size_type step_matrix = rank == 0 ? 0 : (rank * local_count + remaining) * row_count_matrix1;
        for (std::vector<int>::size_type j = 0; j < tmp_res.size(); j++) {
            std::vector<int>::size_type step_row = local_vector_b[local_size] + j % local_column_count_matrix2;
            std::vector<int>::size_type step_column = (j / local_column_count_matrix2) * row_count_matrix1;

            local_res[step_matrix + step_row + step_column] += tmp_res[j];
        }

        MPI_Send(local_vector_b.data(), static_cast<int>(local_vector_b.size()),
            MPI_INT, reciever, i, MPI_COMM_WORLD);

        MPI_Status status;
        MPI_Recv(local_vector_b.data(), static_cast<int>(local_vector_b.size()),
            MPI_INT, sender, i, MPI_COMM_WORLD, &status);
    }

    std::vector<int> res(row_count_matrix1 * column_count_matrix2, 0);
    MPI_Reduce(local_res.data(), res.data(), static_cast<int>(local_res.size()),
        MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    return res;
}