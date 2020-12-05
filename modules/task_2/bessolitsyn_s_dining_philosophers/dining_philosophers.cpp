// Copyright 2020 Bessolitsyn Sergay
#include <mpi.h>
#include <vector>
#include <string>
#include <random>
#include <ctime>
#include <algorithm>
#include <iostream>
#include "../../../modules/task_2/bessolitsyn_s_dining_philosophers/dining_philosophers.h"

#define GET_FORKS 3
#define RETURN_FORKS 4
#define WAIT_FORKS 5

void wait() {
    std::mt19937 gen;
    gen.seed(static_cast<unsigned int>(time(0)));
    double t = MPI_Wtime();
    while (MPI_Wtime() - t < 0.03 +  static_cast<float>(gen() % 1000) / 10000) {}
}

int waiter(int iterations) {
    int size;
    int rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<bool> forks(size-1);
    for (size_t i = 0; i < forks.size(); i++) {
        forks[i] = true;
    }
    MPI_Status status;
    int info;
    while (iterations != 0) {
        MPI_Recv(&info, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        if (status.MPI_TAG == RETURN_FORKS) {
            --iterations;
            std::cout << "RETURN_FORKS from :" << status.MPI_SOURCE << std::endl;
            forks[status.MPI_SOURCE - 1] = true;
            forks[status.MPI_SOURCE % (size - 1)] = true;
            MPI_Send(&info, 1, MPI_INT, status.MPI_SOURCE, RETURN_FORKS, MPI_COMM_WORLD);
        } else {
            if (forks[status.MPI_SOURCE - 1] && forks[status.MPI_SOURCE % (size - 1)]) {
                std::cout << "GET_FORKS to: " << status.MPI_SOURCE << std::endl;
                forks[status.MPI_SOURCE - 1] = false;
                forks[status.MPI_SOURCE % (size - 1)] = false;
                MPI_Send(&info, 1, MPI_INT, status.MPI_SOURCE, GET_FORKS, MPI_COMM_WORLD);
            } else {
                std::cout << "WAIT_FORKS for: " << status.MPI_SOURCE << std::endl;
                MPI_Send(&info, 1, MPI_INT, status.MPI_SOURCE, WAIT_FORKS, MPI_COMM_WORLD);
            }
        }
    }
    return iterations;
}

void philosopher(int plates_for_eating) {
    int rank, info;
    MPI_Status status;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    while (plates_for_eating != 0) {
        wait();
        MPI_Send(&rank, 1, MPI_INT, 0, GET_FORKS, MPI_COMM_WORLD);
        MPI_Recv(&info, 1, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

        if (status.MPI_TAG == GET_FORKS) {
            std::cout << "mmm... delicious: " << rank << std::endl;
            --plates_for_eating;
            wait();
            MPI_Send(&rank, 1, MPI_INT, 0, RETURN_FORKS, MPI_COMM_WORLD);
            MPI_Recv(&info, 1, MPI_INT, 0, RETURN_FORKS, MPI_COMM_WORLD, &status);
        } else {
            wait();
        }
    }
}
