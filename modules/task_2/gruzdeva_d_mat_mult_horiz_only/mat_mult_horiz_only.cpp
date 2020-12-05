// Copyright 2020 Gruzdeva Diana

#include "../../../modules/task_2/gruzdeva_d_mat_mult_horiz_only/mat_mult_horiz_only.h"
#include <random>
#include <vector>
#include <ctime>
#include <iostream>

std::vector<double> getRandomMatrix(int rows, int cols, time_t seed) {
    std::vector<double> mat(rows * cols);
    std::mt19937 gen(seed);
    std::uniform_real_distribution<double> dis(-10, 10);
    for (int i = 0; i < rows * cols; i++) {
        mat[i] = dis(gen);
    }
    return mat;
}


std::vector<double> getSequentialMultiplication(std::vector<double> matrixA,
                    std::vector<double> matrixB, int aRows, int aCols, int bCols) {
    std::vector<double> matrixC(matrixA.size() / aCols * matrixB.size() / aCols);
    for (unsigned int i = 0; i < matrixA.size() / aCols; i++) {
        for (int j = 0; j < bCols; j++) {
            matrixC[i * bCols + j] = 0;
            for (int k = 0; k < aCols; k++) {
                matrixC[i * bCols + j] += matrixA[i * aCols + k] * matrixB[k * bCols + j];
            }
        }
    }
    return matrixC;
}

std::vector<double> getParallelMultiplication(std::vector<double> matrixA,
                    std::vector<double> matrixB, int aRows, int aCols, int bCols) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int stripeHeight = aRows / size;
    int stripeSize = stripeHeight * aCols;
    std::vector<double> matrixC(aRows * aRows);

    std::vector<double> matrixAStripe(stripeHeight * aCols);
    std::vector<double> matrixBStripe(aRows * bCols);
    std::vector<double> matrixCStripe;
    MPI_Status status;

    if (rank == 0) {
        for (int i = 1; i < size; i++) {
            MPI_Send(&matrixA[0] + i * stripeSize, stripeSize, MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
            MPI_Send(&matrixB[0], aCols * bCols, MPI_DOUBLE, i, 1, MPI_COMM_WORLD);
        }
    }

    if (rank == 0) {
        matrixAStripe = std::vector<double>(matrixA.begin(), matrixA.begin() + stripeSize);
        matrixBStripe = std::vector<double>(matrixB.begin(), matrixB.begin() + aCols * bCols);
    } else {
        MPI_Recv(&matrixAStripe[0], stripeSize, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &status);
        MPI_Recv(&matrixBStripe[0], aCols * bCols,
            MPI_DOUBLE, 0, 1, MPI_COMM_WORLD, &status);
    }

    matrixCStripe = getSequentialMultiplication(matrixAStripe,
                    matrixBStripe, aRows, aCols, bCols);

    MPI_Allgather(matrixCStripe.data(), stripeSize,
        MPI_DOUBLE, matrixC.data(), stripeSize, MPI_DOUBLE, MPI_COMM_WORLD);

    if (rank == 0) {
        matrixAStripe = std::vector<double>(matrixA.begin() + size * stripeSize, matrixA.end());
        std::vector<double> remaining = getSequentialMultiplication(matrixAStripe, matrixBStripe, aRows, aCols, bCols);
        for (unsigned int i = 0; i < remaining.size(); i++) {
            matrixC[size * stripeSize + i] = remaining[i];
        }
    }

    return matrixC;
}
