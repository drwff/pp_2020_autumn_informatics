// Copyright 2020 Chistov Vladimir
#ifndef MODULES_TASK_2_CHISTOV_V_BCAST_BROADCAST_H_
#define MODULES_TASK_2_CHISTOV_V_BCAST_BROADCAST_H_

int MY_Bcast(void* buf, int count, MPI_Datatype type, int root, MPI_Comm comm);

#endif  // MODULES_TASK_2_CHISTOV_V_BCAST_BROADCAST_H_
