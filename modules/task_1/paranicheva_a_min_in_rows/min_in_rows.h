// Copyright 2020 Paranicheva Alyona
#ifndef MODULES_TASK_1_PARANICHEVA_A_MIN_IN_ROWS_MIN_IN_ROWS_H_
#define MODULES_TASK_1_PARANICHEVA_A_MIN_IN_ROWS_MIN_IN_ROWS_H_

#include <vector>

std::vector<int> getRandomMatrix(int rows, int cols);
int getMinInRow(std::vector<int> array, int cols, int num);
std::vector<int> getParallMin(std::vector<int> arr, int rows, int cols);
std::vector<int> getSequentialMin(std::vector<int> arr, int rows, int cols);

#endif  // MODULES_TASK_1_PARANICHEVA_A_MIN_IN_ROWS_MIN_IN_ROWS_H_
