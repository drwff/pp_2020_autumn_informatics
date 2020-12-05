// Copyright 2020 Zhuravlev Roman
#ifndef  MODULES_TASK_2_ZHURAVL_R_GATHER_GATHER_H_
#define  MODULES_TASK_2_ZHURAVL_R_GATHER_GATHER_H_

#include <cstring>

int Gather(void *sbuf, int scount, MPI_Datatype stype, void *rbuf,
int count, MPI_Datatype rtype, int root, MPI_Comm comm);

#endif  // MODULES_TASK_2_ZHURAVL_R_GATHER_GATHER_H_
