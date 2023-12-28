// Copyright 2023 Platonova Maria
#include <algorithm>
#include <cmath>
#include "../../../3rdparty/unapproved/unapproved.h"
#include "task_3/platonova_m_sobel_edge/sobel_edge.h"

static std::vector<std::vector<int>> sobel_kernel = {
    { 1, 2, 1},
    { 0, 0, 0},
    { -1, -2, -1}
};

Pixel::Pixel(int r, int g, int b) :
    r(r), g(g), b(b)
{ }

bool Pixel::operator==(const Pixel& o) const {
    return r == o.r && g == o.g && b == o.b;
}

static int clamp(int value, int min, int max) {
    return std::max(min, std::min(value, max));
}

static Pixel get_pixel(
    const std::vector<std::vector<Pixel>>& image, int x, int y) {
    return image[clamp(y, 0, image.size() - 1)]
                [clamp(x, 0, image[0].size() - 1)];
}

std::vector<std::vector<Pixel>> white_image(int w, int h) {
    const Pixel white = { 255, 255, 255 };
    std::vector<std::vector<Pixel>> res(h, std::vector<Pixel>(w, white));
    return res;
}

std::vector<std::vector<Pixel>> black_image(int w, int h) {
    const Pixel black = { 0, 0, 0 };
    std::vector<std::vector<Pixel>> res(h, std::vector<Pixel>(w, black));
    return res;
}

std::vector<std::vector<Pixel>> random_image(int w, int h) {
    const Pixel black = { 0, 0, 0 };
    std::vector<std::vector<Pixel>> res(h, std::vector<Pixel>(w, black));
    std::random_device rnd_device;
    std::mt19937 mt_gen(rnd_device());
    for (int y = 0; y < res.size(); y++) {
        for (int x = 0; x < res[0].size(); x++) {
            res[y][x] = { static_cast<int>(mt_gen()) % 256,
            static_cast<int>(mt_gen()) % 256,
            static_cast<int>(mt_gen()) % 256 };
        }
    }
    return res;
}

static Pixel sobel_pixel(
        const std::vector<std::vector<Pixel>>& image, int x, int y) {
    int gx_r = 0;
    int gx_g = 0;
    int gx_b = 0;
    for (int l = -1; l <= 1; l++) {
        for (int k = -1; k <= 1; k++) {
            Pixel neighbor = get_pixel(image, x + k, y + l);
            gx_r += neighbor.r * sobel_kernel[l + 1][k + 1];
            gx_g += neighbor.g * sobel_kernel[l + 1][k + 1];
            gx_b += neighbor.b * sobel_kernel[l + 1][k + 1];
        }
    }

    int gy_r = 0;
    int gy_g = 0;
    int gy_b = 0;
    for (int l = -1; l <= 1; l++) {
        for (int k = -1; k <= 1; k++) {
            Pixel neighbor = get_pixel(image, x + k, y + l);
            gy_r += neighbor.r * sobel_kernel[k + 1][l + 1];
            gy_g += neighbor.g * sobel_kernel[k + 1][l + 1];
            gy_b += neighbor.b * sobel_kernel[k + 1][l + 1];
        }
    }

    return {
        static_cast<char>(clamp(static_cast<int>(
                sqrt(gx_r * gx_r + gy_r * gy_r)), 0, 255)),
        static_cast<char>(clamp(static_cast<int>(
                sqrt(gx_g * gx_g + gy_g * gy_g)), 0, 255)),
        static_cast<char>(clamp(static_cast<int>(
                sqrt(gx_b * gx_b + gy_b * gy_b)), 0, 255))
    };
}

std::vector<std::vector<Pixel>> sobel(
        const std::vector<std::vector<Pixel>>& image) {
    std::vector<std::vector<Pixel>> result = std::vector<std::vector<Pixel>>(image);
    for (int y = 0; y < image.size(); y++) {
        for (int x = 0; x < image[0].size(); x++) {
            result[y][x] = sobel_pixel(image, x, y);
        }
    }
    return result;
}

std::vector<std::vector<Pixel>> sobel_std(
        const std::vector<std::vector<Pixel>>& image) {
    std::vector<std::vector<Pixel>> result = std::vector<std::vector<Pixel>>(image);
    std::vector<std::thread> threads;
    int threads_count = static_cast<int>(std::sqrt(image.size()));
    int rows_per_thread = image.size() / threads_count + 1;
    for (int t = 0; t < threads_count; t++) {
        threads.emplace_back([t, rows_per_thread, &result, &image]() {
            for (int y = t * rows_per_thread;
                y < image.size() && y < (t + 1) * rows_per_thread;
                y++) {
                for (int x = 0; x < image[0].size(); x++) {
                    result[y][x] = sobel_pixel(image, x, y);
                }
            }
        });
    }
    for (int t = 0; t < threads_count; t++) {
        threads[t].join();
    }
    return result;
}