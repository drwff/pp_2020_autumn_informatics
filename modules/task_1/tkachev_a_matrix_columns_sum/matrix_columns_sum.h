// Copyright 2020 Tkachev Alexey
#ifndef MODULES_TASK_1_TKACHEV_A_MATRIX_COLUMNS_SUM_MATRIX_COLUMNS_SUM_H_
#define MODULES_TASK_1_TKACHEV_A_MATRIX_COLUMNS_SUM_MATRIX_COLUMNS_SUM_H_
#include <vector>

std::vector<int> generateRandomMatrix(int count_rows, int count_columns = 0);
std::vector<int> getSequentialColumnsSum(std::vector<int> vector, int count_columns = 0, int count_rows = 0,
                                         uint64_t processes_count = 1, int process_id = 0, int offset = 0);
std::vector<int> getParallelColumnsSum(std::vector<int> matrix, int count_columns = 0, int count_rows = 0);

#endif  // MODULES_TASK_1_TKACHEV_A_MATRIX_COLUMNS_SUM_MATRIX_COLUMNS_SUM_H_
