// Copyright 2020 Kiseleva Anastasia
#ifndef MODULES_TASK_2_KISELEVA_GAUS_ZHORDAN_ZHORDAN_GAUSS_H_
#define MODULES_TASK_2_KISELEVA_GAUS_ZHORDAN_ZHORDAN_GAUSS_H_

#include <limits.h>
#include <string>
#include <random>
#include <ctime>
#include <algorithm>
#include <iostream>

double* matr(double* array, int str);
void str_del_temp(double* array, int stlb, double temp, int j);
void str_umn_temp(double* array, int stlb, double temp, int j);
void str_minus_str(double* array1, double* array2, int stlb, int j);
double* zhordan_gauss(double* array, int str);
bool Check(double* array, int str, double* ans, double epsilon);
double*posled(double* array, int str);
#endif  // MODULES_TASK_2_KISELEVA_GAUS_ZHORDAN_ZHORDAN_GAUSS_H_
