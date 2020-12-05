// Copyright 2020 Pronin Igor
#include <mpi.h>
#include "../../../modules/task_2/pronin_i_rule/rule.h"
#define ndims 1
MPI_Comm createRuleComm(MPI_Comm comunicator) {
    int size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm  RuleComm;
    int dims[ndims]{ 0 };
    int periods[ndims]{ 0 };
    MPI_Dims_create(size, ndims, dims);
    MPI_Cart_create(MPI_COMM_WORLD, ndims, dims, periods, 0, &RuleComm);
    return RuleComm;
}
