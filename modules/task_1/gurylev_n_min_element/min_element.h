// Copyright 2020 Gurylev Nikita
#ifndef MODULES_TASK_1_GURYLEV_N_MIN_ELEMENT_MIN_ELEMENT_H_
#define MODULES_TASK_1_GURYLEV_N_MIN_ELEMENT_MIN_ELEMENT_H_

#include <iostream>
#include <vector>
#include <ctime>
#include <random>

std::vector<int> getRandomMatrix(int row, int column);
int getSequentialMin(const std::vector<int>& local_vec);
int getParallelMin(const std::vector<int>& matrix, int row, int column);

#endif  // MODULES_TASK_1_GURYLEV_N_MIN_ELEMENT_MIN_ELEMENT_H_
