// Copyright 2020 Paranicheva Alyona
#ifndef MODULES_TASK_2_PARANICHEVA_A_GATHER_GATHER_H_
#define MODULES_TASK_2_PARANICHEVA_A_GATHER_GATHER_H_

#include <vector>

int Gather(void* sbuf, int scount, MPI_Datatype stype,
    void* rbuf, int rcount, MPI_Datatype rtype, int root, MPI_Comm comm);
std::vector<int> getRandomArrInt(int number, int rank);
std::vector<double> getRandomArrDouble(int number, int rank);

#endif  // MODULES_TASK_2_PARANICHEVA_A_GATHER_GATHER_H_
