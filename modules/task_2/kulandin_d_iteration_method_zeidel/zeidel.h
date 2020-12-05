// Copyright 2020 Kulandin Denis

#ifndef MODULES_TASK_2_KULANDIN_D_ITERATION_METHOD_ZEIDEL_ZEIDEL_H_
#define MODULES_TASK_2_KULANDIN_D_ITERATION_METHOD_ZEIDEL_ZEIDEL_H_

#include <vector>
#include <utility>

std::vector<double> randomVectorA(const size_t sz);
std::vector<double> randomVectorB(const size_t sz);
std::pair<bool, std::vector<double> > zeidelSequential(const std::vector<double>& a, const std::vector<double>& b,
                                                       size_t n, double eps);
std::pair<bool, std::vector<double> > zeidelParallel(const std::vector<double>& a, const std::vector<double>& b,
                                                     size_t n, double eps);

#endif  // MODULES_TASK_2_KULANDIN_D_ITERATION_METHOD_ZEIDEL_ZEIDEL_H_
