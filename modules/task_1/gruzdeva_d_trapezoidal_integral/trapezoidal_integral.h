// Copyright 2020 Gruzdeva Diana
#ifndef MODULES_TASK_1_GRUZDEVA_D_TRAPEZOIDAL_INTEGRAL_TRAPEZOIDAL_INTEGRAL_H_
#define MODULES_TASK_1_GRUZDEVA_D_TRAPEZOIDAL_INTEGRAL_TRAPEZOIDAL_INTEGRAL_H_

#include <mpi.h>
#include <cmath>
#include <functional>

double getSequentialIntegral(double a, double b, int n, std::function<double(double)> function);
double getParallelIntegral(double a, double b, int n, std::function<double(double)> function);

double callFunction(std::function<double(double)> function, double value);
double polinomFunction(double x);
double compositeFunction(double x);

#endif  // MODULES_TASK_1_GRUZDEVA_D_TRAPEZOIDAL_INTEGRAL_TRAPEZOIDAL_INTEGRAL_H_
