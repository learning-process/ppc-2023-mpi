// Copyright 2023 Podyachikh Mikhail
#ifndef TASKS_TASK_2_PODYACHIKH_M_REDUCE_REDUCE_H_
#define TASKS_TASK_2_PODYACHIKH_M_REDUCE_REDUCE_H_

#include <vector>
#include <random>

#define RETURN_IF_ERR(VALUE) { \
    int return_code = VALUE;   \
    if (return_code)           \
        return return_code;    \
}

template<typename T>
void FillRandom(T *buffer, int size, T min_value, T max_value) {
    for (int i = 0; i < size; i++) {
        std::default_random_engine re;
        std::uniform_real_distribution<double> rnd(0, 1);
        buffer[i] = min_value + (max_value - min_value) * rnd(re);
    }
}

int reduce(const void* sendbuf, void* recvbuf, int count, MPI_Datatype datatype, MPI_Op op, int root, MPI_Comm comm);

#endif  // TASKS_TASK_2_PODYACHIKH_M_REDUCE_REDUCE_H_
