// Copyright 2020 Tyurmina Alexandra
#ifndef MODULES_TASK_2_TYURMINA_A_ALLREDUCE_ALLREDUCE_H_
#define MODULES_TASK_2_TYURMINA_A_ALLREDUCE_ALLREDUCE_H_

#include <mpi.h>
#include <string.h>
#include <cmath>
#include <iostream>
#include <vector>
#include <functional>

int My_Allreduce(void* sendbuf, void* recvbuf, int count, MPI_Datatype datatype,
    MPI_Op op, MPI_Comm comm);
#endif  // MODULES_TASK_2_TYURMINA_A_ALLREDUCE_ALLREDUCE_H_

