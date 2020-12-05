// Copyright 2020 Prokofeva Elizaveta
#include <mpi.h>
#include <algorithm>
#include <stdexcept>
#include <ctime>
#include "random"
#include "iostream"
#include "../../../modules/task_2/prokofeva_e_lin_topology/lin_topology.h"

int* getRandomVector(int size) {
    std::mt19937 random_generator;
    random_generator.seed(static_cast<unsigned int>(time(0)));
    int* vec = new int[size];
    for (int i = 0; i < size; i++) {
        vec[i] = random_generator() % 100;
    }
    return vec;
}

MPI_Comm create_comm(MPI_Comm comm) {
    int size;

    MPI_Comm_size(MPI_COMM_WORLD, &size);

    MPI_Comm graphcomm;

    int* index = new int[size];
    int* edges = new int[2 * (size - 1)];
    int reorder = 0;
    int k = 1;

    index[0] = 1;
    for (int i = 1; i < size - 1; i++) {
        index[i] = index[i - 1] + 2;
    }
    index[size - 1] = index[size - 2] + 1;

    edges[0] = 1;
    for (int i = 1; i < 2 * (size - 1) - 1; i = i + 2) {
        edges[i] = k - 1;
        edges[i + 1] = k + 1;
        k++;
    }
    edges[2 * (size - 1) - 1] = size - 2;

    MPI_Graph_create(MPI_COMM_WORLD, size, index, edges, reorder, &graphcomm);

    return graphcomm;
}

int* send_recv(int* buf, int count, MPI_Datatype type, int dest, int tag, MPI_Comm comm, int source, int ProcRank) {
    int size;

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Status status;

    int* neighbors = new int[2];

    if ((ProcRank == 0) || (ProcRank == size - 1)) {
        MPI_Graph_neighbors(comm, ProcRank, 1, neighbors);
    } else {
        MPI_Graph_neighbors(comm, ProcRank, 2, neighbors);
    }

    int* local_buf = new int[count];

    if (ProcRank == source) {
        for (int i = 0; i < count; i++) {
            local_buf[i] = buf[i];
        }
    }

    if (source < dest) {
        for (int proc = source; proc < dest; proc++) {
            if (ProcRank == proc) {
                if (ProcRank == 0) {
                    MPI_Send(&local_buf[0], count, type, neighbors[0], 0, comm);
                } else {
                    MPI_Send(&local_buf[0], count, type, neighbors[1], 0, comm);
                }
            } else if (ProcRank == proc + 1) {
                if (ProcRank == size - 1) {
                    MPI_Recv(&local_buf[0], count, type, neighbors[0], 0, comm, &status);
                } else {
                    MPI_Recv(&local_buf[0], count, type, neighbors[0], 0, comm, &status);
                }
            }
            MPI_Barrier(comm);
        }
    }

    if (source > dest) {
        for (int proc = source; proc > dest; proc--) {
            if (ProcRank == proc) {
                if (ProcRank == size - 1) {
                    MPI_Send(&local_buf[0], count, type, neighbors[0], 0, comm);
                } else {
                    MPI_Send(&local_buf[0], count, type, neighbors[0], 0, comm);
                }
            } else if (ProcRank == proc - 1) {
                if (ProcRank == 0) {
                    MPI_Recv(&local_buf[0], count, type, neighbors[0], 0, comm, &status);
                } else {
                    MPI_Recv(&local_buf[0], count, type, neighbors[1], 0, comm, &status);
                }
            }
            MPI_Barrier(comm);
        }
    }
    return local_buf;
}
