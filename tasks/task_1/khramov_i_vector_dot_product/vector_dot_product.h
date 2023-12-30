// Copyright 2023 Khramov Ivan
#ifndef TASKS_TASK_1_KHRAMOV_I_VECTOR_DOT_PRODUCT_VECTOR_DOT_PRODUCT_H_
#define TASKS_TASK_1_KHRAMOV_I_VECTOR_DOT_PRODUCT_VECTOR_DOT_PRODUCT_H_

#include <vector>
#include <string>

std::vector<int> getRandomVector(int  sz);
std::vector<int> getLocalVectorSizes(
    int comm_size,
    int global_vec_size);
int getSequentialVectorDotProduct(
    const std::vector<int>& vec1,
    const std::vector<int>& vec2);
int getParallelVectorDotProduct(
    boost::mpi::communicator world,
    int count_size_vector,
    const std::vector<int>& global_vec1,
    const std::vector<int>& global_vec2);

#endif  // TASKS_TASK_1_KHRAMOV_I_VECTOR_DOT_PRODUCT_VECTOR_DOT_PRODUCT_H_
