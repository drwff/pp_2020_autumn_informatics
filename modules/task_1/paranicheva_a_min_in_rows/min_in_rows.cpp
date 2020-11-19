// Copyright 2020 Paranicheva Alyona
#include <mpi.h>
#include <ctime>
#include <algorithm>
#include <random>
#include <iostream>
#include <vector>
#include "../../../modules/task_1/paranicheva_a_min_in_rows/min_in_rows.h"

std::vector<int> getRandomMatrix(int rows, int cols) {
    std::mt19937 gen;
    gen.seed(static_cast<unsigned int>(time(0)));
    std::vector<int> array(rows * cols);
    for (int i = 0; i < rows * cols; i++)
        array[i] = gen() % 100;
    return array;
}

int getMinInRow(std::vector<int> array, int cols, int num) {
    int min = array[num * cols];
    for (int i = num * cols + 1; i < (num + 1) * cols; i++) {
        if (array[i] < min)
            min = array[i];
    }
    return min;
}

std::vector<int> getSequentialMin(std::vector<int> arr, int rows, int cols) {
    std::vector<int> minsarr(rows);
    for (int i = 0; i < rows; i++)
        minsarr[i] = getMinInRow(arr, cols, i);
    return minsarr;
}

std::vector<int> getParallMin(std::vector<int> arr, int rows, int cols) {
    int size, rank;
    std::vector<int> minsarr(rows);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int count = rows / size;
    int rem = rows % size;
    if (rank == 0) {
        for (int i = 1; i < size; i++)
            MPI_Send(arr.data() + (i * count + rem) * cols, count * cols, MPI_INT, i, 0, MPI_COMM_WORLD);
        std::vector<int> tmparr((count + rem) * cols);
        for (int i = 0; i < (count + rem) * cols; i++)
            tmparr[i] = arr[i];
        std::vector<int> tmpmins = getSequentialMin(tmparr, count + rem, cols);
        for (int i = 0; i < count + rem; i++)
            minsarr[i] = tmpmins[i];
    } else {
        std::vector<int> tmparr(count * cols);
        MPI_Status status;
        MPI_Recv(tmparr.data(), count * cols, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        std::vector<int> tmpmins = getSequentialMin(tmparr, count, cols);
        MPI_Send(tmpmins.data(), count, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }
    if (rank == 0) {
        for (int i = 1; i < size; i++) {
            MPI_Status status;
            MPI_Recv(minsarr.data() + rem + count * i, count, MPI_INT, i, 0, MPI_COMM_WORLD, &status);
        }
    }
    return minsarr;
}
