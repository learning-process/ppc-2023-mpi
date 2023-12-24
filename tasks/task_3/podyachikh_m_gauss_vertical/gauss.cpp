// Copyright 2023 Podyachikh Mikhail
#include <mpi.h>
#include <random>
#include <algorithm>
#include <numeric>
#include "task_3/podyachikh_m_gauss_vertical/gauss.h"

void FillRandom(Image *image) {
    std::default_random_engine re;
    std::uniform_int_distribution<> rnd(0, 255);
    for (auto &line : *image) {
        for (auto &p : line) {
            p = rnd(re);
        }
    }
}

Kernel GetKernel(double sigma = 3) {
    int size = 3;
    Kernel kernel(size, vd(size));
    double norm = 0;

    int r = size / 2;
    for (int x = -r; x <= r; x++)
        for (int y = -r; y <= r; y++) {
            kernel[x+r][y+r] = std::exp(-(x * x + y * y) / (sigma * sigma));
            norm += kernel[x+r][y+r];
        }
    for (int x = 0; x < size; x++)
        for (int y = 0; y < size; y++)
            kernel[x][y] /= norm;

    return kernel;
}

Image Gauss(const Image &image) {
    Image new_image = image;
    Kernel kernel = GetKernel();
    int r = kernel.size() / 2;
    for (int i = 1; i < image.size() - 1; i++) {
        for (int j = 1; j < image[0].size() - 1; j++) {
            double new_color = 0;
            for (int x = -r; x <= r; x++) {
                for (int y = -r; y <= r; y++) {
                    new_color += image[i + x][j + y] * kernel[x+r][y+r];
                }
            }
            int color = round(new_color);
            if (color > 255) color = 255;
            else if (color < 0) color = 0;
            new_image[i][j] = color;
        }
    }
    return new_image;
}

Image GaussParallel(const Image &image) {
    int size_world, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size_world);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int width = image[0].size();
    int part_size = width / size_world;
    int ost = width % size_world;
    std::vector<int> get_width(size_world, part_size);
    std::fill(get_width.begin(), get_width.begin() + ost, part_size + 1);

    std::vector<int> get_displs(size_world);
    std::partial_sum(get_width.begin(), get_width.end() - 1, get_displs.begin() + 1);

    std::vector<int> send_width = get_width;
    std::vector<int> send_displs = get_displs;

    for (int i = 0; i < size_world; i++) {
        if (i != 0) {
            send_width[i]++;
            send_displs[i]--;
        }
        if (i != size_world - 1) {
            send_width[i]++;
        }
    }

    Image part(image.size(), vi(send_width[rank]));
    for (int i = 0; i < image.size(); i++) {
        MPI_Scatterv(image[i].data(), send_width.data(), send_displs.data(), MPI_UNSIGNED_CHAR,
                     part[i].data(), send_width[rank], MPI_UNSIGNED_CHAR, 0, MPI_COMM_WORLD);
    }

    part = Gauss(part);
    Image new_image(image.size(), vi(image[0].size()));
    for (int i = 0; i < image.size(); i++) {
        auto data = part[i].data();
        if (rank != 0) data++;
        MPI_Gatherv(data, get_width[rank], MPI_UNSIGNED_CHAR, new_image[i].data(),
                    get_width.data(), get_displs.data(), MPI_UNSIGNED_CHAR, 0, MPI_COMM_WORLD);
    }
    return new_image;
}
