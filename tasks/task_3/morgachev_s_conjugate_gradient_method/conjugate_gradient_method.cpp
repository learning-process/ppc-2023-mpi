// Copyright 2023 Morgachev Stepan
#include "task_3/morgachev_s_conjugate_gradient_method/conjugate_gradient_method.h"

std::vector<double> fillVectorRandomNumbers(size_t size, int min, int max) {
    if (size <= 0) {
        throw "Wrong size";
    }

    std::vector<double> vector(size);

    for (auto& element : vector) {
        element = min + (std::rand() % (max - min + 1));
    }

    return vector;
}

std::vector<double> fillMatrixRandomNumbers(size_t size, int min, int max) {
    if (size <= 0) {
        throw " Wrong size";
    }

    std::vector<double> matrix(size * size);

    for (int i = 0; i < size; i++) {
        for (int j = i; j < size; j++) {
            matrix[i * size + j] =
                matrix[j * size + i] =
                min + (std::rand() % (max - min + 1));
        }
    }

    return matrix;
}

double scalarMultiply(const std::vector<double>& vectorX,
    const std::vector<double>& vectorY) {
    double scalar = 0;
    size_t vectorSize = vectorX.size();

    for (int i = 0; i < vectorSize; i++) {
        scalar += vectorX[i] * vectorY[i];
    }

    return scalar;
}

std::vector<double> multiplyMatrixToVector(const std::vector<double>& matrix,
    const std::vector<double>& vector) {
    std::vector<double> linearResult(matrix.size() / vector.size());
    size_t matrixCollumn = matrix.size() / vector.size();
    size_t vectorSize = vector.size();

    for (int i = 0; i < matrixCollumn; i++) {
        linearResult[i] = 0;
        for (int j = 0; j < vectorSize; j++) {
            linearResult[i] += matrix[i * vectorSize + j] * vector[j];
        }
    }
    return linearResult;
}

std::vector<double> serialConjugateGradient(const std::vector<double>& matrix,
    const std::vector<double>& vector, size_t size) {
    std::vector<double> residualPrev(size);
    std::vector<double> residualNext(size);
    std::vector<double> approximateSolution(size);
    double norm = 0.0;
    double epsilon = 1e-10;
    double alpha = 0.0;
    double beta = 0.0;

    for (int i = 0; i < size; i++) {
        approximateSolution[i] = 0;
    }

    std::vector<double> multiply =
        multiplyMatrixToVector(matrix, approximateSolution);

    for (int i = 0; i < size; i++) {
        residualPrev[i] = vector[i] - multiply[i];
    }

    std::vector<double> direction(residualPrev);

    for (int i = 0; i < size; i++) {
        multiply = multiplyMatrixToVector(matrix, direction);
        alpha = scalarMultiply(residualPrev, residualPrev) /
            scalarMultiply(direction, multiply);

        for (int j = 0; j < size; j++) {
            approximateSolution[j] += alpha * direction[j];
            residualNext[j] = residualPrev[j] - alpha * multiply[j];
        }

        beta = scalarMultiply(residualNext, residualNext) /
            scalarMultiply(residualPrev, residualPrev);
        norm = sqrt(scalarMultiply(residualNext, residualNext));

        if (sqrt(norm) < epsilon) {
            break;
        }

        for (int j = 0; j < size; j++) {
            direction[j] = residualNext[j] + beta * direction[j];
        }

        residualPrev = residualNext;
    }

    return approximateSolution;
}

std::vector<double> parallelConjugateGradient(const std::vector<double>& matrix,
    const std::vector<double>& vector, size_t size) {
    int rank;
    int numprocs;
    int root = 0;
    MPI_Op op = MPI_SUM;
    MPI_Comm comm = MPI_COMM_WORLD;
    MPI_Datatype datatype = MPI_DOUBLE;
    MPI_Comm_size(comm, &numprocs);
    MPI_Comm_rank(comm, &rank);

    std::vector<double> matrixPar = matrix;
    std::vector<double> vectorPar = vector;
    std::vector<double> approximateSolution(size);
    std::vector<double> direction(size);
    double norm = 0.0;
    double epsilon = 1e-10;
    double alpha = 0.0;
    double beta = 0.0;
    double residualPrevScalarBuffer;
    double residualNextScalarBuffer;
    double numerator;
    double denominator;
    double residualNextScalar;
    int chunk = size / numprocs;
    int remainder = size % numprocs;
    std::vector<double> residualPrev(chunk);
    std::vector<double> residualNext(chunk);
    std::vector<double> directionPart(chunk);
    std::vector<double> directionRes(chunk);

    for (int i = 0; i < size; i++) {
        approximateSolution[i] = 0;
    }

    MPI_Bcast(matrixPar.data(), size * size, datatype, root, comm);
    MPI_Bcast(vectorPar.data(), size, datatype, root, comm);

    std::vector<double> partOfMatrix(chunk * size);

    if (rank == root) {
        if (remainder != 0) {
            partOfMatrix.resize((chunk + remainder) * size);
            for (int i = 0; i < (chunk + remainder) * size; i++) {
                partOfMatrix[i] = matrixPar[i];
            }
        } else {
            for (int i = 0; i < size * chunk; i++) {
                partOfMatrix[i] = matrixPar[i];
            }
        }
        if (chunk != 0) {
            for (int proc = 1; proc < numprocs; proc++) {
                MPI_Send(&matrixPar[0] + proc * chunk * size + remainder * size,
                    chunk * size, datatype, proc, 1, comm);
            }
        }
    } else {
        MPI_Status status;
        if (chunk != 0) {
            MPI_Recv(&partOfMatrix[0],
                chunk * size,
                datatype,
                root,
                1,
                comm,
                &status);
        }
    }

    std::vector<double> multiply =
        multiplyMatrixToVector(partOfMatrix, approximateSolution);

    if (rank == root) {
        if (remainder != 0) {
            residualPrev.resize(chunk + remainder);
            residualNext.resize(chunk + remainder);
        }
        for (int i = 0; i < chunk + remainder; i++) {
            residualPrev[i] = vectorPar[i] - multiply[i];
        }
    } else {
        for (int i = 0; i < chunk; i++) {
            residualPrev[i] =
                vectorPar[rank * chunk + remainder + i] - multiply[i];
        }
    }

    if (rank == root) {
        if (remainder != 0) {
            for (int i = 0; i < chunk + remainder; i++) {
                direction[i] = residualPrev[i];
            }
        } else {
            for (int i = 0; i < chunk; i++) {
                direction[i] = residualPrev[i];
            }
        }
        if (chunk != 0) {
            MPI_Status status;
            for (int proc = 1; proc < numprocs; proc++) {
                MPI_Recv(&direction[0] + proc * chunk + remainder,
                    chunk, datatype, proc, 2, comm, &status);
            }
        }
    } else {
        if (chunk != 0) {
            MPI_Send(&residualPrev[0], chunk, datatype, root, 2, comm);
        }
    }

    MPI_Bcast(direction.data(), size, datatype, root, comm);

    if (rank == 0 && remainder != 0) {
        directionPart.resize(chunk + remainder);
    }

    for (int i = 0; i < size; i++) {
        multiply = multiplyMatrixToVector(partOfMatrix, direction);

        if (rank == root) {
            for (int i = 0; i < chunk + remainder; i++) {
                directionPart[i] = direction[i];
            }
        } else {
            for (int i = 0; i < chunk; i++) {
                directionPart[i] = direction[rank * chunk + remainder + i];
            }
        }

        residualPrevScalarBuffer = scalarMultiply(residualPrev, residualPrev);
        residualNextScalarBuffer = scalarMultiply(directionPart, multiply);
        MPI_Allreduce(&residualPrevScalarBuffer,
            &numerator,
            1,
            datatype,
            op,
            comm);
        MPI_Allreduce(&residualNextScalarBuffer,
            &denominator,
            1,
            datatype,
            op,
            comm);

        alpha = numerator / denominator;

        for (int i = 0; i < size; i++) {
            approximateSolution[i] += alpha * direction[i];
        }

        if (rank == root) {
            for (int i = 0; i < chunk + remainder; i++) {
                residualNext[i] = residualPrev[i] - alpha * multiply[i];
            }
        } else {
            for (int i = 0; i < chunk; i++) {
                residualNext[i] = residualPrev[i] - alpha * multiply[i];
            }
        }

        residualPrevScalarBuffer = scalarMultiply(residualNext, residualNext);
        MPI_Allreduce(&residualPrevScalarBuffer,
            &residualNextScalar,
            1,
            datatype,
            op,
            comm);
        beta = residualNextScalar / numerator;
        norm = sqrt(residualNextScalar);

        if (sqrt(norm) < epsilon) {
            break;
        }

        if (rank == root) {
            for (int i = 0; i < chunk + remainder; i++) {
                direction[i] = residualNext[i] + beta * direction[i];
            }
            if (chunk != 0) {
                MPI_Status status;
                for (int proc = 1; proc < numprocs; proc++) {
                    MPI_Recv(&direction[0] + proc * chunk + remainder,
                        chunk, datatype, proc, 3, comm, &status);
                }
            }
        } else {
            for (int i = 0; i < chunk; i++) {
                directionRes[i] =
                    residualNext[i] + beta *
                    direction[rank * chunk + remainder + i];
            }
            if (chunk != 0) {
                MPI_Send(&directionRes[0], chunk, datatype, root, 3, comm);
            }
        }

        MPI_Bcast(direction.data(), size, datatype, root, comm);
        residualPrev = residualNext;
    }

    return approximateSolution;
}
