// Copyright 2020 Luckyanchencko Ivan
#include <mpi.h>
#include <vector>
#include <random>
#include <ctime>
#include <numeric>
#include<iostream>
#include <cstring>
#include"../../../modules/task_2/lukianchencko_i_gather/gather.h"

int Gather(void *sbuf, int scount, MPI_Datatype stype,
void *rbuf, int rcount, MPI_Datatype rtype,
int root, MPI_Comm comm) {
    int ProcNum;
    int ProcRank;
    int size;
    MPI_Comm_rank(comm, &ProcRank);
    MPI_Comm_size(comm, &ProcNum);
    MPI_Type_size(stype, &size);
    if ((root < 0) || (root >= ProcNum)) {
        std::cout << "Invalid root" << std::endl;
        return MPI_ERR_ROOT;
    }
    int sizeCount = scount * size;
    if (ProcRank != root) {
        MPI_Send(sbuf, scount, stype, root, 1, comm);
    }
    if (ProcRank == root) {
        MPI_Status STATUS;
        int k = ProcRank * sizeCount;
        std::memcpy(static_cast<char*>(rbuf)+k, sbuf, sizeCount);
        for (int i = 0; i < ProcNum; i++) {
            if (i != root) {
            int x = i * sizeCount;
            MPI_Recv(static_cast<char*>(rbuf)+x, rcount, rtype, i, 1, comm, &STATUS);
            }
        }
    }
    return MPI_SUCCESS;
}
