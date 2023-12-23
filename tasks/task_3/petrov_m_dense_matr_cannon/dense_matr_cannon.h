//  Copyright 2023 Petrov Maksim

#ifndef TASKS_TASK_3_PETROV_M_DENSE_MATR_CANNON_DENSE_MATR_CANNON_H_
#define TASKS_TASK_3_PETROV_M_DENSE_MATR_CANNON_DENSE_MATR_CANNON_H_
#include <vector>

void generate_rand_matrix(std::vector<double>* matrix,
    std::vector<double>::size_type size,
    int left_dorder, int right_border);
int calculate_matrix_size(int old_size,
    std::vector<double>::size_type matrix_size);
void getSequentialOperations(const std::vector<double>& first_matrix,
    const std::vector<double>& second_matrix,
    std::vector<double>* answer,
    std::vector<double>::size_type size);
std::vector<double> getParallelOperations(const std::vector<double>& first_matrix,
    const std::vector<double>& second_matrix,
    std::vector<double>::size_type size);

#endif  // TASKS_TASK_3_PETROV_M_DENSE_MATR_CANNON_DENSE_MATR_CANNON_H_
