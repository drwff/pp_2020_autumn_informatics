// Copyright 2020 Gushchin Artem
#ifndef MODULES_TASK_2_GUSHCHIN_A_IMAGE_SMOOTHING_IMAGE_SMOOTHING_H_
#define MODULES_TASK_2_GUSHCHIN_A_IMAGE_SMOOTHING_IMAGE_SMOOTHING_H_

#include <vector>

using uchar = unsigned char;

std::vector<uchar> generateRandomMatrix(const int rows, const int cols);
std::vector<uchar> medianFilter(const std::vector<uchar>& matrix, const int rows, const int cols, const int radius);
std::vector<uchar> rangeMedianFilter(const std::vector<uchar>& matrix, const int rows, const int cols, const int radius,
    const int startX, const int startY, const int endX, const int endY);
std::vector<uchar> parallelMedianFilter(std::vector<uchar> matrix, const int rows, const int cols, const int radius);

#endif  // MODULES_TASK_2_GUSHCHIN_A_IMAGE_SMOOTHING_IMAGE_SMOOTHING_H_
