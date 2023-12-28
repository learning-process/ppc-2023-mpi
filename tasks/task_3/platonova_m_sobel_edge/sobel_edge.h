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

using Image = std::vector<std::vector<Pixel>>;

Image white_image(int w, int h);

Image black_image(int w, int h);

Image random_image(int w, int h);

Image sobel(const Image& image);

Image sobel_std(const Image& image);

#endif  // TASKS_TASK_3_PLATONOVA_M_SOBEL_EDGE_SOBEL_EDGE_H_
