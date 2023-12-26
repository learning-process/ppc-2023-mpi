// Copyright 2023 Kudinov Nikita
#include "task_2/kudinov_n_matrix_multiplication_vertical/matrix_multiplication_vertical.h"

#include <iostream>
#include <utility>
#include <functional>

#include <boost/mpi.hpp>
#include <boost/serialization/utility.hpp>
#include <boost/serialization/vector.hpp>

void print_matrix(const Matrix<double>& matrix) {
    for (std::size_t i = 0; i < matrix.size(); i += 1) {
        for (std::size_t j = 0; j < matrix[i].size(); j += 1) {
            std::cout << matrix[i][j] << ' ';
        }
        std::cout << '\n';
    }
    std::cout << std::endl;
}

void print_vector(const Vector<double>& vector) {
    for (std::size_t i = 0; i < vector.size(); i += 1) {
        std::cout << vector[i] << ' ';
    }
    std::cout << '\n' << std::endl;
}

Matrix<double> matrix_get_submatrix(
    const Matrix<double>& matrix,
    const std::size_t y,
    const std::size_t x,
    const std::size_t height,
    const std::size_t width) {
    if (y + height > matrix.size() || x + width > matrix[0].size()) {
        throw std::runtime_error("Submatrix is out of bounds");
    }

    Matrix<double> out(height, Vector<double>(width, 0.0));

    for (std::size_t i = y; i < y + height; i += 1) {
        for (std::size_t j = x; j < x + width; j += 1) {
            out[i - y][j - x] = matrix[i][j];
        }
    }

    return out;
}

Vector<double> multiply_matrices_sequential(
    const Matrix<double>& matrix,
    const Vector<double>& vector) {
    if (matrix.empty()) return {};
    if (matrix[0].size() != vector.size()) {
        throw std::runtime_error(
            "The number of columns in the matrix must be equal to the size of the vector");
    }

    Vector<double> out(matrix.size(), 0.0);

    for (std::size_t i = 0; i < matrix.size(); i += 1) {
        for (std::size_t j = 0; j < matrix[i].size(); j += 1) {
            out[i] += matrix[i][j] * vector[j];
        }
    }

    return out;
}

Vector<double> multiply_matrices_parallel(
    const Matrix<double>& matrix,
    const Vector<double>& vector) {
    if (matrix.empty()) return {};
    if (matrix[0].size() != vector.size()) {
        throw std::runtime_error(
            "The number of columns in the matrix must be equal to the size of the vector");
    }

    boost::mpi::communicator world;

    std::vector<std::pair<Matrix<double>, Vector<double>>> segments;
    std::pair<Matrix<double>, Vector<double>> local_segment;
    if (world.rank() == 0) {
        std::size_t segment_size = vector.size() / world.size();

        int i;
        std::size_t segment_position;
        for (i = 0, segment_position = 0; i < world.size() - 1; i += 1, segment_position += segment_size) {
            Matrix<double> submatrix = matrix_get_submatrix(matrix, 0, segment_position, matrix.size(), segment_size);

            auto subvector_position = vector.begin() + segment_position;
            Vector<double> subvector = Vector<double>(subvector_position, subvector_position + segment_size);

            segments.push_back(std::make_pair(
                submatrix,
                subvector));
        }

        segments.push_back(std::make_pair(
            matrix_get_submatrix(matrix, 0, segment_position, matrix.size(), matrix[0].size() - segment_position),
            Vector<double>(vector.begin() + segment_position, vector.end())));
    }

    boost::mpi::scatter(world, segments, local_segment, 0);

    Vector<double> local_result = multiply_matrices_sequential(local_segment.first, local_segment.second);

    Vector<double> global_result(local_result.size(), 0.0);
    boost::mpi::reduce(world, local_result, global_result, std::plus<double>(), 0);

    return global_result;
}
