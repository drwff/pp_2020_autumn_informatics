// Copyright 2020 Raevskaia Ekaterina
#ifndef MODULES_TASK_2_RAEVSKAIA_E_REDUCE_REDUCE_H_
#define MODULES_TASK_2_RAEVSKAIA_E_REDUCE_REDUCE_H_
#include <mpi.h>
#include <stdlib.h>
#include <stdio.h>
#include <vector>

typedef void (*func)(void* a, void* b, int);

void Reduce(
    void* where_to_send_from,
    void* where_to_send_to,
    int how_much,
    MPI_Datatype type,
    func op,
    int root,
    MPI_Comm comm
);

#endif  // MODULES_TASK_2_RAEVSKAIA_E_REDUCE_REDUCE_H_
