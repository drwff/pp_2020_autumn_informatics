// Copyright 2020 Panova Olga
#include <mpi.h>
#include <vector>
#include <random>
#include <ctime>
#include <algorithm>
#include <iostream>
#include "../../../modules/task_1/panova_o_max_difference/max_difference.h"
std::vector<int> CreateVector(int size) {
    std::vector<int> v(size);
    std::mt19937 gen(time(0));
    gen.seed(static_cast<unsigned int>(time(0)));
    for (int i = 0; i < size; i++) v[i] = gen() % 100;
    return v;
}
int GetSequentialDifference(std::vector<int> v, int size) {
    int max = abs(v[1] - v[0]);
    for (int i = 0; i < size - 1; i++) {
        int v1 = abs(v[i] - v[i + 1]);
       if (v1 > max) {
          max = v1;
       }
    }
    return max;
}
int GetParallelDifference(std::vector<int> vec, int vec_size) {
    int p_size, p_rank;
    int m = 0, max = 0;
    std::vector<int> v;
    MPI_Comm_size(MPI_COMM_WORLD, &p_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &p_rank);
    const int d = static_cast<int>((vec_size-1) / p_size);
    const int r = static_cast<int>((vec_size-1) % p_size);
    if (p_rank == 0) {
        if (d != 0) {
            for (int dest = 1; dest < p_size; dest++)
                MPI_Send(&vec[0] + d * dest + r, d + 1, MPI_INT, dest, 0, MPI_COMM_WORLD);
        }
    }
    if (p_rank == 0) {
        if (d != 0) {
            v.resize(d + r + 1);
            v.assign(vec.begin(), vec.begin() + d + r + 1);
            m = GetSequentialDifference(v, v.size());
        } else {
            m = GetSequentialDifference(vec, vec_size);
        }
    } else if (d != 0) {
        v.resize(d + 1);
        MPI_Status status;
        MPI_Recv(&v[0], d + 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        m = GetSequentialDifference(v, v.size());
    }
    MPI_Reduce(&m, &max, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);
    return max;
}

