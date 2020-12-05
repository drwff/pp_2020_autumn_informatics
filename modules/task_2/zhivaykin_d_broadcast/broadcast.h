// Copyright 2020 Zhivaykin Daniil
#ifndef MODULES_TASK_2_ZHIVAYKIN_D_BROADCAST_BROADCAST_H_
#define MODULES_TASK_2_ZHIVAYKIN_D_BROADCAST_BROADCAST_H_

#include <mpi.h>
#include <stdexcept>

void myBcast(void *buf, int count, MPI_Datatype type, int root, MPI_Comm comm);

#endif  // MODULES_TASK_2_ZHIVAYKIN_D_BROADCAST_BROADCAST_H_
