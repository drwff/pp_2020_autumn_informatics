// Copyright 2020 Makarychev Sergey
#ifndef MODULES_TASK_1_MAKARYCHEV_S_MAX_VALUE_MAX_VALUE_H_
#define MODULES_TASK_1_MAKARYCHEV_S_MAX_VALUE_MAX_VALUE_H_

#include <iostream>
#include <vector>
#include <random>
#include <ctime>

std::vector<int> getRandomMatrix(int rows, int cols);
int getMaxElemSequential(const std::vector<int>& locVec);
int getMaxElemParallel(const std::vector<int>& mat, int rows, int cols);

#endif  // MODULES_TASK_1_MAKARYCHEV_S_MAX_VALUE_MAX_VALUE_H_
