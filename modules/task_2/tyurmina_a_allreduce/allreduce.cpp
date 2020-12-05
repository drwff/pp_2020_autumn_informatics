// Copyright 2020 Tyurmina Alexandra
#include <mpi.h>
#include "../../../modules/task_2/tyurmina_a_allreduce/allreduce.h"

int My_Allreduce(void* sendbuf, void* recvbuf, int count, MPI_Datatype datatype,
    MPI_Op op, MPI_Comm comm) {
    if (recvbuf == nullptr)
        return MPI_ERR_BUFFER;
    if (count <= 0)
        return MPI_ERR_COUNT;
    if ((datatype != MPI_INT) && (datatype != MPI_DOUBLE))
        return MPI_ERR_TYPE;
    if (op != MPI_SUM)
        return MPI_ERR_OP;

    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int* getbufInt;
    double* getbufDouble;

    if (rank != 0) {
        MPI_Send(sendbuf, count, datatype, 0, 0, comm);
    }

    if (rank == 0) {
        if (datatype == MPI_INT)
            memcpy(recvbuf, sendbuf, count * sizeof(int));
        if (datatype == MPI_DOUBLE)
            memcpy(recvbuf, sendbuf, count * sizeof(double));

        if (datatype == MPI_INT) {
            getbufInt = new int[count];
        } else if (datatype == MPI_DOUBLE) {
            getbufDouble = new double[count];
        }

        for (int proc = 1; proc < size; proc++) {
            MPI_Status status;
            if (datatype == MPI_INT)
                MPI_Recv(getbufInt, count, datatype, proc, 0, comm, &status);
            if (datatype == MPI_DOUBLE)
                MPI_Recv(getbufDouble, count, datatype, proc, 0, comm, &status);

            for (int i = 0; i < count; i++) {
                if (datatype == MPI_INT)
                    static_cast<int*>(recvbuf)[i] += getbufInt[i];
                if (datatype == MPI_DOUBLE)
                    static_cast<double*>(recvbuf)[i] += getbufDouble[i];
            }
        }

        for (int proc = 1; proc < size; proc++) {
            MPI_Send(recvbuf, count, datatype, proc, 0, comm);
        }
    }

    if (rank != 0) {
        MPI_Status status;
        MPI_Recv(recvbuf, count, datatype, 0, 0, comm, &status);
    }

    return MPI_SUCCESS;
}
