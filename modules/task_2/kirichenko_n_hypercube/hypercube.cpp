// Copyright 2020 Kirichenko Nikita
#include <mpi.h>
#include <gtest-mpi-listener.hpp>
#include "../../../modules/task_2/kirichenko_n_hypercube/hypercube.h"

int logOfTwo(int n) {
    if (n < 0)
        throw "Error size";
    int i;
    for (i = 0; degTwo(i) < n; i++) {}
    return i;
}
int degTwo(int deg) {
    if (deg < 0)
        throw "Error size";
    int a = 1;
    for (int i = 0; i < deg; i++)
        a *= 2;
    return a;
}
int* createArr(int n, int a) {
    int* arr = new int[n];
    for (int i = 0; i < n; i++) {
        arr[i] = a;
    }
    return arr;
}
MPI_Comm hypercube(int n) {
    if (n < 1) {
        throw "Error size";
    }
    int size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    if (size == degTwo(n)) {
        int* dim = createArr(n, 2);
        int* per = createArr(n, 1);
        MPI_Comm cub;
        MPI_Cart_create(MPI_COMM_WORLD, n, dim, per, 1, &cub);
        return cub;
    } else {
        return MPI_COMM_WORLD;
    }
}
