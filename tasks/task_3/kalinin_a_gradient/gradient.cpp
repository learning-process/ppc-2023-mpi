// Copyright 2023 Kalinin Alexandr
#include "task_3/kalinin_a_gradient/gradient.h"

Vector RandVec(int n) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(0, 10);

    Vector vect(n);
    for (int i = 0; i < n; ++i) {
        vect[i] = dist(gen) % 3;
    }
    return vect;
}

Vector RandMat(int n) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(0, 10);

    Vector matrix(n * n);
    for (int i = 0; i < n; ++i) {
        for (int j = i; j < n; ++j) {
            matrix[i * n + j] = dist(gen) % 3;
            matrix[j * n + i] = dist(gen) % 3;
        }
    }
    return matrix;
}

double scalar_mult(const Vector& x, const Vector& y) {
    double res = 0;
    for (int i = 0; i < x.size(); i++) {
        res = res + x[i] * y[i];
    }
    return res;
}

Vector mult_MxV(const Vector& matrix,
    const Vector& vect) {
    Vector res(matrix.size() / vect.size());
    for (int i = 0; i < matrix.size() / vect.size(); i++) {
        res[i] = 0;
        for (int j = 0; j < vect.size(); j++) {
            res[i] += matrix[i * vect.size() + j] * vect[j];
        }
    }
    return res;
}

Vector Serial_method_gradient(const Vector& matrixA,
    const Vector& vectorB, int size) {
    Vector gradient(size), direction(size), temp, solution(size);
    double errorThreshold = 0.01, alpha = 0.0, beta = 0.0;
    for (int i = 0; i < size; i++) {
        solution[i] = 1;
    }
    temp = mult_MxV(matrixA, solution);
    for (int i = 0; i < size; i++) {
        gradient[i] = vectorB[i] - temp[i];
        direction[i] = gradient[i];
    }

    int iteration = 0;
    do {
        temp = mult_MxV(matrixA, direction);
        double numerator = scalar_mult(gradient, gradient);
        alpha = numerator / scalar_mult(direction, temp);
        for (int i = 0; i < size; i++) {
            solution[i] = solution[i] + alpha * direction[i];
            gradient[i] = gradient[i] - alpha * temp[i];
        }
        beta = scalar_mult(gradient, gradient) / numerator;
        for (int i = 0; i < size; i++) {
            direction[i] = gradient[i] + beta * direction[i];
        }
        iteration++;
    } while (sqrt(scalar_mult(gradient, gradient))
        > errorThreshold && iteration <= size);
    return solution;
}

Vector Paralle_method_gradient(const Vector& matrixA,
    const Vector& vectorB, int size) {
    int totalProcesses = 0, processRank = 0;
    double errorThreshold = 0.01, alpha = 0.0, beta = 0.0,
        newResidual, partialAlpha, partialBeta,
        alphaNumerator, betaNumerator;
    Vector matrixA1 = matrixA, vectorB1 = vectorB, solution(size);
    for (int i = 0; i < size; i++) {
        solution[i] = 1;
    }

    MPI_Status status;
    MPI_Comm_size(MPI_COMM_WORLD, &totalProcesses);
    MPI_Comm_rank(MPI_COMM_WORLD, &processRank);
    int sizePerProcess = size / totalProcesses;
    int remainder = size % totalProcesses;
    MPI_Bcast(matrixA1.data(), size * size, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(vectorB1.data(), size, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    Vector partialMatrixA(size * sizePerProcess + remainder * size);
    if (processRank == 0) {
        for (int i = 0; i < size * sizePerProcess + size * remainder; i++) {
            partialMatrixA[i] = matrixA1[i];
        }
        if (sizePerProcess != 0) {
            for (int i = 1; i < totalProcesses; i++) {
                MPI_Send(&matrixA1[0] + i * sizePerProcess * size
                    + remainder * size,
                    sizePerProcess * size, MPI_DOUBLE, i, 1, MPI_COMM_WORLD);
            }
        }
    } else {
        if (sizePerProcess != 0) {
            MPI_Recv(&partialMatrixA[0], sizePerProcess * size,
                MPI_DOUBLE, 0, 1, MPI_COMM_WORLD, &status);
        }
    }

    Vector temp = mult_MxV(partialMatrixA, solution);
    Vector gradient0(sizePerProcess + remainder),
        gradient1(sizePerProcess + remainder),
        direction(size), partialDirection(sizePerProcess + remainder),
        directionResult(sizePerProcess + remainder);
    if (processRank == 0) {
        for (int i = 0; i < sizePerProcess + remainder; i++) {
            gradient0[i] = vectorB1[i] - temp[i];
            direction[i] = gradient0[i];
        }
        if (sizePerProcess != 0) {
            for (int i = 1; i < totalProcesses; i++) {
                MPI_Recv(&direction[0] + i * sizePerProcess + remainder,
                    sizePerProcess, MPI_DOUBLE, i, 2, MPI_COMM_WORLD, &status);
            }
        }
    } else {
        for (int i = 0; i < sizePerProcess; i++)
            gradient0[i] = vectorB1[processRank *
            sizePerProcess + remainder + i] - temp[i];
        if (sizePerProcess != 0) {
            MPI_Send(&gradient0[0], sizePerProcess,
                MPI_DOUBLE, 0, 2, MPI_COMM_WORLD);
        }
    }
    MPI_Bcast(direction.data(), size, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    int iteration = 0;
    do {
        temp = mult_MxV(partialMatrixA, direction);
        if (processRank == 0) {
            for (int i = 0; i < sizePerProcess + remainder; i++) {
                partialDirection[i] = direction[i];
            }
        } else {
            for (int i = 0; i < sizePerProcess; i++) {
                partialDirection[i] = direction[processRank
                    * sizePerProcess + remainder + i];
            }
        }
        partialAlpha = scalar_mult(gradient0, gradient0);
        partialBeta = scalar_mult(partialDirection, temp);
        MPI_Allreduce(&partialAlpha, &alphaNumerator, 1,
            MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
        MPI_Allreduce(&partialBeta, &betaNumerator, 1,
            MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
        alpha = alphaNumerator / betaNumerator;
        for (int i = 0; i < size; i++) {
            solution[i] += alpha * direction[i];
        }
        for (int i = 0; i < sizePerProcess + remainder; i++) {
            gradient1[i] = gradient0[i] - alpha * temp[i];
        }
        partialAlpha = scalar_mult(gradient1, gradient1);
        MPI_Allreduce(&partialAlpha, &newResidual, 1,
            MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
        beta = newResidual / alphaNumerator;
        if (processRank == 0) {
            for (int i = 0; i < sizePerProcess + remainder; i++) {
                direction[i] = gradient1[i] + beta * direction[i];
            }
            if (sizePerProcess != 0) {
                for (int i = 1; i < totalProcesses; i++) {
                    MPI_Recv(&direction[0] + i * sizePerProcess + remainder,
                        sizePerProcess, MPI_DOUBLE, i, 3,
                        MPI_COMM_WORLD, &status);
                }
            }
        } else {
            for (int i = 0; i < sizePerProcess; i++) {
                directionResult[i] = gradient1[i] + beta *
                    direction[processRank * sizePerProcess + remainder + i];
            }
            if (sizePerProcess != 0) {
                MPI_Send(&directionResult[0], sizePerProcess,
                    MPI_DOUBLE, 0, 3, MPI_COMM_WORLD);
            }
        }
        MPI_Bcast(direction.data(), size, MPI_DOUBLE, 0, MPI_COMM_WORLD);
        gradient0 = gradient1;
        iteration++;
    } while ((sqrt(newResidual) > errorThreshold) && (iteration <= size));
    return solution;
}
