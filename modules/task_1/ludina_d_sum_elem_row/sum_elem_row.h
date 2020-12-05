// Copyright 2020 Ludina Daria
#ifndef MODULES_TASK_1_LUDINA_D_SUM_ELEM_ROW_SUM_ELEM_ROW_H_
#define MODULES_TASK_1_LUDINA_D_SUM_ELEM_ROW_SUM_ELEM_ROW_H_

#include <mpi.h>
#include <random>
#include <ctime>
#include <vector>

std::vector<int> createRandomMatrix(int rows, int cols);
std::vector<int> getSequentialOperations(std::vector<int> matrix, int row, int cols);
std::vector<int> getParallelOperations(std::vector<int> global_mt, int row, int cols);

#endif  // MODULES_TASK_1_LUDINA_D_SUM_ELEM_ROW_SUM_ELEM_ROW_H_
