// Copyright 2020 Zhivaykin Daniil
#include "../../../modules/task_2/zhivaykin_d_broadcast/broadcast.h"

void myBcast(void *buf, int count, MPI_Datatype type, int root, MPI_Comm comm) {
  int size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if (rank == root) {
    for (int i = 0; i < size; i++) {
      if (i != rank) {
        MPI_Send(buf, count, type, i, 0, comm);
      }
    }
  } else {
    MPI_Status status;
    MPI_Recv(buf, count, type, root, 0, comm, &status);
  }
}
