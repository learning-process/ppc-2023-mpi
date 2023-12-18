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
static inline void doMPIOperations(T* store_buf, const T* temp_buf, int count, MPI_Op op) {
    switch (op) {
    case(MPI_MIN): {
        for (int i = 0; i < count; ++i) {
            store_buf[i] = std::min(store_buf[i], temp_buf[i]);
        }
        break;
    }
    case(MPI_MAX): {
        for (int i = 0; i < count; ++i) {
            store_buf[i] = std::max(store_buf[i], temp_buf[i]);
        }
        break;
    }
    case(MPI_SUM): {
        for (int i = 0; i < count; ++i) {
            store_buf[i] = store_buf[i] + temp_buf[i];
        }
        break;
    }
    case(MPI_PROD): {
        for (int i = 0; i < count; ++i) {
            store_buf[i] = store_buf[i] * temp_buf[i];
        }
        break;
    }
    }
}

static inline void calculate(void* a, const void* b, int count, MPI_Datatype type, MPI_Op op) {
    switch (type) {
    case (MPI_CHAR): {
            doMPIOperations<char>(static_cast<char*>(a), static_cast<const char*>(b), count, op);
            break;
        }
    case (MPI_SHORT): {
            doMPIOperations<int16_t>(static_cast<int16_t*>(a), static_cast<const int16_t*>(b), count, op);
            break;
        }
    case (MPI_LONG): {
            doMPIOperations<int32_t>(static_cast<int32_t*>(a), static_cast<const int32_t*>(b), count, op);
            break;
        }
    case (MPI_INT): {
            doMPIOperations<int>(static_cast<int*>(a), static_cast<const int*>(b), count, op);
            break;
        }
    case (MPI_UNSIGNED_CHAR): {
            doMPIOperations<uint8_t>(static_cast<uint8_t*>(a), static_cast<const uint8_t*>(b), count, op);
            break;
        }
    case (MPI_UNSIGNED_SHORT): {
            doMPIOperations<uint16_t>(static_cast<uint16_t*>(a), static_cast<const uint16_t*>(b), count, op);
            break;
        }
    case (MPI_UNSIGNED): {
            doMPIOperations<unsigned>(static_cast<unsigned*>(a), static_cast<const unsigned*>(b), count, op);
            break;
        }
    case (MPI_UNSIGNED_LONG): {
            doMPIOperations<uint32_t>(static_cast<uint32_t*>(a), static_cast<const uint32_t*>(b), count, op);
            break;
        }
    case (MPI_FLOAT): {
            doMPIOperations<float>(static_cast<float*>(a), static_cast<const float*>(b), count, op);
            break;
        }
    case (MPI_DOUBLE): {
            doMPIOperations<double>(static_cast<double*>(a), static_cast<const double*>(b), count, op);
            break;
        }
    case (MPI_LONG_DOUBLE): {
            doMPIOperations<long double>(static_cast<long double*>(a), static_cast<const long double*>(b), count, op);
            break;
        }
    }
}

int myAllreduce(const void* send_buf, void* recv_buf, int count, MPI_Datatype datatype, MPI_Op op, MPI_Comm comm) {
    int ret = MPI_SUCCESS;
    if (count <= 0) {
        ret = MPI_ERR_BUFFER;
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

    void* temp_buf = std::malloc(count * elem_size);
    MPI_Status status;

    memcpy(recv_buf, send_buf, count * elem_size);

    int iterpid = 0;

    for (; iterpid != rank; ++iterpid) {
        MPI_Recv(temp_buf, count, datatype, iterpid, 0, comm, &status);
        calculate(recv_buf, temp_buf, count, datatype, op);
    }

        ++iterpid;

    for (int pid = 0; pid < size; ++pid) {
        if (!(pid == rank)) {
            MPI_Send(send_buf, count, datatype, pid, 0, comm);
        }
    }

    for (; iterpid < size; ++iterpid) {
        MPI_Recv(temp_buf, count, datatype, iterpid, 0, comm, &status);
        calculate(recv_buf, temp_buf, count, datatype, op);
    }

    return ret;
}
