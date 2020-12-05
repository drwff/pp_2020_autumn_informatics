// Copyright 2020 Lobov Aleksandr
#ifndef MODULES_TASK_2_LOBOV_A_ZEIDEL_METHOD_ZEIDEL_METHOD_H_
#define MODULES_TASK_2_LOBOV_A_ZEIDEL_METHOD_ZEIDEL_METHOD_H_

#include <mpi.h>
#include <cstdlib>
#include <vector>
#include <random>
using std::vector;
double EvklidNormal(const std::vector<double>& x);

std::vector<double> ZeidelSeq(const vector<vector<double> >& A, const vector<double>& b, int n, double eps);
std::vector<double> ZeidelMPI(const vector<vector<double> >& A, const vector<double>& _b, int _n, double eps);

#endif  // MODULES_TASK_2_LOBOV_A_ZEIDEL_METHOD_ZEIDEL_METHOD_H_
