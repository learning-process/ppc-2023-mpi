 /*  Copyright 2023 Petrov Maksim  */
#include <mpi.h>
#include <vector>
#include <string>
#include <random>
#include <algorithm>
#include "task_3/petrov_m_dense_matr_cannon/dense_matr_cannon.h"

void generate_rand_matrix(std::vector<double>* matrix, std::vector<double>::size_type size,
    int left_border, int right_border) {
    std::random_device dev;
    std::mt19937 gen(dev());
    std::uniform_real_distribution<> urd(left_border, right_border);
    for (std::vector<double>::size_type i = 0; i < size * size; i++) {
        (*matrix)[i] = urd(gen);
    }
}

//  to split the matrix into blocks
int calculate_matrix_size(int old_size, std::vector<double>::size_type matrix_size) {
    int sqrt_size = static_cast<int>(sqrt(old_size));
    while ((matrix_size % sqrt_size) && sqrt_size > 1)
        --sqrt_size;

    return sqrt_size * sqrt_size;
}

void getSequentialOperations(const std::vector<double>& first_matrix, const std::vector<double>& second_matrix,
    std::vector<double>* result,
    std::vector<double>::size_type size) {
    for (std::vector<double>::size_type i = 0; i < size; ++i) {
        for (std::vector<double>::size_type j = 0; j < size; ++j) {
            for (std::vector<double>::size_type k = 0; k < size; ++k) {
                (*result)[i * size + j] += first_matrix[i * size + k] * second_matrix[j + k * size];
            }
        }
    }
}


std::vector<double> getParallelOperations(const std::vector<double>& first_matrix,
    const std::vector<double>& second_matrix,
    std::vector<double>::size_type matrix_size) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int actual_size = calculate_matrix_size(size, matrix_size);

    MPI_Group old_group, new_group;
    MPI_Comm_group(MPI_COMM_WORLD, &old_group);

    std::vector<int> actual_proc_vector(actual_size);
    for (int i = 0; i < actual_size; ++i)
        actual_proc_vector[i] = i;

    MPI_Comm actual_proc;
    MPI_Group_incl(old_group, actual_size, actual_proc_vector.data(), &new_group);
    MPI_Comm_create(MPI_COMM_WORLD, new_group, &actual_proc);

    std::vector<double> global_res(matrix_size * matrix_size, 0.0);
    if (size <= 3 || actual_size <= 3) {
        if (rank == 0)
            getSequentialOperations(first_matrix, second_matrix, &global_res, matrix_size);

        return global_res;
    }

    if (rank >= actual_size)
        return global_res;

    int count_parts = static_cast<int>(sqrt(actual_size));
    std::vector<double>::size_type local_size = matrix_size / count_parts;   //  the size of each block

    if (rank == 0) {
        for (int proc = 1; proc < actual_size; ++proc) {
            std::vector<double>::size_type step = (proc / count_parts) * matrix_size * local_size +
                proc % count_parts * local_size;

            std::vector<double> part_first_matrix(local_size * local_size, 0.0);
            std::vector<double> part_second_matrix(local_size * local_size, 0.0);

            for (std::vector<double>::size_type i = 0; i < local_size * local_size; ++i) {
                std::vector<double>::size_type global_offset = matrix_size * (i / local_size);
                std::vector<double>::size_type local_offset = i % local_size;

                part_first_matrix[i] = first_matrix[step + local_offset + global_offset];
                part_second_matrix[i] = second_matrix[step + local_offset + global_offset];
            }

            MPI_Send(part_first_matrix.data(), static_cast<int>(local_size * local_size),
                MPI_DOUBLE, proc, 1, actual_proc);
            MPI_Send(part_second_matrix.data(), static_cast<int>(local_size * local_size),
                MPI_DOUBLE, proc, 2, actual_proc);
        }
    }

    std::vector<double> local_first_matrix(local_size * local_size, 0.0);
    std::vector<double> local_second_matrix(local_size * local_size, 0.0);

    if (rank == 0) {
        for (std::vector<double>::size_type i = 0; i < local_size * local_size; ++i) {
            //  to calculate the initial index of each block
            std::vector<double>::size_type global_offset = matrix_size * (i / local_size);
            //  to calculate the indexes of the elements inside each block
            std::vector<double>::size_type local_offset = i % local_size;

            local_first_matrix[i] = first_matrix[local_offset + global_offset];
            local_second_matrix[i] = second_matrix[local_offset + global_offset];
        }
    } else {
        MPI_Status status;
        MPI_Recv(local_first_matrix.data(), static_cast<int>(local_size * local_size),
            MPI_DOUBLE, 0, 1, actual_proc, &status);
        MPI_Recv(local_second_matrix.data(), static_cast<int>(local_size * local_size),
            MPI_DOUBLE, 0, 2, actual_proc, &status);
    }

    int proc_number_row = rank / count_parts;   //  line number in the process grid
    int proc_number_col = rank % count_parts;

     //  initial recipients and senders (communication pairs between processes)
    int init_recipient_a;
    if ((rank - proc_number_row) / count_parts == proc_number_row && rank - proc_number_row >= 0) {
        init_recipient_a = rank - proc_number_row;
    } else {
        init_recipient_a = rank + count_parts - proc_number_row;
    }
    int init_recipient_b;
    if (rank - proc_number_col * count_parts >= 0) {
        init_recipient_b = rank - proc_number_col * count_parts;
    } else {
        init_recipient_b = actual_size + (rank - proc_number_col * count_parts);
    }
    int init_sender_a;
    if ((rank + proc_number_row) / count_parts == proc_number_row) {
        init_sender_a = rank + proc_number_row;
    } else {
        init_sender_a = rank - count_parts + proc_number_row;
    }

    int init_sender_b;
    if (rank + proc_number_col * count_parts < actual_size) {
        init_sender_b = rank + proc_number_col * count_parts;
    } else {
        init_sender_b = (rank + proc_number_col * count_parts) - actual_size;
    }

    std::vector<double> local_res_vector(local_size * local_size, 0.0);
    //  for temporary storage of partial results
    std::vector<double> local_res_matrix(matrix_size * matrix_size, 0.0);
    //  for the final result of the multiplication

    MPI_Status status;

     //  data exchange by rows and columns in the process grid
    if (proc_number_row > 0) {
        MPI_Sendrecv_replace(local_first_matrix.data(), static_cast<int>(local_size * local_size),
            MPI_DOUBLE, init_recipient_a, 1, init_sender_a, 1, actual_proc, &status);
    }

    if (proc_number_col > 0) {
        MPI_Sendrecv_replace(local_second_matrix.data(), static_cast<int>(local_size * local_size),
            MPI_DOUBLE, init_recipient_b, 2, init_sender_b, 2, actual_proc, &status);
    }
      //  To exchange data with the nearest processes in the process grid
      //  (to complete the multiplication of blocks)
    int recipient_a;
    if ((rank - 1) / count_parts == proc_number_row && rank - 1 >= 0) {
        recipient_a = rank - 1;
    } else {
        recipient_a = rank + count_parts - 1;
    }
    int recipient_b;
    if (rank - count_parts >= 0) {
        recipient_b = rank - count_parts;
    } else {
        recipient_b = actual_size + (rank - count_parts);
    }
    int sender_a;
    if ((rank + 1) / count_parts == proc_number_row) {
        sender_a = rank + 1;
    } else {
        sender_a = rank - count_parts + 1;
    }
    int sender_b;
    if (rank + count_parts < actual_size) {
        sender_b = rank + count_parts;
    } else {
        sender_b = (rank + count_parts) - actual_size;
    }

    for (int i = 0; i < count_parts; ++i) {
        getSequentialOperations(local_first_matrix, local_second_matrix, &local_res_vector, local_size);

        MPI_Sendrecv_replace(local_first_matrix.data(), static_cast<int>(local_size * local_size),
            MPI_DOUBLE, recipient_a, 1, sender_a, 1, actual_proc, &status);
        MPI_Sendrecv_replace(local_second_matrix.data(), static_cast<int>(local_size * local_size),
            MPI_DOUBLE, recipient_b, 2, sender_b, 2, actual_proc, &status);
    }

    for (std::vector<double>::size_type i = 0; i < local_res_vector.size(); i++) {
        std::vector<double>::size_type row = (rank / count_parts) * local_size + i / local_size;
        std::vector<double>::size_type column = (rank % count_parts) * local_size + i % local_size;

        local_res_matrix[row * matrix_size + column] = local_res_vector[i];
    }

    MPI_Reduce(local_res_matrix.data(), global_res.data(), static_cast<int>(matrix_size * matrix_size),
        MPI_DOUBLE, MPI_SUM, 0, actual_proc);

    return global_res;
}
