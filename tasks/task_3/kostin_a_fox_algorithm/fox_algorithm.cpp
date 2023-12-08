// Copyright 2023 Kostin Artem
#include <mpi.h>
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <math.h>
#include <random>
#include <algorithm>
#include <functional>
#include "task_3/kostin_a_fox_algorithm/fox_algorithm.h"

double* SequentialMul(double* matrixa, double* matrixb, int n) {
    double* resmatrix = 0;
    matrCalloc(resmatrix, n);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            for (int k = 0; k < n; k++)
                resmatrix[i * n + j] += matrixa[i * n + k] * matrixb[k * n + j];
    return resmatrix;
}

void getRandMatrix(double* matrix, int n) {
    std::random_device dev;
    std::mt19937 gen(dev());
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            matrix[i * n + j] = static_cast<double>(static_cast<int>(gen()) % 200 - 100) / static_cast<double>(10);
}

bool isMatrEqual(double* matrixa, double* matrixb, int n) {
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            if (abs(matrixa[i * n + j] - matrixb[i * n + j]) > 0.00001)
                return false;
    return true;
}

void matrMalloc(double*& matrix, int n) {
    matrix = reinterpret_cast<double*>(malloc(n * n * sizeof(double)));
}

void matrCalloc(double*& matrix, int n) {
    matrix = reinterpret_cast<double*>(calloc(n * n, sizeof(double)));
}

double* Fox_algorithm(int rank, int size, double* matrixa, double* matrixb, int n) {
    int sqrtsize = static_cast<int>(sqrt(static_cast<double>(size)));
    if (sqrtsize * sqrtsize != size)
        return 0;

    int BSize = static_cast<int>(ceil(static_cast<double>(n) / sqrtsize));
    double* pAblock;
    matrMalloc(pAblock, BSize);
    //double* pAblock = reinterpret_cast<double*>(malloc(Bsize * Bsize * sizeof(double)));
    double* pBblock;
    matrMalloc(pBblock, BSize);
    double* tmpreceived;
    matrMalloc(tmpreceived, BSize);
    double* resmatrix = 0;
    double* tmpmatra = 0;
    double* tmpmatrb = 0;
    int enlarged_size = BSize * sqrtsize;
    if (rank == 0) {
        matrCalloc(tmpmatra, enlarged_size);
        matrCalloc(tmpmatrb, enlarged_size);
        // tmpmatra = (double*)calloc(enlarged_size * enlarged_size, sizeof(double));
        // tmpmatrb = (double*)calloc(enlarged_size * enlarged_size, sizeof(double));
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++) {
                tmpmatra[i * enlarged_size + j] = matrixa[i * n + j];
                tmpmatrb[i * enlarged_size + j] = matrixb[i * n + j];
            }
        matrMalloc(resmatrix, enlarged_size);
    }
    int* pstd = reinterpret_cast<int*>(malloc(size * sizeof(int)));
    int* ineq = reinterpret_cast<int*>(malloc(size * sizeof(int)));
    for (int i = 0; i < size; i++) {
        pstd[i] = 1;
        ineq[i] = BSize * (i % sqrtsize) + i / sqrtsize * BSize * enlarged_size;
    }

    int lenofb[2], clmn[2];
    MPI_Aint disp[2];
    MPI_Datatype typeofblock, tosend;
    MPI_Aint sizeofdouble;
    MPI_Type_vector(BSize, BSize, enlarged_size, MPI_DOUBLE, &typeofblock);
    MPI_Type_commit(&typeofblock);
    MPI_Type_extent(MPI_DOUBLE, &sizeofdouble);
    disp[0]  = 0;
    disp[1]  = sizeofdouble;
    lenofb[0] = 1;
    lenofb[1] = 1;
    clmn[0]= typeofblock;
    clmn[1] = MPI_UB;
    MPI_Type_struct(2, lenofb, disp, clmn, &tosend);
    MPI_Type_commit(&tosend);

    MPI_Scatterv(tmpmatra, pstd, ineq, tosend, pAblock, BSize * BSize, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Scatterv(tmpmatrb, pstd, ineq, tosend, pBblock, BSize * BSize, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    double* resblock;
    matrCalloc(resblock, BSize);

    MPI_Comm comm, strcom;
    int dimSize[2], periodic[2], subDims[2];
    dimSize[0] = sqrtsize;
    dimSize[1] = sqrtsize;
    subDims[0] = 0;
    subDims[1] = 1;
    periodic[0] = 1;
    periodic[1] = 1;
    MPI_Cart_create(MPI_COMM_WORLD, 2, dimSize, periodic, 1, &comm);
    MPI_Cart_sub(comm, subDims, &strcom);

    MPI_Status Status;
    int from, to, startofbcast;
    bool flag = false;
    for (int l = 0; l < sqrtsize; l++) {
        startofbcast = (rank / sqrtsize + l) % sqrtsize;
        flag = false;
        if (startofbcast == rank % sqrtsize) flag = true;
        if (flag == true)
            MPI_Bcast(pAblock, BSize * BSize, MPI_DOUBLE, startofbcast, strcom);
        else
            MPI_Bcast(tmpreceived, BSize * BSize, MPI_DOUBLE, startofbcast, strcom);
        for (int i = 0; i < BSize; i++)
            for (int j = 0; j < BSize; j++)
                for (int k = 0; k < BSize; k++) {
                    if (flag == true)
                        resblock[i * BSize + j] += pAblock[i * BSize + k] * pBblock[k * BSize + j];
                    else
                        resblock[i * BSize + j] += tmpreceived[i * BSize + k] * pBblock[k * BSize + j];
                }
        if (l != sqrtsize - 1) {
            from = rank - sqrtsize;
            if (from < 0)
                from = sqrtsize * sqrtsize - sqrtsize + rank % sqrtsize;
            to = rank + sqrtsize;
            if (to >= size)
                to = to % size;

            MPI_Sendrecv_replace(pBblock, BSize * BSize, MPI_DOUBLE, from, 0, to, 0, MPI_COMM_WORLD, &Status);
        }
    }

    MPI_Gatherv(resblock, BSize * BSize, MPI_DOUBLE, resmatrix, pstd, ineq, tosend, 0, MPI_COMM_WORLD);

    double* cmatrix = 0;
    if (rank == 0) {
        matrMalloc(cmatrix, n);
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                cmatrix[i * n + j] = resmatrix[i * enlarged_size + j];
    }

    free(pstd);
    free(ineq);
    free(pAblock);
    free(pBblock);
    free(tmpreceived);
    if (rank == 0) {
        free(tmpmatra);
        free(tmpmatrb);
    }
    free(resblock);
    free(resmatrix);
    MPI_Type_free(&typeofblock);
    MPI_Type_free(&tosend);
    MPI_Comm_free(&comm);
    MPI_Comm_free(&strcom);

    return cmatrix;
}
