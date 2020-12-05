// Copyright 2020 Shirokov Sergey
#include <mpi.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <utility>
#include "./gauss_jordan.h"
#include "../../../modules/task_2/shirokov_s_gauss_jordan/gauss_jordan.h"
#define MAX_NUMBER 1000
#define ROOT 0

struct PivotChoice {
    double max;
    int procRank;
};
int procRank, procCount;
int* procPivotIter;
int gauss_jordan(int argc, char* argv[]) {
    double* a = NULL;
    double* b = NULL;
    double* resMat = NULL;
    double* resVec = NULL;
    double* procRows = NULL;
    double* procVec = NULL;
    int n = atoi(argv[1]);
    int rowsCount;
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
    MPI_Comm_size(MPI_COMM_WORLD, &procCount);
    processInitialization(&a, &b, &resMat, &resVec, &procRows, &procVec, n, &rowsCount);
    if (procRank == ROOT && n < 8) {
        printf("Source matrix:\n");
        printMatrix(a, b, n, n);
    }
    dataDistribution(a, b, procRows, procVec, n);
    parallelGJElimination(procRows, procVec, n, rowsCount);
    collectData(resMat, resVec, procRows, procVec, n);
    if (procRank == ROOT) {
        double* testRes = new double(n * sizeof(double));
        int maxPos;
        int i, j, k = 0;
        for (j = 0; j < n; ++j)
            for (i = 0; i < n; ++i)
                if (resMat[i * n + j] == 1.0) {
                    swapRows(&resMat[i * n], &resMat[k * n], n);
                    swap(&resVec[i], &resVec[k]);
                    ++k;
                }
        if (n < 8) {
            printf("Output matrix:\n");
            printMatrix(resMat, resVec, n, n);
        }
        for (i = 0; i < n; ++i)
            testRes[i] = 0.0;
        for (i = 0; i < n; ++i)
            for (j = 0; j < n; ++j)
                testRes[i] += a[i * n + j] * resVec[j];
        maxPos = 0;
        for (i = 1; i < n; ++i)
            if (fabs(testRes[maxPos] - b[maxPos]) < fabs(testRes[i] - b[i]))
                maxPos = i;
        printf("Max delta = %2.14f", fabs(testRes[maxPos] - b[maxPos]));
        delete(testRes);
    }
    delete(a);
    delete(b);
    delete(resMat);
    delete(resVec);
    delete(procRows);
    delete(procVec);
    return 0;
}
void processInitialization(double** a, double** b, double** resMat, double** resVec,
    double** procRows, double** procVec, const int n, int* rowsCount) {
    int rowsRest, i;
    rowsRest = n;
    for (i = 0; i < procRank; ++i)
        rowsRest -= rowsRest / (procCount - i);
    *rowsCount = rowsRest / (procCount - procRank);
    *procRows = new double(*rowsCount * n * sizeof(double));
    *procVec = new double(*rowsCount * sizeof(double));
    if (procRank == ROOT) {
        *a = createVec(n * n, MAX_NUMBER);
        *b = createVec(n, MAX_NUMBER);
        *resMat = new double(n * n * sizeof(double));
        *resVec = new double(n * sizeof(double));
    }
    procPivotIter = new int(*rowsCount * sizeof(int));
    for (i = 0; i < *rowsCount; ++i)
        procPivotIter[i] = -1;
}
void dataDistribution(double* a, double* b, double* procRows,
    double* procVec, const int n) {
    int* sendCount, * sendIndices, i;
    setArraysValues(&sendCount, &sendIndices, n);
    MPI_Scatterv(a, sendCount, sendIndices, MPI_DOUBLE,
        procRows, sendCount[procRank], MPI_DOUBLE, ROOT, MPI_COMM_WORLD);
    for (i = 0; i < procCount; ++i) {
        sendCount[i] /= n;
        sendIndices[i] /= n;
    }
    MPI_Scatterv(b, sendCount, sendIndices, MPI_DOUBLE,
        procVec, sendCount[procRank], MPI_DOUBLE, ROOT, MPI_COMM_WORLD);
    free(sendCount);
    free(sendIndices);
}
void parallelGJElimination(double* procRows, double* procVec, const int n, const int rowsCount) {
    parallelDirectFlow(procRows, procVec, n, rowsCount);
    MPI_Barrier(MPI_COMM_WORLD);
    parallelReverseFlow(procRows, procVec, n, rowsCount);
}
void parallelDirectFlow(double* procRows, double* procVec, const int n, const int rowsCount) {
    double max;
    int pivotPos;
    struct PivotChoice procPivot, pivot;
    double* pivotRow = new double((n + 1) * sizeof(double));
    int i, j;
    for (i = 0; i < n; ++i) {
        max = 0.0;
        for (j = 0; j < rowsCount; ++j)
            if (max < fabs(procRows[j * n + i]) && procPivotIter[j] == -1) {
                max = fabs(procRows[j * n + i]);
                pivotPos = j;
            }
        procPivot.max = max;
        procPivot.procRank = procRank;
        MPI_Allreduce(&procPivot, &pivot, 1, MPI_DOUBLE_INT, MPI_MAXLOC, MPI_COMM_WORLD);
        if (pivot.max != 0.0) {
            if (procRank == pivot.procRank) {
                double div = procRows[pivotPos * n + i];
                procPivotIter[pivotPos] = i;
                divideVec(&procRows[pivotPos * n], n, div);
                procVec[pivotPos] /= div;
                cpyVec(&procRows[pivotPos * n], pivotRow, n);
                pivotRow[n] = procVec[pivotPos];
            }
            MPI_Bcast(pivotRow, n + 1, MPI_DOUBLE, pivot.procRank, MPI_COMM_WORLD);
            parallelDirectEliminateColumns(procRows, procVec, pivotRow, n, rowsCount, i);
        }
    }
    free(pivotRow);
}
void parallelReverseFlow(double* procRows, double* procVec, const int n, const int rowsCount) {
    int pivotPos;
    struct PivotChoice procPivot, pivot;
    double pivotElem;
    int i, j;
    for (i = n - 1; i >= 0; --i) {
        pivotPos = 0;
        for (j = 1; j < rowsCount; ++j)
            if (procPivotIter[pivotPos] < procPivotIter[j])
                pivotPos = j;
        procPivot.max = procPivotIter[pivotPos];
        procPivot.procRank = procRank;
        MPI_Allreduce(&procPivot, &pivot, 1, MPI_DOUBLE_INT, MPI_MAXLOC, MPI_COMM_WORLD);
        if (procRank == pivot.procRank) {
            procPivotIter[pivotPos] = -1;
            pivotElem = procVec[pivotPos];
        }
        MPI_Bcast(&pivotElem, 1, MPI_DOUBLE, pivot.procRank, MPI_COMM_WORLD);
        parallelReverseEliminateColumns(procRows, procVec, pivotElem, n, rowsCount, pivot.max);
    }
}
void parallelDirectEliminateColumns(double* procRows, double* procVec,
    double* pivotRow, const int n, const int rowsCount, const int i) {
    int j;
    for (j = 0; j < rowsCount; ++j)
        if (procPivotIter[j] == -1) {
            double mult = procRows[j * n + i];
            subVecs(&procRows[j * n], pivotRow, n, mult);
            procVec[j] -= (mult * pivotRow[n]);
        }
}
void parallelReverseEliminateColumns(double* procRows, double* procVec, const double pivotElem,
    const int n, const int rowsCount, const int i) {
    int j;
    for (j = 0; j < rowsCount; ++j)
        if (procPivotIter[j] != -1) {
            double mult = procRows[j * n + i];
            procRows[j * n + i] = 0.0;
            procVec[j] -= (mult * pivotElem);
        }
}
void divideVec(double* vec, const int n, const double div) {
    int i;
    for (i = 0; i < n; ++i)
        vec[i] /= div;
}
void subVecs(double* minuend, double* sub, const int n, const double mult) {
    int i;
    for (i = 0; i < n; ++i)
        minuend[i] -= (mult * sub[i]);
}
void cpyVec(double* source, double* dest, const int n) {
    int i;
    for (i = 0; i < n; ++i)
        dest[i] = source[i];
}
void printMatrix(double* a, double* b, const int rowsCount, const int colsCount) {
    int i, j;
    for (i = 0; i < rowsCount; ++i) {
        for (j = 0; j < colsCount; ++j)
            printf("%3.2f ", a[i * colsCount + j]);
        printf("\t%3.2f\n", b[i]);
    }
}
double* createVec(const int n, const int max) {
    std::random_device rd;
    std::mt19937 number(rd());
    double* vec;
    int i;
    vec = new double(n * sizeof(double));
    srand(time(0));
    for (i = 0; i < n; ++i)
        vec[i] = static_cast<int>(number() % max);
    return vec;
}
void collectData(double* resMat, double* resVec, double* procRows, double* procVec, const int n) {
    int* rcvCount, * rcvIndices, i;
    setArraysValues(&rcvCount, &rcvIndices, n);
    MPI_Gatherv(procRows, rcvCount[procRank], MPI_DOUBLE,
        resMat, rcvCount, rcvIndices, MPI_DOUBLE, ROOT, MPI_COMM_WORLD);
    for (i = 0; i < procCount; ++i) {
        rcvCount[i] /= n;
        rcvIndices[i] /= n;
    }
    MPI_Gatherv(procVec, rcvCount[procRank], MPI_DOUBLE, resVec,
        rcvCount, rcvIndices, MPI_DOUBLE, ROOT, MPI_COMM_WORLD);
    free(rcvCount);
    free(rcvIndices);
}
void setArraysValues(int** count, int** indices, const int n) {
    int rowsCount, rowsRest, i;
    *count = new int(procCount * sizeof(int));
    *indices = new int(procCount * sizeof(int));
    rowsRest = n;
    rowsCount = n / procCount;
    (*count)[0] = rowsCount * n;
    (*indices)[0] = 0;
    for (i = 1; i < procCount; ++i) {
        rowsRest -= rowsCount;
        rowsCount = rowsRest / (procCount - i);
        (*count)[i] = rowsCount * n;
        (*indices)[i] = (*indices)[i - 1] + (*count)[i - 1];
    }
}
void swapRows(double* r1, double* r2, const int n) {
    int i;
    for (i = 0; i < n; ++i)
        swap(&r1[i], &r2[i]);
}
void swap(double* v1, double* v2) {
    double tmp = *v1;
    *v1 = *v2;
    *v2 = tmp;
}
