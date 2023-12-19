// Copyright 2023 Kruglov Alexey
#include "task_2/kruglov_a_allreduce/allreduce.h"

std::vector<int> getRandomVector(int sz) {
    std::random_device dev;
    std::mt19937 gen(dev());
    std::vector<int> vec(sz);
    for (int i = 0; i < sz; i++) { vec[i] = gen() % 100; }
    return vec;
}

std::vector<float> getRandomFloatVector(int sz) {
    std::random_device dev;
    std::mt19937 gen(dev());
    std::vector<float> vec(sz);
    for (int i = 0; i < sz; i++) { vec[i] = static_cast<float>(gen()) / INT_MAX; }
    return vec;
}

std::vector<double> getRandomDoubleVector(int sz) {
    std::random_device dev;
    std::mt19937 gen(dev());
    std::vector<double> vec(sz);
    for (int i = 0; i < sz; i++) { vec[i] = static_cast<double>(gen()) / INT_MAX; }
    return vec;
}

template <typename T>
static inline int doMPIOperations(T* store_buf, const T* temp_buf, int count, MPI_Op op) {
    int ret = MPI_SUCCESS;
    if (op == MPI_MIN) {
        for (int i = 0; i < count; ++i) {
            store_buf[i] = std::min(store_buf[i], temp_buf[i]);
        }
    } else if (op == MPI_MAX) {
        for (int i = 0; i < count; ++i) {
            store_buf[i] = std::max(store_buf[i], temp_buf[i]);
        }
    } else if (op == MPI_SUM) {
        for (int i = 0; i < count; ++i) {
            store_buf[i] = store_buf[i] + temp_buf[i];
        }
    } else if (op == MPI_PROD) {
        for (int i = 0; i < count; ++i) {
            store_buf[i] = store_buf[i] * temp_buf[i];
        }
    } else {
        ret = MPI_ERR_OP;
    }
    return ret;
}

static inline int calculate(void* a, const void* b, int count, MPI_Datatype type, MPI_Op op) {
    int ret = MPI_SUCCESS;
    if (type == MPI_CHAR) {
        doMPIOperations<char>(reinterpret_cast<char*>(a), reinterpret_cast<const char*>(b), count, op);
    } else if (type == MPI_SHORT) {
        doMPIOperations<int16_t>(reinterpret_cast<int16_t*>(a), reinterpret_cast<const int16_t*>(b), count, op);
    } else if (type == MPI_LONG) {
        doMPIOperations<int32_t>(reinterpret_cast<int32_t*>(a), reinterpret_cast<const int32_t*>(b), count, op);
    } else if (type == MPI_INT) {
        doMPIOperations<int>(reinterpret_cast<int*>(a), reinterpret_cast<const int*>(b), count, op);
    } else if (type == MPI_UNSIGNED_CHAR) {
        doMPIOperations<uint8_t>(reinterpret_cast<uint8_t*>(a), reinterpret_cast<const uint8_t*>(b), count, op);
    } else if (type == MPI_UNSIGNED_SHORT) {
        doMPIOperations<uint16_t>(reinterpret_cast<uint16_t*>(a), reinterpret_cast<const uint16_t*>(b), count, op);
    } else if (type == MPI_UNSIGNED) {
        doMPIOperations<unsigned>(reinterpret_cast<unsigned*>(a), reinterpret_cast<const unsigned*>(b), count, op);
    } else if (type == MPI_UNSIGNED_LONG) {
        doMPIOperations<uint32_t>(reinterpret_cast<uint32_t*>(a), reinterpret_cast<const uint32_t*>(b), count, op);
    } else if (type == MPI_FLOAT) {
        doMPIOperations<float>(reinterpret_cast<float*>(a), reinterpret_cast<const float*>(b), count, op);
    } else if (type == MPI_DOUBLE) {
        doMPIOperations<double>(reinterpret_cast<double*>(a), reinterpret_cast<const double*>(b), count, op);
    } else if (type == MPI_LONG_DOUBLE) {
        doMPIOperations<long double>(reinterpret_cast<long double*>(a),
            reinterpret_cast<const long double*>(b), count, op);
    } else {
        ret = MPI_ERR_TYPE;
    }
    return ret;
}

int myAllreduce(const void* send_buf, void* recv_buf, int count, MPI_Datatype datatype, MPI_Op op, MPI_Comm comm) {
    int ret = MPI_SUCCESS;
    if (count <= 0) {
        ret = MPI_ERR_COUNT;
        MPI_Abort(comm, ret);
        return ret;
    }

    if (send_buf == nullptr || recv_buf == nullptr) {
        ret = MPI_ERR_BUFFER;
        MPI_Abort(comm, ret);
        return ret;
    }

    int size, rank;
    MPI_Comm_size(comm, &size);
    MPI_Comm_rank(comm, &rank);

    int elem_size;
    MPI_Type_size(datatype, &elem_size);

    MPI_Status status;

    if (rank == 0) {
        void* temp_buf = std::malloc(count * elem_size);
        std::memcpy(recv_buf, send_buf, count * elem_size);
        for (int i = 1; i < size; ++i) {
            MPI_Recv(temp_buf, count, datatype, i, 0, comm, &status);
            calculate(recv_buf, temp_buf, count, datatype, op);
        }
        free(temp_buf);
        for (int i = 1; i < size; ++i) {
            MPI_Send(recv_buf, count, datatype, i, 0, comm);
        }
    } else {
        MPI_Send(send_buf, count, datatype, 0, 0, comm);
        MPI_Recv(recv_buf, count, datatype, 0, 0, comm, &status);
    }

    return ret;
}
