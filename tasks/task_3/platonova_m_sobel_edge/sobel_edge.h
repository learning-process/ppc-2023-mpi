// Copyright 2023 Platonova Maria
#ifndef TASKS_TASK_3_PLATONOVA_M_SOBEL_EDGE_SOBEL_EDGE_H_
#define TASKS_TASK_3_PLATONOVA_M_SOBEL_EDGE_SOBEL_EDGE_H_

#include <vector>
#include <random>

struct Pixel {
    Pixel(int r, int g, int b);

    unsigned char r;
    unsigned char g;
    unsigned char b;

    bool operator==(const Pixel& o) const;
};


std::vector<std::vector<Pixel>> white_image(int w, int h);

std::vector<std::vector<Pixel>> black_image(int w, int h);

std::vector<std::vector<Pixel>> random_image(int w, int h);

std::vector<std::vector<Pixel>> sobel(
            const std::vector<std::vector<Pixel>>& image);

std::vector<std::vector<Pixel>> sobel_std(
            const std::vector<std::vector<Pixel>>& image);

#endif  // TASKS_TASK_3_PLATONOVA_M_SOBEL_EDGE_SOBEL_EDGE_H_
