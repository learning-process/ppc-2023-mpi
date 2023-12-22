// Copyright 2023 Podyachikh Mikhail
#include <mpi.h>
#include <iostream>
#include <utility>
#include <algorithm>
#include "task_2/podyachikh_m_reduce/reduce.h"

template<typename T>
int CommonOperation(T *result, const T *buf, int count, MPI_Op op) {
    T (*function)(T, T);
    if (op == MPI_SUM) {
        function = [](T a, T b) -> T { return a + b; };
    } else if (op == MPI_PROD) {
        function = [](T a, T b) -> T { return a * b; };
    } else if (op == MPI_MAX) {
        function = [](T a, T b) -> T { return std::max(a, b); };
    } else if (op == MPI_MIN) {
        function = [](T a, T b) -> T { return std::min(a, b); };
    } else if (op == MPI_LAND) {
        function = [](T a, T b) -> T { return a && b; };
    } else if (op == MPI_LOR) {
        function = [](T a, T b) -> T { return a || b; };
    } else {
        return MPI_ERR_OP;
    }

    for (int i = 0; i < count; i++) {
        result[i] = function(result[i], buf[i]);
    }
    return 0;
}

template<typename T>
int OperationBitTypes(T *result, const T *buf, int count, MPI_Op op) {
    T (*function)(T, T);
    if (op == MPI_SUM) {
        function = [](T a, T b) -> T { return a + b; };
    } else if (op == MPI_PROD) {
        function = [](T a, T b) -> T { return a * b; };
    } else if (op == MPI_MAX) {
        function = [](T a, T b) -> T { return std::max(a, b); };
    } else if (op == MPI_MIN) {
        function = [](T a, T b) -> T { return std::min(a, b); };
    } else if (op == MPI_LAND) {
        function = [](T a, T b) -> T { return a && b; };
    } else if (op == MPI_LOR) {
        function = [](T a, T b) -> T { return a || b; };
    } else if (op == MPI_BAND) {
        function = [](T a, T b) -> T { return a & b; };
    } else if (op == MPI_BOR) {
        function = [](T a, T b) -> T { return a | b; };
    } else {
        return MPI_ERR_OP;
    }

    for (int i = 0; i < count; i++) {
        result[i] = function(result[i], buf[i]);
    }
    return 0;
}

template<typename T>
int OperationLoc(T *result, const T *buf, int count, MPI_Op op) {
    T (*function)(T, T);
    if (op == MPI_MINLOC) {
        function = [](T a, T b) -> T {
          if (a.first == b.first) {
              if (a.second < b.second)
                  return a;
              else
                  return b;
          }
          return min(a, b);
        };
    } else if (op == MPI_MAXLOC) {
        function = [](T a, T b) -> T {
          if (a.first == b.first) {
              if (a.second < b.second)
                  return a;
              else
                  return b;
          }
          return max(a, b);
        };
    } else {
        return MPI_ERR_OP;
    }
    for (int i = 0; i < count; i++) {
        result[i] = function(result[i], buf[i]);
    }
    return 0;
}

template<typename T>
int ReduceHelper(const void* sendbuf, void* recvbuf, int count, MPI_Datatype datatype, MPI_Op op,
                 int root, MPI_Comm comm, int (*BufferOperation)(T*, const T*, int, MPI_Op)) {
    int world_size, rank;
    RETURN_IF_ERR(MPI_Comm_size(comm, &world_size));
    RETURN_IF_ERR(MPI_Comm_rank(comm, &rank));
    rank = (rank - root + world_size) % world_size;

    MPI_Status status;
    auto result = new T[count];
    memcpy(result, sendbuf, count * sizeof(T));
    auto tmpbuf = new T[count];
    int i = 1;
    // While found first '1' in rank bites
    while (!(rank & i)) {
        int new_rank = rank | i;
        if (new_rank >= world_size) break;
        new_rank = ((rank | i) + root) % world_size;
        RETURN_IF_ERR(MPI_Recv(tmpbuf, count, datatype, new_rank, 0, comm, &status));
        RETURN_IF_ERR(BufferOperation(result, tmpbuf, count, op));
        i = i << 1;
    }
    if (rank != 0) {
        // `(rank - 1)` - set last `1` bit as `0`, the bits to the right become `1`
        // Extra `1` bits removes by `& rank`
        int new_rank = (rank - 1) & rank;
        new_rank = (new_rank + root) % world_size;
        RETURN_IF_ERR(MPI_Send(result, count, datatype, new_rank, 0, comm));
    }
    memcpy(recvbuf, result, count * sizeof(T));
    delete[] result;
    delete[] tmpbuf;
    RETURN_IF_ERR(MPI_Barrier(comm));
    return 0;
}

int reduce(const void* sendbuf, void* recvbuf, int count, MPI_Datatype datatype, MPI_Op op, int root, MPI_Comm comm) {
    if (datatype == MPI_LONG_LONG_INT)
        return ReduceHelper<int64_t>(sendbuf, recvbuf, count, datatype,
                                     op, root, comm, &OperationBitTypes);
    if (datatype == MPI_LONG_INT || datatype == MPI_INT)
        return ReduceHelper<int32_t>(sendbuf, recvbuf, count, datatype,
                                     op, root, comm, &OperationBitTypes);
    if (datatype == MPI_DOUBLE)
        return ReduceHelper<double>(sendbuf, recvbuf, count, datatype,
                                    op, root, comm, &CommonOperation);
    if (datatype == MPI_2INT)
        return ReduceHelper<std::pair<int, int>>(sendbuf, recvbuf, count, datatype,
                                                 op, root, comm, &OperationLoc);
    if (datatype == MPI_DOUBLE_INT)
        return ReduceHelper<std::pair<double, int>>(sendbuf, recvbuf, count, datatype,
                                                    op, root, comm, &OperationLoc);
    // Any needed type is easy to support
    return MPI_ERR_TYPE;
}
