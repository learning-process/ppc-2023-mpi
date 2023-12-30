// Copyright 2023 Skotin Alexander
#include "task_3/skotin_a_matrix_algo_cannon/matrix_algo_cannon.h"
#include <random>

std::vector<std::vector<double>> GetMatrixRandom(int size) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dis(-10.0, 100.0);

    std::vector<std::vector<double>> matrix(size, std::vector<double>(size));
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            matrix[i][j] = std::round(dis(gen) * 10.0) / 10.0;
        }
    }
    return matrix;
}

std::vector<std::vector<double>> MatrixMultiply(
    const std::vector<std::vector<double>>& matrix_1,
    const std::vector<std::vector<double>>& matrix_2, int m_size) {
    std::vector<std::vector<double>> result(m_size,
        std::vector<double>(m_size, 0));
    for (int i = 0; i < m_size; i++) {
        for (int j = 0; j < m_size; j++) {
            for (int k = 0; k < m_size; k++) {
                result[i][j] += matrix_1[i][k] * matrix_2[k][j];
            }
        }
    }
    return result;
}

std::vector<std::vector<double>> CannonMultiply(
    const std::vector<std::vector<double>>& matrix_1,
    const std::vector<std::vector<double>>& matrix_2, size_t m_size) {
    boost::mpi::communicator world_comm;
    int rank = world_comm.rank();
    int proc_sqrt = 0;

    while (proc_sqrt * proc_sqrt <= world_comm.size())
        proc_sqrt++;
    proc_sqrt--;

    int adjusted_dim = m_size % proc_sqrt == 0 ? m_size :
        m_size + (proc_sqrt - m_size % proc_sqrt);
    std::vector<int> block_sizes(world_comm.size(), (adjusted_dim /
        proc_sqrt) * (adjusted_dim / proc_sqrt));
    int idx = proc_sqrt * proc_sqrt;
    while (idx < world_comm.size()) {
        block_sizes[idx] = 0;
        idx++;
    }

    std::vector<double> local_mat1(block_sizes[rank]),
        local_mat2(block_sizes[rank]),
        result_gathered(adjusted_dim * adjusted_dim);
    int local_dim = sqrt(block_sizes[rank]);

    if (rank == 0) {
        std::vector<double> flat_A(adjusted_dim * adjusted_dim);
        std::vector<double> flat_B(adjusted_dim * adjusted_dim);
        int x = 0;
        while (x < proc_sqrt) {
            int y = 0;
            while (y < proc_sqrt) {
                for (int i = 0; i < local_dim; i++) {
                    for (int j = 0; j < local_dim; j++) {
                        int k = (x + y) % proc_sqrt;
                        int new_idx = (x * proc_sqrt + y) * local_dim *
                            local_dim + (i * local_dim + j);
                        int old_x_A = x * local_dim + i, old_y_A = k *
                            local_dim + j;
                        int old_x_B = k * local_dim + i, old_y_B = y *
                            local_dim + j;
                        flat_A[new_idx] = (old_x_A < m_size && old_y_A < m_size)
                            ? matrix_1[old_x_A][old_y_A] : 0.0;
                        flat_B[new_idx] = (old_x_B < m_size && old_y_B < m_size)
                            ? matrix_2[old_x_B][old_y_B] : 0.0;
                    }
                }
                y++;
            }
            x++;
        }

        boost::mpi::scatterv(world_comm, flat_A, block_sizes,
            local_mat1.data(), 0);
        boost::mpi::scatterv(world_comm, flat_B, block_sizes,
            local_mat2.data(), 0);
    } else {
        boost::mpi::scatterv(world_comm, local_mat1.data(),
            block_sizes[rank], 0);
        boost::mpi::scatterv(world_comm, local_mat2.data(),
            block_sizes[rank], 0);
    }

    std::vector<double> local_result(block_sizes[rank], 0.0);
    if (local_dim != 0) {
        for (int q = 0; q < proc_sqrt; q++) {
            for (int i = 0; i < local_dim; i++) {
                for (int j = 0; j < local_dim; j++) {
                    for (int k = 0; k < local_dim; k++) {
                        local_result[i * local_dim + j] +=
                            local_mat1[i * local_dim + k]
                            * local_mat2[k * local_dim + j];
                    }
                }
            }

            if (proc_sqrt - q != 1) {
                int x = rank / proc_sqrt, y = rank % proc_sqrt;
                int send_b = ((x - 1 + proc_sqrt) % proc_sqrt) * proc_sqrt + y;
                int send_a = x * proc_sqrt + ((y - 1 + proc_sqrt) % proc_sqrt);
                int recv_b = ((x + 1) % proc_sqrt) * proc_sqrt + y;
                int recv_a = x * proc_sqrt + ((y + 1) % proc_sqrt);

                world_comm.send(send_a, 0, local_mat1.data(),
                    block_sizes[rank]);
                world_comm.send(send_b, 0, local_mat2.data(),
                    block_sizes[rank]);
                world_comm.recv(recv_a, 0, local_mat1.data(),
                    block_sizes[rank]);
                world_comm.recv(recv_b, 0, local_mat2.data(),
                    block_sizes[rank]);
            }
        }
    }

    std::vector<std::vector<double>> final_result(m_size,
        std::vector<double>(m_size));
    if (world_comm.rank() != 0) {
        boost::mpi::gatherv(world_comm, local_result, 0);
    } else {
        boost::mpi::gatherv(world_comm, local_result,
            result_gathered.data(), block_sizes, 0);

        int new_x, new_y, old_idx;
        for (int x = 0; x < proc_sqrt; x++) {
            for (int y = 0; y < proc_sqrt; y++) {
                for (int i = 0; i < adjusted_dim / proc_sqrt; i++) {
                    for (int j = 0; j < adjusted_dim / proc_sqrt; j++) {
                        old_idx = (x * proc_sqrt + y) * local_dim
                            * local_dim + (i * local_dim + j);
                        new_x = x * local_dim + i;
                        new_y = y * local_dim + j;
                        if (new_x < m_size && new_y < m_size) {
                            final_result[new_x][new_y] =
                                result_gathered[old_idx];
                        }
                    }
                }
            }
        }
    }

    world_comm.barrier();
    return final_result;
}
