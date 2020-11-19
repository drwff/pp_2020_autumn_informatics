// Copyright 2020 Kiseleva Anastasia
#ifndef MODULES_TASK_1_KISELEVA_A_MIN_STOLB_MATRIX_MIN_STOLB_H_
#define MODULES_TASK_1_KISELEVA_A_MIN_STOLB_MATRIX_MIN_STOLB_H_

#include <limits.h>
#include <string>
#include <vector>
#include <random>
#include <ctime>
#include <algorithm>
#include <iostream>


std::vector<std::vector<int>> RandomMatrix(int str, int stlb);
std::vector<int> VVconvertV(std::vector<std::vector<int>> matrix, int str, int stlb);
std::vector<int> Min(std::vector<int> linm, int str, int stlb);

#endif  // MODULES_TASK_1_KISELEVA_A_MIN_STOLB_MATRIX_MIN_STOLB_H_
