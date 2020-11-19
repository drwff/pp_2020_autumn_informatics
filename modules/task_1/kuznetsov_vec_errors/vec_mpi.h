// Copyright 2020 Kuznetsov Nikita
#ifndef MODULES_TASK_1_KUZNETSOV_VEC_ERRORS_VEC_MPI_H_
#define MODULES_TASK_1_KUZNETSOV_VEC_ERRORS_VEC_MPI_H_

#include <vector>

std::vector<int> randV(int size);
std::vector<int> vecNormalize(std::vector<int> a);
int parallelVector(const std::vector<int>& a, int n);
int countError(const std::vector<int>& a);

#endif  // MODULES_TASK_1_KUZNETSOV_VEC_ERRORS_VEC_MPI_H_
