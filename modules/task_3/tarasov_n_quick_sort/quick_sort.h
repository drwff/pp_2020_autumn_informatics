// Copyright 2020 Tarasov Nikita
#ifndef MODULES_TASK_3_TARASOV_N_QUICK_SORT_QUICK_SORT_H_
#define MODULES_TASK_3_TARASOV_N_QUICK_SORT_QUICK_SORT_H_
#include <vector>

void print_array(std::vector<int> vec);
std::vector<int> random_array(int _size);
void quick_sort(std::vector<int> *vec, int left, int right);
void quick_sort_mpi(std::vector<int> *vec);

#endif  // MODULES_TASK_3_TARASOV_N_QUICK_SORT_QUICK_SORT_H_
