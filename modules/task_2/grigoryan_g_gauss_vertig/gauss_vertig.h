// Copyright 2020 Grigoryan Garry

#ifndef MODULES_TASK_2_GRIGORYAN_G_GAUSS_VERTIG_GAUSS_VERTIG_H_
#define MODULES_TASK_2_GRIGORYAN_G_GAUSS_VERTIG_GAUSS_VERTIG_H_

#include <vector>
#include <ctime>
#include <random>

std::vector<double> ParrG(std::vector<double> matrix, int _size);
bool NullStr(std::vector<double> matrix, int str, int _size);
std::vector <double> RandomMatrix(int _size, time_t seed);
int maxind(int it, int _size, std::vector <double> matrix);
int Check(std::vector<double> matrix, int _size);
std::vector<double> OGauss(std::vector<double> matrix, int _size);

#endif  // MODULES_TASK_2_GRIGORYAN_G_GAUSS_VERTIG_GAUSS_VERTIG_H_
