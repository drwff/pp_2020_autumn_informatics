// Copyright 2020 Makarov Alexander
#ifndef MODULES_TASK_2_MAKAROV_A_JACOBI_METHOD_JACOBI_METHOD_H_
#define MODULES_TASK_2_MAKAROV_A_JACOBI_METHOD_JACOBI_METHOD_H_

#include <vector>

const double epsilon = 0.0001;
const int max_iter = 100;

std::vector<double> generate_A(int size);
std::vector<double> generate_b(int size);

double getMaxDistance(const std::vector<double>& v1,
                      const std::vector<double>& v2);

double calculateError(const std::vector<double>& A,
                      const std::vector<double>& x,
                      const std::vector<double>& b);

std::vector<double> solveJacobiSequential(const std::vector<double>& A,
                                          const std::vector<double>& b);
std::vector<double> solveJacobiParallel(const std::vector<double>& A,
                                        const std::vector<double>& b);

#endif  // MODULES_TASK_2_MAKAROV_A_JACOBI_METHOD_JACOBI_METHOD_H_
