// Copyright 2023 Safarov Nurlan
#ifndef TASKS_TASK_2_SAFAROV_N_IMAGE_SMOOTHING_IMAGE_SMOOTHING_H_
#define TASKS_TASK_2_SAFAROV_N_IMAGE_SMOOTHING_IMAGE_SMOOTHING_H_

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <ctime>
#include <iostream>
#include <random>
#include <vector>

struct Point {
    int x, y;
    Point(int _x, int _y) {
        x = _x;
        y = _y;
    }
};

class Image {
private:

    std::vector<unsigned char> pixels;
    int width;
    int height;

public:

    Image(int _width, int _height);

    int GetWidth() const;
    int GetHeight() const;

    void FillRandom();
    unsigned char GetPixel(int x, int y) const;
    void SetPixel(int x, int y, unsigned char value);
    void Show();

    bool operator==(const Image& other) const;
};

std::vector<Point> GetPoints();

Image GetCustomImage();

Image SmoothingSequential(const Image& source);

Image SmoothingParallel(const Image& source);

#endif  // TASKS_TASK_2_SAFAROV_N_IMAGE_SMOOTHING_IMAGE_SMOOTHING_H_
