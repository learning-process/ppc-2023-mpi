// Copyright 2023 Pozdnyakov Vasya
#include "task_2/pozdnyakov_a_allreduce/allreduce.h"
int local_reduce(const void *sendbuf, void *recvbuf, int count,
    MPI_Datatype datatype, MPI_Op op) {
        if (datatype == MPI_CHAR) {
            return operation_int<char>(sendbuf, recvbuf, count, op);
        } else if (datatype == MPI_UNSIGNED_CHAR) {
            return operation_int<unsigned char>(sendbuf, recvbuf,
            count, op);
        } else if (datatype ==  MPI_SHORT) {
            return operation_int<int16_t>(sendbuf, recvbuf, count, op);
        } else if (datatype ==  MPI_UNSIGNED_SHORT) {
            return operation_int<uint16_t>(sendbuf, recvbuf,
            count, op);
        } else if (datatype ==  MPI_INT) {
            return operation_int<int>(sendbuf, recvbuf, count, op);
        } else if (datatype ==  MPI_UNSIGNED) {
            return operation_int<unsigned int>(sendbuf, recvbuf,
            count, op);
        } else if (datatype ==  MPI_LONG) {
            return operation_int<int32_t>(sendbuf, recvbuf, count, op);
        } else if (datatype ==  MPI_UNSIGNED_LONG) {
            return operation_int<uint32_t>(sendbuf, recvbuf,
            count, op);
        } else if (datatype ==  MPI_LONG_LONG) {
            return operation_int<int64_t>(sendbuf, recvbuf, count, op);
        } else if (datatype ==  MPI_UNSIGNED_LONG_LONG) {
            return operation_int<uint64_t>(sendbuf, recvbuf,
            count, op);
        } else if (datatype ==  MPI_FLOAT) {
            return operation<float>(sendbuf, recvbuf, count, op);
        } else if (datatype ==  MPI_DOUBLE) {
            return operation<double>(sendbuf, recvbuf, count, op);
        } else {
            return MPI_ERR_TYPE;
        }
    }

int allreduce(const void *sendbuf, void *recvbuf, int count,
    MPI_Datatype datatype, MPI_Op op, MPI_Comm comm) {
    if (sendbuf == nullptr || recvbuf == nullptr) return MPI_ERR_BUFFER;

    if (count <= 0) return MPI_ERR_COUNT;

    if (!(datatype == MPI_INT || datatype == MPI_FLOAT
        || datatype == MPI_DOUBLE || datatype == MPI_CHAR
        || datatype == MPI_UNSIGNED_CHAR || datatype == MPI_UNSIGNED
        || datatype == MPI_UNSIGNED_SHORT || datatype == MPI_UNSIGNED_LONG
        || datatype == MPI_LONG || datatype == MPI_SHORT
        || datatype == MPI_LONG_LONG
        || datatype == MPI_UNSIGNED_LONG_LONG)) return MPI_ERR_TYPE;

    if (!(op == MPI_MAX || op == MPI_MIN || op == MPI_SUM
        || op == MPI_PROD || op == MPI_LAND || op == MPI_LOR
        || op == MPI_LXOR || op == MPI_BXOR || op == MPI_BOR
        || op == MPI_BAND)) return MPI_ERR_OP;

    if (comm == MPI_COMM_NULL) return MPI_ERR_COMM;

    int rank = 0;
    int world_size = 0;

    MPI_Comm_rank(comm, &rank);
    MPI_Comm_size(comm, &world_size);

    int size_of_type = 0;
    MPI_Type_size(datatype, &size_of_type);
    size_t size_of_buf = static_cast<size_t>(count * size_of_type);
    std::memcpy(recvbuf, sendbuf, size_of_buf);

    int parent_num = (rank - 1) / 2;
    int left_son_num = rank == 0 ? 1 : 2 * rank + 1;
    int right_son_num = rank == 0 ? 2 : 2 * rank + 2;
    void* left_buf = nullptr;
    void* right_buf = nullptr;

    if (left_son_num < world_size) {
        left_buf = std::malloc(size_of_buf);
        MPI_Recv(left_buf, count, datatype, left_son_num, 0,
            comm, MPI_STATUS_IGNORE);
        local_reduce(left_buf, recvbuf, count, datatype, op);
        free(left_buf);
    }
    if (right_son_num < world_size) {
        right_buf = std::malloc(size_of_buf);
        MPI_Recv(right_buf, count, datatype, right_son_num, 0,
            comm, MPI_STATUS_IGNORE);
        local_reduce(right_buf, recvbuf, count, datatype, op);
        free(right_buf);
    }

    if (rank != 0) {
        MPI_Send(recvbuf, count, datatype, parent_num, 0, comm);
        MPI_Recv(recvbuf, count, datatype, parent_num, 0,
            comm, MPI_STATUS_IGNORE);
    }
    if (left_son_num < world_size) {
        MPI_Send(recvbuf, count, datatype, left_son_num, 0, comm);
    }
    if (right_son_num < world_size) {
        MPI_Send(recvbuf, count, datatype, right_son_num, 0, comm);
    }

    MPI_Barrier(comm);

    return MPI_SUCCESS;
}
