// Copyright 2020 Gurylev Nikita
#include <mpi.h>
#include <vector>
#include <random>
#include <ctime>
#include <numeric>
#include <iostream>
#include <cstring>
#include"../../../modules/task_2/gurylev_n_gauss_jordan/gauss_jordan.h"

int getChunk(int total, int commsize, int rank) {
    int n = total;
    int q = n / commsize;
    if (n % commsize)
        q++;
    int r = commsize * q - n;
    int chunk = q;
    if (rank >= commsize - r)
        chunk = q - 1;
    return chunk;
}

double* getParallelGaussJordan(int n, int nrows, int* rows, double* a, double* x, double* tmp) {
    int rank, commsize;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &commsize);
    int row = 0;
    for (int i = 0; i < n - 1; i++) {
        if (i == rows[row]) {
            MPI_Bcast(&a[row * (n + 1)], n + 1, MPI_DOUBLE, rank, MPI_COMM_WORLD);
            for (int j = 0; j <= n; j++)
                tmp[j] = a[row * (n + 1) + j];
            row++;
        } else {
            MPI_Bcast(tmp, n + 1, MPI_DOUBLE, i % commsize, MPI_COMM_WORLD);
        }
        for (int j = row; j < nrows; j++) {
            double scaling = a[j * (n + 1) + i] / tmp[i];
            for (int k = i; k < n + 1; k++)
                a[j * (n + 1) + k] -= scaling * tmp[k];
        }
    }
    row = 0;
    for (int i = 0; i < n; i++) {
        x[i] = 0;
        if (i == rows[row]) {
            x[i] = a[row * (n + 1) + n];
            row++;
        }
    }
    row = nrows - 1;
    for (int i = n - 1; i > 0; i--) {
        if (row >= 0) {
            if (i == rows[row]) {
                x[i] /= a[row * (n + 1) + i];
                MPI_Bcast(&x[i], 1, MPI_DOUBLE, rank, MPI_COMM_WORLD);
                row--;
            } else {
                MPI_Bcast(&x[i], 1, MPI_DOUBLE, i % commsize, MPI_COMM_WORLD);
            }
        } else {
            MPI_Bcast(&x[i], 1, MPI_DOUBLE, i % commsize, MPI_COMM_WORLD);
        }
        for (int j = 0; j <= row; j++)
            x[rows[j]] -= a[j * (n + 1) + i] * x[i];
    }
    if (rank == 0)
        x[0] /= a[row * (n + 1)];
    MPI_Bcast(x, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    return x;
}
