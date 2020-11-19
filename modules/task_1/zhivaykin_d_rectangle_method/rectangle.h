// Copyright 2020 Zhivaykin Daniil
#ifndef MODULES_TASK_1_ZHIVAYKIN_D_RECTANGLE_METHOD_RECTANGLE_H_
#define MODULES_TASK_1_ZHIVAYKIN_D_RECTANGLE_METHOD_RECTANGLE_H_

#include <mpi.h>
#include <functional>

double def_int(std::function<double(double)> funct, double a, double b, int count);
double parall_int(std::function<double(double)> funct, double a, double b, int count);

double func(double x);
#endif  // MODULES_TASK_1_ZHIVAYKIN_D_RECTANGLE_METHOD_RECTANGLE_H_
