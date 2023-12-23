// Copyright 2023 Zawadowski Jan
#include "task_3/zawadowski_j_horizontal_linear_filtering/horizontal_linear_filtering.h"

unsigned char changePixel(const std::vector<unsigned char>& image,
                          int x, int y, int rows, int cols) {
    int sum = 0;
    unsigned char gauss[3][3]{ {1, 2, 1}, {2, 4, 2}, {1, 2, 1} };

    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++) {
            int tX = x + i - 1,
                tY = y + j - 1;

            if (tX < 0 || tX > rows - 1) tX = x;
            if (tY < 0 || tY > cols - 1) tY = y;
            if (tX * cols + tY >= cols * rows) {
                tX = x;
                tY = y;
            }
            sum += static_cast<int>(image[tX * cols + tY] * (gauss[i][j]));
        }

    return sum / 16;
}

std::vector<unsigned char> getRandomImage(int rows, int cols) {
    std::vector<unsigned char> image(rows * cols);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<>
        dist(0, std::numeric_limits<unsigned char>::max());

    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            image[i * cols + j] = dist(gen);

    return image;
}

std::vector<unsigned char> filterImageP(const std::vector<unsigned char> &image,
                                        int rows, int cols) {
    MPI_Status status;
    int sizeWorld, rank;
    std::vector<unsigned char> result(rows * cols);
    MPI_Comm_size(MPI_COMM_WORLD, &sizeWorld);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rows <= sizeWorld) return filterImageC(image, rows, cols);
    int chunk = rows / sizeWorld,
        remainder = rows % sizeWorld;
    std::vector<unsigned char> tImage(chunk * cols + 2 * cols),
                               tResult(chunk * cols);

    if (rank == 0) {
        if (sizeWorld != 1) {
            tImage.resize((chunk + remainder + 1) * cols);
            for (int i = 0; i < (chunk + remainder + 1) * cols; i++)
                tImage[i] = image[i];
        }
        for (int i = 1; i < sizeWorld; i++) {
            int count = chunk * cols + cols;
            if (i != sizeWorld - 1) count += cols;

            MPI_Send(&image[0] + (i * chunk * cols) + (remainder - 1) * cols,
                     count,
                     MPI_UNSIGNED_CHAR,
                     i,
                     0,
                     MPI_COMM_WORLD);
        }
    } else {
        if (rank != sizeWorld - 1) {
            MPI_Recv(&tImage[0],
                     (chunk + 2) * cols + 2,
                     MPI_UNSIGNED_CHAR,
                     0,
                     0,
                     MPI_COMM_WORLD,
                     &status);
        } else {
            tImage.resize((chunk + 1) * cols);
            MPI_Recv(&tImage[0],
                     (chunk + 1) * cols,
                     MPI_UNSIGNED_CHAR,
                     0,
                     0,
                     MPI_COMM_WORLD,
                     &status);
        }
    }

    if (rank == sizeWorld - 1 && sizeWorld != 1) {
        for (int i = 0; i < chunk; i++)
            for (int j = 0; j < cols; j++)
                tResult[i * cols + j] =
                    changePixel(tImage, i + 1, j, chunk + 1, cols);
    } else if (rank != 0) {
        for (int i = 1; i < chunk + 1; i++)
            for (int j = 0; j < cols; j++)
                tResult[(i - 1) * cols + j] =
                    changePixel(tImage, i, j, chunk + 2, cols);
    } else {
        if (sizeWorld != 1) {
            for (int i = 0; i < chunk + remainder; i++)
                for (int j = 0; j < cols; j++)
                    result[i * cols + j] =
                        changePixel(tImage, i, j, chunk + remainder + 1, cols);
        } else {
            for (int i = 0; i < rows; i++)
                for (int j = 0; j < cols; j++)
                    result[i * cols + j] = changePixel(image, i, j, rows, cols);
        }
    }

    if (rank != 0) {
        MPI_Send(&tResult[0],
                 chunk * cols,
                 MPI_UNSIGNED_CHAR,
                 0,
                 0,
                 MPI_COMM_WORLD);
    } else {
        for (int i = 1; i < sizeWorld; i++)
            MPI_Recv(&result[0] +
                        ((chunk + remainder) * cols) + ((i - 1) * chunk * cols),
                     chunk * cols,
                     MPI_UNSIGNED_CHAR,
                     i,
                     0,
                     MPI_COMM_WORLD,
                     &status);
    }

    return result;
}

std::vector<unsigned char> filterImageC(const std::vector<unsigned char> &image,
                                        int rows, int cols) {
    std::vector<unsigned char> result(rows * cols);

    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            result[i * cols + j] = changePixel(image, i, j, rows, cols);

    return result;
}
