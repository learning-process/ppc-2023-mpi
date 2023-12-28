// Copyright 2023 Novostroev Ivan
#include "task_3/novostroev_i_matrix_mult_strassen/strassen.h"

std::vector<double> sequentialMul(std::vector<double> matrixa,
 std::vector<double> matrixb, int n) {
    std::vector<double> resmatrix(n * n);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            for (int k = 0; k < n; k++)
                resmatrix[i * n + j] +=
                 matrixa[i * n + k] * matrixb[k * n + j];
    return resmatrix;
}

void getRandMatrix(std::vector<double>* matrix, int N, int n) {
    std::random_device dev;
    std::mt19937 gen(dev());
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            (*matrix)[i * N + j] =
             static_cast<double>(static_cast<int>(gen()) % 200 - 100)
              / static_cast<double>(10);
}

bool isMatrEqual(std::vector<double> matrixa,
 std::vector<double> matrixb, int n) {
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            if (abs(matrixa[i * n + j] - matrixb[i * n + j]) > 0.00001)
                return false;
    return true;
}

void matrMalloc(double** matrix, int n) {
    *matrix = reinterpret_cast<double*>(malloc(n * n * sizeof(double)));
}

void matrCalloc(double** matrix, int n) {
    *matrix = reinterpret_cast<double*>(calloc(n * n, sizeof(double)));
}

void print_matr(std::vector<double> matrix, int Size) {
    for (int i = 0; i < Size; i++) {
        for (int j = 0; j < Size; j++)
            std::cout << matrix[i * Size + j] << " ";
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

std::vector<double> strassenAlgorithm(std::vector<double> matrixa,
 std::vector<double> matrixb, int n) {
    int size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int sqrtSize = static_cast<int>(sqrt(static_cast<double>(size)));
    if (sqrtSize * sqrtSize != size)
        throw 0;

    std::vector<double> cmatrix(n * n);
    std::vector<int> coordinates_of_grid(2);
    std::vector<int> dimSize(2, sqrtSize);
    std::vector<int> periodic(2, 0);
    std::vector<int> subDims(2);
    subDims = { 0, 1 };
    MPI_Comm cgrid;
    MPI_Comm ccolumn;
    MPI_Comm crow;

    MPI_Cart_create(MPI_COMM_WORLD, 2,
     dimSize.data(), periodic.data(), 0, &cgrid);
    MPI_Cart_coords(cgrid, rank, 2, coordinates_of_grid.data());
    MPI_Cart_sub(cgrid, subDims.data(), &crow);
    std::swap(subDims[0], subDims[1]);
    MPI_Cart_sub(cgrid, subDims.data(), &ccolumn);

    int BSize = static_cast<int>(ceil(static_cast<double>(n) / sqrtSize));
    int BBSize = BSize * BSize;
    std::vector<double> pAblock(BBSize, 0);
    std::vector<double> pBblock(BBSize, 0);
    std::vector<double> pCblock(BBSize, 0);
    if (rank == 0) {
        for (int i = 0; i < BSize; i++)
            for (int j = 0; j < BSize; j++) {
                pAblock[i * BSize + j] = matrixa[i * n + j];
                pBblock[i * BSize + j] = matrixb[i * n + j];
            }
    }
    int enlarged_size = BSize * sqrtSize;
    MPI_Datatype typeofblock;
    MPI_Type_vector(BSize, BSize, enlarged_size, MPI_DOUBLE, &typeofblock);
    MPI_Type_commit(&typeofblock);

    MPI_Status Status;
    if (rank == 0) {
        for (int l = 1; l < size; l++) {
            MPI_Send(matrixa.data() + (l % sqrtSize) * BSize +
             (l / sqrtSize) * n * BSize, 1, typeofblock, l, 0, cgrid);
            MPI_Send(matrixb.data() + (l % sqrtSize) * BSize +
             (l / sqrtSize) * n * BSize, 1, typeofblock, l, 1, cgrid);
        }
    } else {
        MPI_Recv(pAblock.data(), BBSize, MPI_DOUBLE, 0, 0, cgrid, &Status);
        MPI_Recv(pBblock.data(), BBSize, MPI_DOUBLE, 0, 1, cgrid, &Status);
    }


    for (int i = 0; i < sqrtSize; i++) {
        std::vector<double> tmpmatra(BBSize);
        int bcast = (coordinates_of_grid[0] + i) % sqrtSize;
        if (coordinates_of_grid[1] == bcast) tmpmatra = pAblock;
        MPI_Bcast(tmpmatra.data(), BBSize, MPI_DOUBLE, bcast, crow);
        for (int j = 0; j < BSize; j++)
            for (int k = 0; k < BSize; k++) {
                double temp = 0;
                for (int l = 0; l < BSize; l++)
                    temp += tmpmatra[j * BSize + l] * pBblock[l * BSize + k];
                pCblock[j * BSize + k] += temp;
            }
        int nextp = coordinates_of_grid[0] + 1;
        if (coordinates_of_grid[0] == sqrtSize - 1) nextp = 0;
        int prevp = coordinates_of_grid[0] - 1;
        if (coordinates_of_grid[0] == 0) prevp = sqrtSize - 1;
        MPI_Sendrecv_replace(pBblock.data(), BBSize,
         MPI_DOUBLE, prevp, 0, nextp, 0, ccolumn, &Status);
    }

    if (rank == 0) {
        for (int i = 0; i < BSize; i++)
            for (int j = 0; j < BSize; j++)
                cmatrix[i * n + j] = pCblock[i * BSize + j];
    }
    if (rank != 0)
        MPI_Send(pCblock.data(), BBSize, MPI_DOUBLE, 0, 3, MPI_COMM_WORLD);
    if (rank == 0) {
        for (int i = 1; i < size; i++)
            MPI_Recv(cmatrix.data() + (i % sqrtSize) * BSize +
            (i / sqrtSize) * n *
            BSize, BBSize, typeofblock, i, 3, MPI_COMM_WORLD, &Status);
    }

    MPI_Type_free(&typeofblock);
    return cmatrix;
}
