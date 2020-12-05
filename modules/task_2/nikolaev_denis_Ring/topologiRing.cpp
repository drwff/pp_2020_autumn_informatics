// Copyright 2020 Nikolaev Denis
#include "../../../modules/task_2/nikolaev_denis_Ring/topologiRing.h"
#include <mpi.h>

int* randomizeArray(int size) {
    std::mt19937 gen;
    gen.seed(static_cast<unsigned int>(time(0)));
    int* array = new int[size];
    for (int i = 0; i < size; i++) {
        array[i] = gen() % 2000;
    }
    return array;
}
MPI_Comm topologyRing(MPI_Comm oldComm) {
    int* dims = new int[NDims];
    int* periods = new int[NDims];
    periods[0] = 1;
    MPI_Comm_size(oldComm, &dims[0]);
    MPI_Comm ringTop;
    MPI_Cart_create(oldComm, NDims, dims, periods, Reorder, &ringTop);
    return ringTop;
}
int* Send(int* local_arr1, int arraySize, int currentRank, int finiteRank, MPI_Comm ringTop) {
    int* local_arr = new int[arraySize];
    for (int i = 0; i < arraySize; i++) {
        local_arr[i] = local_arr1[i];
    }
    int ProcNum, ProcRank;
    MPI_Comm_size(ringTop, &ProcNum);
    MPI_Comm_rank(ringTop, &ProcRank);
    int* res = new int[arraySize];
    if (currentRank == finiteRank) {
        return local_arr;
    }
    int local_current_rank, local_dest_rank;
    MPI_Status status;
    MPI_Request request;
    MPI_Cart_shift(ringTop, 0, 1, &local_current_rank, &local_dest_rank);
    if (ProcRank == currentRank) {
        MPI_Isend(&local_arr[0], arraySize, MPI_INT, local_dest_rank, 0, ringTop, &request);
        MPI_Wait(&request, &status);
        // free(local_arr);
        // free(res);
    } else if (currentRank > finiteRank &&
        ((ProcRank > currentRank && ProcRank < ProcNum) || (ProcRank < finiteRank))) {
        MPI_Irecv(&res[0], arraySize, MPI_INT, local_current_rank, 0, ringTop, &request);
        MPI_Wait(&request, &status);
        MPI_Isend(&local_arr[0], arraySize, MPI_INT, local_dest_rank, 0, ringTop, &request);
        MPI_Wait(&request, &status);
        // free(local_arr);
       // free(res);
    } else if (currentRank < finiteRank && ProcRank > currentRank && ProcRank < finiteRank) {
        MPI_Irecv(&res[0], arraySize, MPI_INT, local_current_rank, 0, ringTop, &request);
        MPI_Wait(&request, &status);
        MPI_Isend(&local_arr[0], arraySize, MPI_INT, local_dest_rank, 0, ringTop, &request);
        MPI_Wait(&request, &status);
        // free(local_arr);
        // free(res);
    } else if (ProcRank == finiteRank) {
        MPI_Irecv(&res[0], arraySize, MPI_INT, local_current_rank, 0, ringTop, &request);
        MPI_Wait(&request, &status);
        // free(local_arr);
        // free(res);
    }
    return res;
    // free(local_arr);
    // free(res);
}
