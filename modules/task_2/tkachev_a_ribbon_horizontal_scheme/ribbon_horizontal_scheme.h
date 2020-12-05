// Copyright 2020 Tkachev Alexey
#ifndef MODULES_TASK_2_TKACHEV_A_RIBBON_HORIZONTAL_SCHEME_RIBBON_HORIZONTAL_SCHEME_H_
#define MODULES_TASK_2_TKACHEV_A_RIBBON_HORIZONTAL_SCHEME_RIBBON_HORIZONTAL_SCHEME_H_
#include <vector>

std::vector<int> getRandomVector(int count_rows = 0, int count_columns = 0);

std::vector<int> multiplyOnVector(std::vector<int> matrix, std::vector<int> the_vector, int matrix_count_rows = 0,
                     int matrix_count_columns = 0);

std::vector<int> parallelMultiplyOnVector(std::vector<int> matrix, std::vector<int> the_vector,
                                          int matrix_count_rows = 0, int matrix_count_columns = 0);

#endif  // MODULES_TASK_2_TKACHEV_A_RIBBON_HORIZONTAL_SCHEME_RIBBON_HORIZONTAL_SCHEME_H_
