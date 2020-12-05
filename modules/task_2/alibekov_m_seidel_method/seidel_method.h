// Copyright 2020 Alibekov Murad
#ifndef MODULES_TASK_2_ALIBEKOV_M_SEIDEL_METHOD_SEIDEL_METHOD_H_
#define MODULES_TASK_2_ALIBEKOV_M_SEIDEL_METHOD_SEIDEL_METHOD_H_

#include <vector>

const double epsilon = 0.0001;
const int max_count = 100;

std::vector<double> generate_A(int size);
std::vector<double> generate_b(int size);
double d(const std::vector<double>& x, const std::vector<double>& y);
double parallel_dot_product(const std::vector<double>& x, const std::vector<double>& y);

std::vector<double> solving_SLAE_sequential(const std::vector<double>& A, const std::vector<double>& b);
std::vector<double> solving_SLAE_parallel(const std::vector<double>& A, const std::vector<double>& b);

#endif  // MODULES_TASK_2_ALIBEKOV_M_SEIDEL_METHOD_SEIDEL_METHOD_H_
