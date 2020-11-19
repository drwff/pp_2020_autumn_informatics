// Copyright 2020 Schekotilova Julia
#ifndef MODULES_TASK_1_SCHEKOTILOVA_JU_ALTER_SIGN_ALTER_SIGN_H_
#define MODULES_TASK_1_SCHEKOTILOVA_JU_ALTER_SIGN_ALTER_SIGN_H_

#include <mpi.h>
#include <vector>
#include <random>
#include <ctime>
#include <algorithm>

std::vector<int> getRandomVector(int  sz);
int getParallelOperations(std::vector<int> vec, int count_size_vec);
int getSequentialOperations(std::vector<int> v, int size_v);

#endif  // MODULES_TASK_1_SCHEKOTILOVA_JU_ALTER_SIGN_ALTER_SIGN_H_
