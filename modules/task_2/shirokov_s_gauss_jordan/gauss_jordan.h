// Copyright 2020 Shirokov Sergey
#ifndef MODULES_TASK_2_SHIROKOV_S_GAUSS_JORDAN_GAUSS_JORDAN_H_
#define MODULES_TASK_2_SHIROKOV_S_GAUSS_JORDAN_GAUSS_JORDAN_H_
#include <random>
#include <iostream>
#include <vector>
#include <utility>

void processInitialization(double** a, double** b, double** resMat,
    double** resVec, double** procRows, double** procVec, const int n, int* rowsCount);
void dataDistribution(double* a, double* b, double* procRows, double* procVec, const int n);
void parallelGJElimination(double* procRows, double* procVec, const int n, const int rowsCount);
void parallelDirectFlow(double* procRows, double* procVec, const int n, const int rowsCount);
void parallelReverseFlow(double* procRows, double* procVec,
    const int n, const int rowsCount);
void parallelDirectEliminateColumns(double* procRows, double* procVec,
    double* pivotRow, const int n, const int rowsCount, const int i);
void parallelReverseEliminateColumns(double* procRows, double* procVec,
    const double pivotElem, const int n, const int rowsCount, const int i);
void collectData(double* resMat, double* resVec, double* procRows, double* procVec, const int n);
void setArraysValues(int** count, int** indices, const int n);
void divideVec(double* vec, const int n, const double div);
void cpyVec(double* source, double* dest, const int n);
void subVecs(double* minuend, double* sub, const int n, const double mult);
double* createVec(const int n, const int max);
void printMatrix(double* a, double* b, const int rowsCount, const int colsCount);
void swapRows(double* r1, double* r2, const int n);
void swap(double* v1, double* v2);

#endif  // MODULES_TASK_2_SHIROKOV_S_GAUSS_JORDAN_GAUSS_JORDAN_H_
