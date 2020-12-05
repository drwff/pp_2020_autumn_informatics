// Copyright 2020 Nikolaev Denis
#pragma once
#ifndef MODULES_TASK_2_NIKOLAEV_DENIS_RING_TOPOLOGIRING_H_
#define MODULES_TASK_2_NIKOLAEV_DENIS_RING_TOPOLOGIRING_H_
#include <mpi.h>
#include <random>
#include <ctime>
#include <iostream>
#define Reorder 0
#define NDims 1

MPI_Comm topologyRing(MPI_Comm oldComm);
int* randomizeArray(int size);
int* Send(int* local_arr1, int arraySize, int currentRank, int finiteRank, MPI_Comm ringTop);

#endif  //  MODULES_TASK_2_NIKOLAEV_DENIS_RING_TOPOLOGIRING_H_
