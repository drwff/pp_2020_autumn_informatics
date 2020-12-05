// Copyright 2020 Loganov Andrei
#include "../../../../modules/task_2/loganov_a_smoothing/smoothing.h"
#include <stdlib.h>
#include <mpi.h>
#include <vector>
#include <random>
#include <ctime>
#include <numeric>
#include <iostream>
#include <algorithm>
#include <functional>
void arrdel(int** arr, int rows1) {
    for (int i = 0; i < rows1; i++) {
        delete[] arr[i];
    }
    delete[] arr;
}
int clamp(int v, int max, int min) {
    if (v > max) return max;
    else if (v < min) return min;
    return v;
}
int calculateavg(int** im, int x, int y, int radius, int cols, int rows) {
    int size = 2 * radius + 1;
    int* vec = new int[size * size];
    for (int i = -radius; i <= radius; i++) {
        for (int j = -radius; j <= radius; j++) {
            int idx = (i + radius) * size + j + radius;
            vec[idx] = im[clamp(x + j, rows - 1, 0)][clamp(y + i, cols - 1, 0)];
        }
    }
    std::sort(&vec[0], &vec[0] + size * size, std::greater<int>());
    return ((vec[0] + vec[(size * size - 1)]) / 2);
}
int** avg_point(int** im, int cols, int rows, int radius) {
    int** imres;
    imres = new int* [rows];
    for (int i = 0; i < rows; i++) {
        imres[i] = new int[cols];
    }
    for (int x = 0; x < rows; x++) {
        for (int y = 0; y < cols; y++) {
            imres[x][y] = calculateavg(im, x, y,  radius, cols, rows);
        }
    }
    return imres;
}
int** ParallelSmoothing(int** image, int cols, int rows, int radius) {
    int size;
    int rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int Delta = cols / size;
    int ost = cols % size;
    int* sendbuff = new int[cols];
    int* sendcounts = new int[size];
    int* disps = new int[size];
    for (int i = 0; i < cols; i++) {
        sendbuff[i] = i;
    }
    for (int i = 0; i < size; i++) {
         if (ost > 0) {
            sendcounts[i] = Delta + 1;
            ost--;
         } else {
            sendcounts[i] = Delta;
         }
    }
    int buff = 0;
    for (int i = 0; i < size; i++) {
        disps[i] = buff;
        buff += (sendcounts[i]);
    }
    std::vector<int> recivbuff(sendcounts[rank]);
    MPI_Scatterv(&sendbuff[0], sendcounts, disps, MPI_INT, &recivbuff[0], sendcounts[rank], MPI_INT, 0, MPI_COMM_WORLD);
    std::vector<int> resimk(static_cast<int>(recivbuff.size() * rows));
    for (int i = 0; i < static_cast<int>(recivbuff.size()); i++) {
        for (int j = 0; j < rows; j++) {
            resimk[i* rows+j] = calculateavg(image, j, recivbuff[i] ,  radius, cols, rows);
        }
    }
    std::vector<int> result(cols * rows);
    int* sendcountsR = new int[size];
    int* disps1 = new int[size];
    for (int i = 0; i < size; i++) {
        sendcountsR[i] = static_cast<int>(recivbuff.size()) * rows;
    }
    int buff1 = 0;
    for (int i = 0; i < size; i++) {
        disps1[i] = buff1*rows;
        buff1 += (sendcounts[i]);
    }
    MPI_Gatherv(&resimk[0], sendcountsR[rank], MPI_INT, &result[0], sendcountsR, disps1, MPI_INT, 0, MPI_COMM_WORLD);
    int** globalrez;
    globalrez = new int* [rows];
    for (int i = 0; i < rows; i++) {
        globalrez[i] = new int[cols];
    }
    if (rank == 0) {
        for (int i = 0; i < cols; i++) {
            for (int j = 0; j < rows; j++) {
                globalrez[j][i] =  result[i * rows + j];
            }
        }
    }
    return globalrez;
}

