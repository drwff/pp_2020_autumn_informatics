// Copyright 2020 Nadir mohammed
#ifndef MODULES_TASK_2_METHOD_GAUSS_NADIR_METHODGAUSS_H_
#define MODULES_TASK_2_METHOD_GAUSS_NADIR_METHODGAUSS_H_

#include<vector>


double* methodGauss(std::vector<std::vector<double>> array, double* solution, int row);
void methodGaussParallel(const double* array, double* solution, int row, int col);

#endif  // MODULES_TASK_2_METHOD_GAUSS_NADIR_METHODGAUSS_H_
