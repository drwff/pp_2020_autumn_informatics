// Copyright 2020 Schekotilova Julia
#ifndef MODULES_TASK_2_SCHEKOTILOVA_JU_SEIDEL_METHOD_SEIDEL_METHOD_H_
#define MODULES_TASK_2_SCHEKOTILOVA_JU_SEIDEL_METHOD_SEIDEL_METHOD_H_

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <random>
#include <ctime>
#include <algorithm>
#include <cstdlib>

std::vector<double> getRandomVector(int sz);
double evklid_norma(const std::vector<double>& vec);
bool CompareTwoVectors(const std::vector<double>& vec1,
  const std::vector<double>& vec2, const int& size, double eps);
std::vector<double> getSequentialOperations(const std::vector<double>& a,
  const std::vector<double>& b, int n, double eps);
std::vector<double> getParallelOperations(const std::vector<double>& matrix,
  const std::vector<double>& _b, int _n, double eps);

#endif  // MODULES_TASK_2_SCHEKOTILOVA_JU_SEIDEL_METHOD_SEIDEL_METHOD_H_

