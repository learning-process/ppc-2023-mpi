// Copyright 2023 Afanasyev Aleksey
#include "task_3/afanasyev_a_horizontal_gauss_filter/horizontal_gauss_filter.h"

bool compareVectors(const std::vector<unsigned char>& vector1,
                    const std::vector<unsigned char>& vector2) {
    if (vector1.size() != vector2.size()) {
        return false;
    }

    for (size_t i = 0; i < vector1.size(); ++i) {
        if (vector1[i] != vector2[i]) {
            return false;
        }
    }

    return true;
}

std::vector<unsigned char> generateMonoImage(int columns, int rows, int color) {
    std::vector<unsigned char> image(columns * rows);

    for (int i = 0; i < columns; i++)
        for (int j = 0; j < rows; j++)
            image[i * rows + j] = static_cast<unsigned char>(color);

    return image;
}

std::vector<unsigned char> generateRandomImage(int columns, int rows) {
    std::mt19937 gen;
    gen.seed(static_cast<unsigned int>(time(0)));
    std::vector<unsigned char> image(columns * rows);

    for (int i = 0; i < columns; i++)
        for (int j = 0; j < rows; j++)
            image[i*rows+j] = static_cast<unsigned char>(gen() % 256);

    return image;
}

unsigned char applyGaussianFilterPixelOperation(const std::vector<unsigned char> &image,
                                           int x1, int y1, int rows, int cols) {
    int sum = 0;

    for (int i = -1; i < 2; ++i) {
        for (int j = -1; j < 2; ++j) {
            int x = x1 + i;
            int y = y1 + j;

            if (x < 0 || x > rows - 1)
                x = x1;

            if (y < 0 || y > cols - 1)
                y = y1;

            if (x * cols + y >= cols * rows) {
                x = x1;
                y = y1;
            }

         sum += static_cast<int>(image[x*cols + y] * (matrix_gauss[i + 1][j + 1]));
        }
    }

    return sum/17;
}

std::vector<unsigned char> applySeqGaussianFilter
    (const std::vector<unsigned char>& image, int rows, int cols) {
    std::vector<unsigned char> result(cols * rows);

    for (int i = 0; i < rows; ++i)
        for (int j = 0; j < cols; ++j)
            result[i * cols + j] =
               applyGaussianFilterPixelOperation(image, i, j, rows, cols);

    return result;
}

std::vector<unsigned char> applyParGaussianFilter
(const std::vector<unsigned char>& image, int rows, int cols) {
    int comm_size, rank;
    MPI_Status status;
    std::vector<unsigned char> globalResult(cols * rows);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rows < comm_size) {
        return applySeqGaussianFilter(image, rows, cols);
    }

    const int interval = rows / comm_size;
    const int last_interval = rows % comm_size;
    std::vector<unsigned char> localImage(interval * cols + 2 * cols);
    std::vector<unsigned char> localResult(interval * cols);

    if (rank == 0) {
        if (comm_size != 1) {
            localImage.resize((interval + last_interval + 1) * cols);

            for (int i = 0; i < (interval + last_interval + 1) * cols; ++i) {
                localImage[i] = image[i];
            }
        }
        for (int proc = 1; proc < comm_size; proc++) {
            int startAdress = (proc * interval * cols) + (last_interval - 1) * cols;
            int countSend;

            if (proc != (comm_size - 1)) {
                countSend = interval * cols + 2 * cols;
            } else {
                countSend = interval * cols + cols;
            }

            MPI_Send(&image[0] + startAdress, countSend,
                MPI_UNSIGNED_CHAR, proc, 0, MPI_COMM_WORLD);
        }
    } else if (rank != 0) {
        if (rank != comm_size - 1) {
            MPI_Recv(&localImage[0], (interval + 2) * cols + 2,
                MPI_UNSIGNED_CHAR, 0, 0, MPI_COMM_WORLD, &status);
        } else {
            localImage.resize((interval + 1) * cols);
            MPI_Recv(&localImage[0], (interval + 1) * cols,
                MPI_UNSIGNED_CHAR, 0, 0, MPI_COMM_WORLD, &status);
        }
    }
    if (rank == comm_size - 1 && comm_size != 1) {
        for (int i = 1; i < interval + 1; ++i)
            for (int j = 0; j < cols; ++j)
                localResult[(i - 1) * cols + j] = applyGaussianFilterPixelOperation(localImage,
                    i, j, interval + 1, cols);
    } else if (rank != 0) {
        for (int i = 1; i < interval + 1; ++i)
            for (int j = 0; j < cols; ++j)
                localResult[(i - 1) * cols + j] = applyGaussianFilterPixelOperation(localImage,
                    i, j, interval + 2, cols);
    } else {
        if (comm_size != 1) {
            for (int i = 0; i < interval + last_interval; ++i)
                for (int j = 0; j < cols; ++j)
                    globalResult[i * cols + j] = applyGaussianFilterPixelOperation(localImage,
                        i, j, interval + last_interval + 1, cols);
        } else {
            for (int i = 0; i < rows; ++i)
                for (int j = 0; j < cols; ++j)
                    globalResult[i * cols + j] = applyGaussianFilterPixelOperation(image,
                        i, j, rows, cols);
        }
    }
    if (rank != 0) {
        MPI_Send(&localResult[0], interval * cols,
            MPI_UNSIGNED_CHAR, 0, 0, MPI_COMM_WORLD);
    } else {
        for (int proc = 1; proc < comm_size; ++proc) {
            int start = ((interval + last_interval) * cols)
                + ((proc - 1) * interval * cols);
            MPI_Recv(&globalResult[0] + start, interval * cols,
                MPI_UNSIGNED_CHAR, proc, 0, MPI_COMM_WORLD, &status);
        }
    }

    return globalResult;
}
