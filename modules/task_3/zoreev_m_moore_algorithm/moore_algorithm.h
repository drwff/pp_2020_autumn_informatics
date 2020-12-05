// Copyright 2020 Zoreev Mikhail
#ifndef MODULES_TASK_3_ZOREEV_M_MOORE_ALGORITHM_MOORE_ALGORITHM_H_
#define MODULES_TASK_3_ZOREEV_M_MOORE_ALGORITHM_MOORE_ALGORITHM_H_

#include <mpi.h>

#include <iostream>
#include <iomanip>
#include <ctime>
#include <random>
#include <stdexcept>

void randomGraphWithPath(size_t size, int64_t *graph);

void randomCompleteGraph(size_t size, int64_t *graph);

void printGraph(size_t size, int64_t *graph);
void printPredecessor(size_t size, uint64_t *predecessor);

void mooreAlgorithm(size_t size, int64_t *graph, int64_t *distance, uint64_t *predecessor, size_t root);

void mooreAlgorithmParallel(size_t size, int64_t *graph, int64_t *distance, uint64_t *predecessor, size_t root);

#endif  // MODULES_TASK_3_ZOREEV_M_MOORE_ALGORITHM_MOORE_ALGORITHM_H_
