// Copyright 2020 Gurylev Nikita
#ifndef MODULES_TASK_2_GURYLEV_N_GAUSS_JORDAN_GAUSS_JORDAN_H_
#define MODULES_TASK_2_GURYLEV_N_GAUSS_JORDAN_GAUSS_JORDAN_H_

int getChunk(int total, int commsize, int rank);
double* getParallelGaussJordan(int n, int nrows, int* rows, double* a, double* x, double* tmp);

#endif  // MODULES_TASK_2_GURYLEV_N_GAUSS_JORDAN_GAUSS_JORDAN_H_
