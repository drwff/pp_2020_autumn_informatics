// Copyright 2020 Kuznetsov Nikita
#ifndef MODULES_TASK_2_KUZNETSOV_GAUSSIAN_VERTICAL_GAUSSIAN_VERTICAL_H_
#define MODULES_TASK_2_KUZNETSOV_GAUSSIAN_VERTICAL_GAUSSIAN_VERTICAL_H_

#include <vector>

std::vector<double> getMatrix(int x);
std::vector<double> sequentialMethod(std::vector<double> mat, int x);
std::vector<double> parallelMethod(std::vector<double> mat, int x);

#endif  // MODULES_TASK_2_KUZNETSOV_GAUSSIAN_VERTICAL_GAUSSIAN_VERTICAL_H_
