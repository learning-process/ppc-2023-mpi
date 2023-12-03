// Copyright 2023 Saratova Marina
#include "task_1/saratova_m_min_elem_vector/min_elem_vector.h"

std::vector<int> generate_random_vector(int size, int low, int high) {
    std::vector<int> randomVector;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(low, high);

    for (int i = 0; i < size; ++i) {
        randomVector.push_back(dis(gen));
    }
    return randomVector;
}

int seq_find_min_elem_vector(const std::vector<int>& vec) {
  int min = INT_MAX;
  if (vec.size() == 0) {
    return 0;
  }
  for (int i = 0; i < vec.size(); i++) {
    if (vec[i] < min) {
      min = vec[i];
    }
  }
  return min;
}

int par_find_min_elem_vector(const std::vector<int>& vec) {
    int rank = 0, size = 0;

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0 && vec.size() == 0) {
        return 0;
    }

    std::vector<int> counts(size);
    int elements_per_proc = vec.size() / size;
    int remainder = vec.size() % size;

    for (int i = 0; i < size; ++i) {
        counts[i] = elements_per_proc + (i < remainder ? 1 : 0);
    }

    std::vector<int> displs(size);
    displs[0] = 0;
    for (int i = 1; i < size; ++i) {
        displs[i] = displs[i - 1] + counts[i - 1];
    }

    int local_size = counts[rank];
    std::vector<int> local_vec(local_size);

    MPI_Scatterv(vec.data(), counts.data(), displs.data(), MPI_INT,
     local_vec.data(), local_size, MPI_INT, 0, MPI_COMM_WORLD);

    int local_min = INT_MAX;
    for (int i = 0; i < local_vec.size(); i++) {
        if (local_vec[i] < local_min) {
            local_min = local_vec[i];
        }
    }

    int global_min = INT_MAX;
    MPI_Reduce(&local_min, &global_min, 1, MPI_INT, MPI_MIN, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        return global_min;
    }

    return 0;
}
