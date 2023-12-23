// Copyright 2023 Pozdnyakov Vasya
#include "task_3/pozdnyakov_a_cannon_multiply/cannon_multiply.h"

std::vector<std::vector<double>> GetRandomMatrix(int size) {
    std::vector<std::vector<double>> res(size, std::vector<double>(size));
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            res[i][j] = round(((static_cast<double>(std::rand()) / RAND_MAX * 100) - 50) * 10) / 10;
        }
    }
    return res;
}

std::vector<std::vector<double>> Matrix_multiply(const std::vector<std::vector<double>>& A,
    const std::vector<std::vector<double>>& B, size_t matrix_dimension) {
    std::vector<std::vector<double>> res(matrix_dimension, std::vector<double>(matrix_dimension, 0));
    for (int i = 0; i < matrix_dimension; i++) {
        for (int j = 0; j < matrix_dimension; j++) {
            for (int k = 0; k < matrix_dimension; k++) {
                res[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    return res;
}

std::vector<std::vector<double>> Cannon_multiply(const std::vector<std::vector<double>>& A,
    const std::vector<std::vector<double>>& B, size_t matrix_dimension) {
    boost::mpi::communicator world;
    int proc_rank = world.rank();
    int sqrt_proc_num = 0;

    while (sqrt_proc_num * sqrt_proc_num <= world.size())
        sqrt_proc_num++;
    sqrt_proc_num--;

    int used_matrix_dimension = matrix_dimension % sqrt_proc_num == 0 ? matrix_dimension : matrix_dimension
        + (sqrt_proc_num - matrix_dimension % sqrt_proc_num);
    std::vector<int> local_matrix_sizes(world.size(), (used_matrix_dimension / sqrt_proc_num)
        * (used_matrix_dimension / sqrt_proc_num));
    for (int i = sqrt_proc_num * sqrt_proc_num; i < world.size(); i++)
        local_matrix_sizes[i] = 0;
    std::vector<double> local_A(local_matrix_sizes[proc_rank]), local_B(local_matrix_sizes[proc_rank]),
        gather_result(used_matrix_dimension * used_matrix_dimension);
    int local_matrix_dimension = sqrt(local_matrix_sizes[proc_rank]);

    if (proc_rank == 0) {
        std::vector<double> A1(used_matrix_dimension * used_matrix_dimension);
        std::vector<double> B1(used_matrix_dimension * used_matrix_dimension);

        for (int x = 0; x < sqrt_proc_num; x++) {
            for (int y = 0; y < sqrt_proc_num; y++) {
                for (int i = 0; i < local_matrix_dimension; i++) {
                    for (int j = 0; j < local_matrix_dimension; j++) {
                        int k = (x + y) % sqrt_proc_num;
                        int new_index = (x * sqrt_proc_num + y) * local_matrix_dimension *
                            local_matrix_dimension + (i * local_matrix_dimension + j);
                        int old_x_A = x * local_matrix_dimension + i;
                        int old_y_A = k * local_matrix_dimension + j;
                        int old_x_B = k * local_matrix_dimension + i;
                        int old_y_B = y * local_matrix_dimension + j;
                        if (old_x_A < matrix_dimension && old_y_A < matrix_dimension) {
                            A1[new_index] = A[old_x_A][old_y_A];
                        } else {
                            A1[new_index] = 0.0;
                        }
                        if (old_x_B < matrix_dimension && old_y_B < matrix_dimension) {
                            B1[new_index] = B[old_x_B][old_y_B];
                        } else {
                            B1[new_index] = 0.0;
                        }
                    }
                }
            }
        }

        boost::mpi::scatterv(world, A1, local_matrix_sizes, local_A.data(), 0);
        boost::mpi::scatterv(world, B1, local_matrix_sizes, local_B.data(), 0);

    } else {
        boost::mpi::scatterv(world, local_A.data(), local_matrix_sizes[proc_rank], 0);
        boost::mpi::scatterv(world, local_B.data(), local_matrix_sizes[proc_rank], 0);
    }

    std::vector<double> local_res(local_matrix_sizes[proc_rank], 0.0);
    if (local_matrix_dimension != 0) {
        for (int q = 0; q < sqrt_proc_num; q++) {
            for (int i = 0; i < local_matrix_dimension; i++)
                for (int j = 0; j < local_matrix_dimension; j++)
                    for (int k = 0; k < local_matrix_dimension; k++)
                        local_res[i * local_matrix_dimension + j] += local_A[i * local_matrix_dimension + k]
                            * local_B[k * local_matrix_dimension + j];
            if (sqrt_proc_num - q != 1) {
                int x = proc_rank / sqrt_proc_num;
                int y = proc_rank % sqrt_proc_num;
                int send_b = (x - 1 + sqrt_proc_num) % sqrt_proc_num * sqrt_proc_num + y;
                int send_a = x * sqrt_proc_num + (y - 1 + sqrt_proc_num) % sqrt_proc_num;
                int recv_b = (x + 1) % sqrt_proc_num * sqrt_proc_num + y;
                int recv_a = x * sqrt_proc_num + (y + 1) % sqrt_proc_num;

                world.send(send_a, 0, local_A.data(), local_matrix_sizes[proc_rank]);
                world.send(send_b, 0, local_B.data(), local_matrix_sizes[proc_rank]);
                world.recv(recv_a, 0, local_A.data(), local_matrix_sizes[proc_rank]);
                world.recv(recv_b, 0, local_B.data(), local_matrix_sizes[proc_rank]);
            }
        }
    }

    std::vector<std::vector<double>> result(matrix_dimension, std::vector<double>(matrix_dimension));
    if (world.rank() != 0) {
        boost::mpi::gatherv(world, local_res, 0);
    } else {
        boost::mpi::gatherv(world, local_res, gather_result.data(), local_matrix_sizes, 0);

        int old_index, new_x, new_y;
        for (int x = 0; x < sqrt_proc_num; x++) {
            for (int y = 0; y < sqrt_proc_num; y++) {
                for (int i = 0; i < used_matrix_dimension / sqrt_proc_num; i++) {
                    for (int j = 0; j < used_matrix_dimension / sqrt_proc_num; j++) {
                        old_index = (x * sqrt_proc_num + y) * local_matrix_dimension *
                            local_matrix_dimension + (i * local_matrix_dimension + j);
                        new_x = x * local_matrix_dimension + i;
                        new_y = y * local_matrix_dimension + j;
                        if (new_x < matrix_dimension && new_y < matrix_dimension) {
                            result[new_x][new_y] = gather_result[old_index];
                        }
                    }
                }
            }
        }
    }

    world.barrier();
    return result;
}
