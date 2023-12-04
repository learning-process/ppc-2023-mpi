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
            matrix[i * size + j] = matrix[j * size + i] = min + (std::rand() % (max - min + 1));
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
    /*std::vector<double> approximateSolution(size, 0.0);
    std::vector<double> remainder = vector;
    std::vector<double> searchDirection = remainder;
    double oldNorm = 0.0;
    double newNorm = 0.0;
    double alpha = 0.0;
    double epsilon = 1e-10;

    oldNorm = scalarMultiply(remainder, remainder);

    for (size_t i = 0; i < size; i++) {
        std::vector<double> matrixMultiplyVector = multiplyMatrixToVector(matrix, vector);
        alpha = oldNorm / scalarMultiply(searchDirection, matrixMultiplyVector);

        for (size_t j = 0; j < size; j++) {
            approximateSolution[j] += alpha * searchDirection[j];
            remainder[j] -= alpha * matrixMultiplyVector[j];
        }

        newNorm = 0.0;
        newNorm = scalarMultiply(remainder, remainder);

        if (std::sqrt(newNorm) < epsilon) {
            break;
        }

        for (size_t j = 0; j < size; j++) {
            searchDirection[j] = remainder[j] + (newNorm / oldNorm) * searchDirection[j];
        }

        oldNorm = newNorm;
    }

    return approximateSolution;*/
    std::vector<double> remainderCurrent(size);
    std::vector<double> remainderNext(size);
    std::vector<double> approximateSolution(size);
    double norm = 0.0;
    double epsilon = 1e-10;
    double alpha = 0.0;
    double scalar = 0.0;
    int flag = 0;

    for (int i = 0; i < size; i++) {
        approximateSolution[i] = 1;
    }

    std::vector<double> multiply = multiplyMatrixToVector(matrix, approximateSolution);

    for (int i = 0; i < size; i++) {
        remainderCurrent[i] = vector[i] - multiply[i];
    }

    std::vector<double> position(remainderCurrent);

    for(int i = 0; i < size; i++){
        multiply = multiplyMatrixToVector(matrix, position);
        alpha = scalarMultiply(remainderCurrent, remainderCurrent) / scalarMultiply(position, multiply);

        for (int j = 0; j < size; j++) {
            approximateSolution[j] += alpha * position[j];
            remainderNext[j] = remainderCurrent[j] - alpha * multiply[j];
        }

        scalar = scalarMultiply(remainderNext, remainderNext) / scalarMultiply(remainderCurrent, remainderCurrent);
        norm = sqrt(scalarMultiply(remainderNext, remainderNext));

        if (sqrt(norm) < epsilon) {
            break;
        }

        for (int j = 0; j < size; j++) {
            position[j] = remainderNext[j] + scalar * position[j];
        }

        remainderCurrent = remainderNext;
    }

    return approximateSolution;
}

//std::vector<double> parallelConjugateGradient(const std::vector<double>& matrix,
//    const std::vector<double>& vector, size_t size) {
//   
//}