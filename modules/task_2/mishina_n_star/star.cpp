// Copyright 2020 Mishina Nadezhda
#include <mpi.h>
#include<algorithm>
#include "../../../modules/task_2/mishina_n_star/star.h"

MPI_Comm StarComm(int size) {
    int* index;
    int* edges;
    index = new int[size];
    edges = new int[2 * size - 2];
    for (int i = 0; i < size-1; i++) {
        index[i] = 1;
    }
    index[size - 1] = size - 1;
    for (int i = 0; i < size - 1; i++) {
        edges[i] = 0;
    }
    for (int i = size-1; i < 2*size - 2; i++) {
        edges[i] = i-size+2;
    }
    MPI_Comm starcomm;
    MPI_Graph_create(MPI_COMM_WORLD, size, index, edges, 0, &starcomm);
    return starcomm;
}
