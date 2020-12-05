// Copyright 2020 Kirichenko Nikita
#ifndef MODULES_TASK_2_KIRICHENKO_N_HYPERCUBE_HYPERCUBE_H_
#define MODULES_TASK_2_KIRICHENKO_N_HYPERCUBE_HYPERCUBE_H_

#include <mpi.h>

int logOfTwo(int n);
int degTwo(int d);
int* createArr(int n, int a);
MPI_Comm hypercube(int n);

#endif  // MODULES_TASK_2_KIRICHENKO_N_HYPERCUBE_HYPERCUBE_H_
