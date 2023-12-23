  // Copyright 2023 Vetoshnikova Ekaterina

#include "task_1/vetoshnikova_k_vector_dot_product/vector_dot_product.h"

std::vector<int> vector_generation(int mn, int mx, int sz) {
    std::vector<int> res(sz);
    for (int i = 0; i < sz; i++) {
        res[i] = mn + (std::rand() % (mx - mn + 1));
    }

    return res;
}

int getSequentialOperations(std::vector<int> a, std::vector<int> b,
    int count_size_vector) {
    int res = 0;
    for (int i = 0; i < count_size_vector; i++) {
        res += a[i] * b[i];
    }

    return res;
}

int getParallelOperations(std::vector<int> global_vec_a,
    std::vector<int> global_vec_b, int count_size_vector) {
    int ProcNum;
    int ProcRank;
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    std::vector<int> counts_element(ProcNum);
    std::vector<int> dis(ProcNum);
    int delta     = count_size_vector / ProcNum;
    int remainder = count_size_vector % ProcNum;

    for (int i = 0; i < ProcNum; i++) {
        counts_element[i] = delta;
        if (i < remainder) {
            counts_element[i]++;
        }
        if (i > 0) dis[i] = dis[i - 1] + counts_element[i - 1];
        else
            dis[i] = 0;
    }
    std::vector<int> a_local(counts_element[ProcRank]);
    std::vector<int> b_local(counts_element[ProcRank]);
    MPI_Scatterv(global_vec_a.data(), counts_element.data(), dis.data(), MPI_INT,
        a_local.data(), counts_element[ProcRank], MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Scatterv(global_vec_b.data(), counts_element.data(), dis.data(), MPI_INT,
        b_local.data(), counts_element[ProcRank], MPI_INT, 0, MPI_COMM_WORLD);
    int sum = 0;
    int sum_all = 0;
    sum = getSequentialOperations(a_local, b_local, counts_element[ProcRank]);
    MPI_Reduce(&sum, &sum_all, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    return sum_all;
}




