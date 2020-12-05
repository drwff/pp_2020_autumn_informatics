// Copyright 2020 Shagov Maksim
#ifndef MODULES_TASK_2_SHAGOV_M_ODD_EVEN_SORT_ODD_EVEN_SORT_H_
#define MODULES_TASK_2_SHAGOV_M_ODD_EVEN_SORT_ODD_EVEN_SORT_H_
#include <vector>
#include <algorithm>

std::vector<int> createRandomVector(int  sz);
void blockMergeArrays(int *arr1, int *arr2, int size1, int size2);
void oddEvenParallelSort(int *arr, int arr_size);
void bubbleSort(int *arr, int arr_size);
#endif  // MODULES_TASK_2_SHAGOV_M_ODD_EVEN_SORT_ODD_EVEN_SORT_H_
