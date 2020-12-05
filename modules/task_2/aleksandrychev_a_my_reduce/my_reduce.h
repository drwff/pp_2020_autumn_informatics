// Copyright 2020 Aleksandychev Andrey
#ifndef MODULES_TASK_2_ALEKSANDRYCHEV_A_MY_REDUCE_MY_REDUCE_H_
#define MODULES_TASK_2_ALEKSANDRYCHEV_A_MY_REDUCE_MY_REDUCE_H_
#include <iostream>

int my_reduce(void* sendbuf, void* recvbuf, int count, MPI_Datatype datatype, MPI_Op op, int root, MPI_Comm comm);

#endif  // MODULES_TASK_2_ALEKSANDRYCHEV_A_MY_REDUCE_MY_REDUCE_H_
