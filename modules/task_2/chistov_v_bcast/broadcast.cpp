// Copyright 2020 Chistov Vladimir

#include <mpi.h>
#include <iostream>
#include"../../../modules/task_2/chistov_v_bcast/broadcast.h"

int MY_Bcast(void* buf, int count, MPI_Datatype type, int root, MPI_Comm comm) {
    int ProcNum, ProcRank, size;
    MPI_Comm_size(comm, &ProcNum);
    MPI_Comm_rank(comm, &ProcRank);
    MPI_Type_size(type, &size);
    if ((root < 0) || (root >= ProcNum)) {
        return MPI_ERR_ROOT;
    }
    if (ProcRank == root) {
        for (int i = 0; i < ProcNum; i++) {
            if (i != root) {
                MPI_Send(buf, count, type, i, 0, comm);
            }
        }
    }
    if (ProcRank != root) {
            MPI_Status status;
            MPI_Recv(buf, count, type, root, 0, comm, &status);
    }
    return 0;
}
