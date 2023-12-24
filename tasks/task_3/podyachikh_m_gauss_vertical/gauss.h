// Copyright 2023 Podyachikh Mikhail
#ifndef TASKS_TASK_3_PODYACHIKH_M_GAUSS_VERTICAL_GAUSS_H_
#define TASKS_TASK_3_PODYACHIKH_M_GAUSS_VERTICAL_GAUSS_H_

#include <vector>

typedef std::vector<uint8_t> vi;
typedef std::vector<double> vd;
typedef std::vector<vi> Image;
typedef std::vector<vd> Kernel;

void FillRandom(Image *image);
Image Gauss(const Image &image);
Image GaussParallel(const Image &image);

#endif  // TASKS_TASK_3_PODYACHIKH_M_GAUSS_VERTICAL_GAUSS_H_
