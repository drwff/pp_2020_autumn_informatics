// Copyright 2020 Solovev Aleksandr
#include <mpi.h>
#include <algorithm>
#include <vector>
#include "../../../modules/task_2/solovev_a_scatter/scatter.h"

int My_Scatter(void* send_data, int send_count, MPI_Datatype send_datatype, void* recv_data,
    int recv_count, MPI_Datatype recv_datatype, int root, MPI_Comm communicator) {
    if (send_count <= 0)
        return MPI_ERR_COUNT;
    if (recv_count <= 0)
        return MPI_ERR_COUNT;
    if (send_count != recv_count)
        return MPI_ERR_COUNT;
    if (root < 0)
        return MPI_ERR_ROOT;
    if (send_data == nullptr)
        return MPI_ERR_BUFFER;
    int size, rank;
    int elemSizer, elemSizes;

    MPI_Comm_size(communicator, &size);
    MPI_Comm_rank(communicator, &rank);

    MPI_Type_size(send_datatype, &elemSizes);
    MPI_Type_size(recv_datatype, &elemSizer);

    if (rank == root) {
        memcpy(recv_data, reinterpret_cast<int8_t*>(send_data) + root * send_count * elemSizer, send_count * elemSizes);
        for (int i = 0; i < size; i++) {
            if (i != root)
                MPI_Send(reinterpret_cast<int8_t*>(send_data) + i * send_count * elemSizes,
                send_count, send_datatype, i, 0, communicator);
        }
    } else {
        MPI_Status status;
        MPI_Recv(recv_data, recv_count, recv_datatype, root, 0, communicator, &status);
    }
    return MPI_SUCCESS;
}
