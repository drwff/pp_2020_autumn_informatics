// Copyright 2020 Paranicheva Alyona
#include <mpi.h>
#include <ctime>
#include <cstring>
#include <algorithm>
#include <random>
#include <iostream>
#include <vector>
#include <string>
#include "../../../modules/task_2/paranicheva_a_gather/gather.h"

std::vector<int> getRandomArrInt(int number, int s) {
    std::mt19937 gen;
    gen.seed(static_cast<unsigned int>(time(0)));
    std::vector<int> array(number);
    for (int i = 0; i < number; i++)
        array[i] = (gen() % 100) * (s + 1);
    return array;
}

std::vector<double> getRandomArrDouble(int number, int s) {
    std::mt19937 gen;
    gen.seed(static_cast<unsigned int>(time(0)));
    std::vector<double> array(number);
    for (int i = 0; i < number; i++) {
        array[i] = ((gen() % 100) / 10.0) * (s + 0.1);
    }
    return array;
}

int Gather(void *sbuf, int scount, MPI_Datatype stype,
    void *rbuf, int rcount, MPI_Datatype rtype, int root, MPI_Comm comm) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (root < 0 || root >= size)
        return MPI_ERR_ROOT;
    int stype_size, rtype_size;
    MPI_Type_size(stype, &stype_size);
    if (stype_size == MPI_ERR_TYPE)
        return MPI_ERR_TYPE;
    MPI_Type_size(rtype, &rtype_size);
    if (rtype_size == MPI_ERR_TYPE)
        return MPI_ERR_TYPE;
    if (scount != rcount)
        return MPI_ERR_COUNT;
    if (scount <= 0)
        return MPI_ERR_COUNT;
    if (rcount <= 0)
        return MPI_ERR_COUNT;
    if (rank == root) {
        std::memcpy(reinterpret_cast<char*>(rbuf) + rcount * rtype_size * root,
            sbuf, scount* stype_size);
        for (int i = 0; i < size; i++) {
            if (i != root) {
                MPI_Status status;
                MPI_Recv(reinterpret_cast<char*>(rbuf) + rcount * rtype_size * i, rcount,
                    rtype, i, 0, comm, &status);
            }
        }
    } else {
        MPI_Send(sbuf, scount, stype, root, 0, comm);
    }
    return MPI_SUCCESS;
}
