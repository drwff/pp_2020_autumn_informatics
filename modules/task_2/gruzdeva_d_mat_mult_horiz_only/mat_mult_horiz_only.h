// Copyright 2020 Gruzdeva Diana
#ifndef MODULES_TASK_2_GRUZDEVA_D_MAT_MULT_HORIZ_ONLY_MAT_MULT_HORIZ_ONLY_H_
#define MODULES_TASK_2_GRUZDEVA_D_MAT_MULT_HORIZ_ONLY_MAT_MULT_HORIZ_ONLY_H_

#include <mpi.h>
#include <random>
#include <ctime>
#include <vector>

std::vector<double> getRandomMatrix(int rows, int cols, time_t seed);
std::vector<double> getSequentialMultiplication(std::vector<double> matrixA,
                    std::vector<double> matrixB, int aRows, int aCols, int bCols);
std::vector<double> getParallelMultiplication(std::vector<double> matrixA,
                    std::vector<double> matrixB, int aRows, int aCols, int bCols);

#endif  // MODULES_TASK_2_GRUZDEVA_D_MAT_MULT_HORIZ_ONLY_MAT_MULT_HORIZ_ONLY_H_
