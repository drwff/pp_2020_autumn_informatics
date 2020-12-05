// Copyright 2020 Dobrov Pavel
#ifndef MODULES_TASK_2_DOBROV_P_GRID_TORUS_GRID_TORUS_H_
#define MODULES_TASK_2_DOBROV_P_GRID_TORUS_GRID_TORUS_H_

#include <mpi.h>

MPI_Comm getTorusComm(const MPI_Comm old, int height = 0, int width = 0);

bool testTorusTopology(const MPI_Comm t_comm);

bool testCommunications(const MPI_Comm t_comm);

#endif  // MODULES_TASK_2_DOBROV_P_GRID_TORUS_GRID_TORUS_H_
