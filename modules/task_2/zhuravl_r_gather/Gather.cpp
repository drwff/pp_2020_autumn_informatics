// Copyright 2020 Zhuravlev Roman
#include <mpi.h>
#include <iostream>
#include <cstring>
#include <numeric>
#include <vector>
#include <random>
#include"../../../modules/task_2/zhuravl_r_gather/Gather.h"

int Gather(void *sbuf, int scount, MPI_Datatype stype, void *rbuf,
int count, MPI_Datatype rtype, int root, MPI_Comm comm) {
    int rank, num, size;
    MPI_Comm_rank(comm, &rank);
    MPI_Comm_size(comm, &num);
    MPI_Type_size(stype, &size);
    if ((root < 0) || (root >= num)) {
        std::cout << "Invalid root" << std::endl;
        return MPI_ERR_ROOT;
    }
    int sizeCount = scount * size;
    if (rank != root) {
        MPI_Send(sbuf, scount, stype, root, 1, comm);
    }
    if (rank == root) {
        MPI_Status STATUS;
        int k = rank * sizeCount;
        std::memcpy(static_cast<char*>(rbuf)+k, sbuf, sizeCount);
        for (int i = 0; i < num; i++) {
            if (i != root) {
            int x = i * sizeCount;
            MPI_Recv(static_cast<char*>(rbuf)+x, count, rtype, i, 1, comm, &STATUS);
            }
        }
    }
    return MPI_SUCCESS;
}
