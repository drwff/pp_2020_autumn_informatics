// Copyright 2020 Taktaev Artem
#include "../../../modules/task_2/taktaev_a_hypercube/hypercube.h"

int getDimsCount(int proc_num) {
    if (proc_num <= 0) return -1;
    int dim_count = 0;

    while (proc_num > 1) {
        if (proc_num % 2 == 1) return -2;
        proc_num = proc_num / 2;
        dim_count++;
    }
    return dim_count;
}

MPI_Comm createHypercubeTopology(int dim_count) {
    if (dim_count <= 0) throw "dim_count <= 0";
    MPI_Comm hypercube_comm;
    int *dims = new int[dim_count];
    int *periods = new int[dim_count];

    for (int i = 0; i < dim_count; i++) {
        dims[i] = 2;
        periods[i] = 1;
    }
    MPI_Cart_create(MPI_COMM_WORLD, dim_count, dims, periods, 0, &hypercube_comm);
    delete[] dims;
    delete[] periods;

    return hypercube_comm;
}
