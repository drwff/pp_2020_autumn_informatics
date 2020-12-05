// Copyright 2020 Skripal Andrey
#include <mpi.h>
#include <vector>
#include <ctime>
#include <random>
#include <iostream>
#include <utility>
#include "../../../modules/task_2/skripal_a_matrixmult/matrixmult.h"

std::vector<double> transposition(std::vector<double> B, int size) {
    for (int i = 0; i < size; i++) {
        for (int j = i + 1; j < size; j++) {
            std::swap(B[i*size + j], B[j*size + i]);
        }
    }
    return B;
}

std::vector<double> sequentialMatrixMult(std::vector<double> A, std::vector<double> B, int size) {
    std::vector<double> C(size*size);
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            for (int k = 0; k < size; k++) {
                C[i*size + j] += A[i*size + k] * B[j*size + k];
            }
        }
    }
    return C;
}

std::vector<double> genMatrix(int size) {
    std::mt19937 gen;
    gen.seed(static_cast<unsigned int>(time(0)));
    std::vector<double> matrix(size*size);
    for (int i = 0; i < size*size; i++) {
        matrix[i] = gen() % 100;
    }
    return matrix;
}

std::vector<double> parallelMatrixMult(const std::vector<double> &A, const std::vector<double> &B, int size) {
    int procnum, procrank;
    MPI_Comm_size(MPI_COMM_WORLD, &procnum);
    MPI_Comm_rank(MPI_COMM_WORLD, &procrank);
    MPI_Status status;

    double tmp = 0.0;
    int index;
    int procpart = size / procnum;
    int partelem = procpart * size;

    std::vector<double> C(size*size);
    std::vector<double> localA(partelem);
    std::vector<double> localB(partelem);
    std::vector<double> localC(partelem);

    MPI_Scatter(A.data(), partelem, MPI_DOUBLE, localA.data(), partelem, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Scatter(B.data(), partelem, MPI_DOUBLE, localB.data(), partelem, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    for (int i = 0; i < procpart; i++) {
        for (int j = 0; j < procpart; j++) {
            for (int k = 0; k < size; k++) {
                tmp += localA[i*size + k] * localB[j*size + k];
            }
            localC[i*size + j + procpart * procrank] = tmp;
            tmp = 0.0;
        }
    }

    int nextproc, prevproc;
    for (int p = 1; p < procnum; p++) {
        nextproc = procrank + 1;
        prevproc = procrank - 1;

        if (procrank == procnum - 1) {
            nextproc = 0;
        }

        if (procrank == 0) {
            prevproc = procnum - 1;
        }

        MPI_Sendrecv_replace(localB.data(), partelem, MPI_DOUBLE, nextproc, 0, prevproc, 0, MPI_COMM_WORLD, &status);

        tmp = 0.0;
        for (int i = 0; i < procpart; i++) {
            for (int j = 0; j < procpart; j++) {
                for (int k = 0; k < size; k++) {
                    tmp += localA[i*size + k] * localB[j*size + k];
                }
                if (procrank - p >= 0) {
                    index = procrank - p;
                } else {
                    index = procnum - p + procrank;
                }
                localC[i*size + j + index * procpart] = tmp;
                tmp = 0.0;
            }
        }
    }

    MPI_Gather(localC.data(), partelem, MPI_DOUBLE, C.data(), partelem, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    return C;
}

