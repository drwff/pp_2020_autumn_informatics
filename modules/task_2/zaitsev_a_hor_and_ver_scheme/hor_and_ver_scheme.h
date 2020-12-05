// Copyright 2020 Zaitsev Andrey
#ifndef MODULES_TASK_2_ZAITSEV_A_HOR_AND_VER_SCHEME_HOR_AND_VER_SCHEME_H_
#define MODULES_TASK_2_ZAITSEV_A_HOR_AND_VER_SCHEME_HOR_AND_VER_SCHEME_H_

#include <vector>
#include <random>
#include <ctime>

std::vector<double> createRandomMatrix(int size, time_t seed);
std::vector<double> transposition(std::vector<double> matrix_B, int size);
double scalar_product_of_vectors(const std::vector<double>& vec1, const std::vector<double>& vec2);
std::vector<double> matrix_product_sequential(const std::vector<double>& matrix_A,
  const std::vector<double>& matrix_B, int size);
std::vector<double> matrix_product_parallel(const std::vector<double>& matrix_A,
  const std::vector<double>& matrix_B, int size);

#endif  // MODULES_TASK_2_ZAITSEV_A_HOR_AND_VER_SCHEME_HOR_AND_VER_SCHEME_H_
