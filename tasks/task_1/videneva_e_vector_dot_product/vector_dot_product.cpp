// Copyright 2023 Videneva Ekaterina

#include "task_1/videneva_e_vector_dot_product/vector_dot_product.h"

int generateRandomNumbers(int min, int max) {
  int randomNumber = min + (std::rand() % (max - min + 1));

  return randomNumber;
}

std::vector<int> getRandomVector(int sz) {
    std::random_device dev;
    std::mt19937 gen(dev());
    std::vector<int> vec(sz);
    for (int  i = 0; i < sz; i++) { vec[i] = gen() % 100; }
    return vec;
}

int scalar_product(const std::vector<int>& vec1, const std::vector<int>& vec2) {
    if (vec1.size() != vec2.size()) {
        return 1;
    }
    int result = 0;
    for (size_t i = 0; i < vec1.size(); ++i) {
        result += vec1[i] * vec2[i];
    }
    return result;
}

int Parall_product(const std::vector<int> &vec1, const std::vector<int> &vec2) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (vec1.size() != vec2.size()) {
        return 1;
    }

    int n = vec1.size() / size;
    int remainder = vec1.size() % size;

    std::vector<int> local_vec1(n + (rank < remainder ? 1 : 0));
    std::vector<int> local_vec2(n + (rank < remainder ? 1 : 0));

    int offset = rank * n + std::min(rank, remainder);
    for (int i = 0; i < local_vec1.size(); ++i) {
        local_vec1[i] = vec1[offset + i];
        local_vec2[i] = vec2[offset + i];
    }

    int local_result = scalar_product(local_vec1, local_vec2);

    int result = 0;
    MPI_Reduce(&local_result, &result, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    // if (rank == 0) {
    //     std::cout << "Scalar product: " << result << std::endl;
    // }
    return result;
}
