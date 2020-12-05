// Copyright 2020 Bulychev Vladislav
#ifndef MODULES_TASK_2_BULYCHEV_V_SIMPLE_ITERATIONS_SIMPLE_ITERATIONS_H_
#define MODULES_TASK_2_BULYCHEV_V_SIMPLE_ITERATIONS_SIMPLE_ITERATIONS_H_

#include <vector>

std::vector<double> Simple_iterations_Method(std::vector<double> A, std::vector<double> b, double precision);
std::vector<double> MPI_Simple_iterations_Method(std::vector<double> A, std::vector<double> b, double precision);

#endif  // MODULES_TASK_2_BULYCHEV_V_SIMPLE_ITERATIONS_SIMPLE_ITERATIONS_H_
