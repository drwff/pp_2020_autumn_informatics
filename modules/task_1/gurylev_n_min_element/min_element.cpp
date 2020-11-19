// Copyright 2020 Gurylev Nikita
#include "../../../modules/task_1/gurylev_n_min_element/min_element.h"
#include <mpi.h>
#include <iostream>
#include <ctime>
#include <algorithm>
#include <vector>
#include <random>

std::vector<int> getRandomMatrix(int row, int column) {
    int size_m = row * column;
    std::mt19937 gen;
    gen.seed(static_cast<unsigned int>(time(0)));
    std::vector<int> rand_vec(size_m);
    for (int i = 0; i < size_m; i++) {
        rand_vec[i] = gen() % 100;
    }
    return rand_vec;
}

int getSequentialMin(const std::vector<int>& local_vec) {
    return *std::min_element(local_vec.begin(), local_vec.end());
}

int getParallelMin(const std::vector<int>& matrix, int row, int column) {
    int size_m = row * column;
    if (size_m <= 0) {
        throw "Unsuitable size";
    }
    int ProcNum, ProcRank;
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    double t1, t2;
    int delta = size_m / ProcNum;
    int remain = size_m % ProcNum;
    if (ProcRank == 0) {
        t1 = MPI_Wtime();
        for (int proc = 1; proc < ProcNum; proc++) {
            MPI_Send(matrix.data() + remain + proc * delta, delta, MPI_INT, proc, 0, MPI_COMM_WORLD);
        }
    }
    int local_size;
    if (ProcRank == 0) {
        local_size = delta + remain;
    } else {
        local_size = delta;
    }
    std::vector<int> local_vec(local_size);
    if (ProcRank == 0) {
        local_vec = std::vector<int>(matrix.begin(), matrix.begin() + local_size);
    } else {
        MPI_Status status;
        MPI_Recv(local_vec.data(), local_size, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
    }
    int local_min = getSequentialMin(local_vec);
    int global_min;
    MPI_Reduce(&local_min, &global_min, 1, MPI_INT, MPI_MIN, 0, MPI_COMM_WORLD);
    if (ProcRank == 0) {
        t2 = MPI_Wtime();
        std::cout << "Time = " << std::fixed << t2 - t1 << std::endl;
    }
    return global_min;
}
