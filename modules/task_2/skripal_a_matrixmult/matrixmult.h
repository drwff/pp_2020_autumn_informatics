// Copyright 2020 Skripal Andrey
#ifndef MODULES_TASK_2_SKRIPAL_A_MATRIXMULT_MATRIXMULT_H_
#define MODULES_TASK_2_SKRIPAL_A_MATRIXMULT_MATRIXMULT_H_
#include <vector>

std::vector<double> transposition(std::vector<double> B, int size);
std::vector<double> sequentialMatrixMult(std::vector<double> A, std::vector<double> B, int size);
std::vector<double> genMatrix(int size);
std::vector<double> parallelMatrixMult(const std::vector<double> &A, const std::vector<double> &B, int size);

#endif  //  MODULES_TASK_2_SKRIPAL_A_MATRIXMULT_MATRIXMULT_H_
