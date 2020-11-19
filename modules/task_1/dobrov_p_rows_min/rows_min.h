// Copyright 2020 Dobrov Pavel
#ifndef MODULES_TASK_1_DOBROV_P_ROWS_MIN_ROWS_MIN_H_
#define MODULES_TASK_1_DOBROV_P_ROWS_MIN_ROWS_MIN_H_

#include <vector>

std::vector<int> randomMatrix(int cols, int rows);
std::vector<int> getParallelOperations(std::vector<int> global_mat, int cols, int rows);
std::vector<int> getSequentialOperations(std::vector<int> mat, int cols, int rows);

#endif  // MODULES_TASK_1_DOBROV_P_ROWS_MIN_ROWS_MIN_H_
