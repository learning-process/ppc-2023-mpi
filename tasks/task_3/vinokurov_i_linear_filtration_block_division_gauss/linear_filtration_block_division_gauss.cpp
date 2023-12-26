// Copyright 2023 Vinokurov Ivan
#include <algorithm>
#include <functional>
#include <vector>
#include <iostream>
#include <cmath>
#include "task_3/vinokurov_i_linear_filtration_block_division_gauss/linear_filtration_block_division_gauss.h"

int funcClamp(int _input, int _min, int _max) {
    if (_input < _min) {
        return 0;
    } else if (_input > _max) {
        return _max;
    } else {
        return _input;
    }
}

const float kernel[3][3] = {
    {1.0f / 16, 2.0f / 16, 1.0f / 16},
    {2.0f / 16, 4.0f / 16, 2.0f / 16},
    {1.0f / 16, 2.0f / 16, 1.0f / 16}
};

unsigned char funcProcessPixelFlat(int _x, int _y, const std::vector<unsigned char>& _flattenImage, int _cols) {
    int radiusX = 1;
    int radiusY = 1;
    float result = 0;

    unsigned char neighborColor;

    for (int l = -radiusY; l <= radiusY; l++) {
        for (int k = -radiusX; k <= radiusX; k++) {
            int idX = funcClamp(_x + k, 0, _cols - 1);
            int idY = funcClamp(_y + l, 0, _cols - 1);
            neighborColor = _flattenImage[idX * _cols + idY];
            result += neighborColor * kernel[k + radiusX][l + radiusY];
        }
    }
    return static_cast<unsigned char>(result);
}

std::vector<unsigned char> convertTo1D(const std::vector<std::vector<unsigned char>>& _image) {
    std::vector<unsigned char> flattenedImage;
    for (const auto& row : _image) {
        flattenedImage.insert(flattenedImage.end(), row.begin(), row.end());
    }
    return flattenedImage;
}

std::vector<std::vector<unsigned char>> convertTo2D(const std::vector<unsigned char>& _flattenedImage,
    int _rows, int _cols) {
    std::vector<std::vector<unsigned char>> image;
    for (int i = 0; i < _rows; ++i) {
        image.emplace_back(_flattenedImage.begin() + i * _cols, _flattenedImage.begin() + (i + 1) * _cols);
    }
    return image;
}

std::vector<std::vector<unsigned char>> applyFilter(const std::vector<std::vector<unsigned char>>& _image) {
    int rows = _image.size();
    int cols = _image[0].size();

    std::vector<unsigned char> flattenedImage = convertTo1D(_image);

    std::vector<unsigned char> temp(flattenedImage.size());

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            temp[i * cols + j] = funcProcessPixelFlat(i, j, flattenedImage, cols);
        }
    }

    return convertTo2D(temp, rows, cols);
}

std::vector<std::vector<unsigned char>> applyFilterMPI(const std::vector<std::vector<unsigned char>>& _image) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (_image.empty()) {
        throw std::runtime_error("Cannot work with an empty image");
    }

    int rows = _image.size();
    int cols = _image[0].size();

    std::vector<unsigned char> flattenedImage = convertTo1D(_image);

    int blockSize = rows / size;
    int blockStart = rank * blockSize;
    int blockEnd = (rank == size - 1) ? rows : blockStart + blockSize;

    std::vector<unsigned char> localFlattenedImage(flattenedImage.begin() + blockStart * cols,
        flattenedImage.begin() + blockEnd * cols);

    std::vector<unsigned char> localResult(localFlattenedImage.size());

    for (int i = 0; i < blockSize; ++i) {
        for (int j = 0; j < cols; ++j) {
            int x = blockStart + i;
            int y = j;

            localResult[i * cols + j] = funcProcessPixelFlat(x, y, flattenedImage, cols);
        }
    }

    std::vector<unsigned char> gatheredResult(rows * cols);
    MPI_Allgather(localResult.data(), localResult.size(), MPI_UNSIGNED_CHAR,
        gatheredResult.data(), localResult.size(), MPI_UNSIGNED_CHAR, MPI_COMM_WORLD);

    return convertTo2D(gatheredResult, rows, cols);
}
