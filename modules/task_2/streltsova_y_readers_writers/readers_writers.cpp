// Copyright 2020 Streltsova Yana
#include <mpi.h>
#include <random>
#include <ctime>
#include "../../../modules/task_2/streltsova_y_readers_writers/readers_writers.h"

void wait(double seconds) {
    double t1 = MPI_Wtime();
    while (MPI_Wtime() - t1 < seconds) {}
}

int accessData(int count) {
    int data = 0;  // Data area
    int request, response;
    int accessWrite = 0, accessRead = 0;  // Access for writers and readers
    int activeReaders = 0;  // Number of active readers
    MPI_Status status;
    while (count != 0) {
        MPI_Recv(&request, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        if (status.MPI_TAG == 1) {  // Writer request
            response = 0;
            if (accessWrite == 0) {
                accessWrite = 1;
                accessRead = 1;  // Closing access to new readers
                response = 1;
                MPI_Send(&response, 1, MPI_INT, status.MPI_SOURCE, 0, MPI_COMM_WORLD);
            } else {
                MPI_Send(&response, 1, MPI_INT, status.MPI_SOURCE, 0, MPI_COMM_WORLD);
            }
        } else if (status.MPI_TAG == 2) {  // Checking for active readers
            response = 0;
            if (activeReaders == 0) {
                response = 1;
                MPI_Send(&response, 1, MPI_INT, status.MPI_SOURCE, 0, MPI_COMM_WORLD);
                data = request;
                accessRead = 0;  // Opening access to readers
                accessWrite = 0;
                count--;
            } else {
                MPI_Send(&response, 1, MPI_INT, status.MPI_SOURCE, 0, MPI_COMM_WORLD);
            }
        } else if (status.MPI_TAG == 3) {  // Reader request
            response = 0;
            if (accessRead == 0) {
                activeReaders++;
                response = 1;
                MPI_Send(&response, 1, MPI_INT, status.MPI_SOURCE, 0, MPI_COMM_WORLD);
                MPI_Send(&data, 1, MPI_INT, status.MPI_SOURCE, 4, MPI_COMM_WORLD);
            } else {
                MPI_Send(&response, 1, MPI_INT, status.MPI_SOURCE, 0, MPI_COMM_WORLD);
            }
        } else if (status.MPI_TAG == 5) {  // The reader has finished reading
            activeReaders--;
            count--;
        }
    }
    return count;
}

void writer() {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    wait(0.01 * rank);
    int data = rank;
    int request = 0;
    MPI_Status status;
    while (!request) {  // We will send a request until access is free for the writer
        MPI_Send(&request, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
        MPI_Recv(&request, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
    }
    request = 0;
    while (!request) {  // We are waiting for active readers to finish reading and writing data
        MPI_Send(&data, 1, MPI_INT, 0, 2, MPI_COMM_WORLD);
        MPI_Recv(&request, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
    }
}

void reader() {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    wait(0.01 * rank);
    int data, request = 0;
    MPI_Status status;
    while (!request) {  // We will send a request until access is free for the reader
        MPI_Send(&request, 1, MPI_INT, 0, 3, MPI_COMM_WORLD);
        MPI_Recv(&request, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
    }
    MPI_Recv(&data, 1, MPI_INT, 0, 4, MPI_COMM_WORLD, &status);  // Reading data
    MPI_Send(&rank, 1, MPI_INT, 0, 5, MPI_COMM_WORLD);  // Finishing reading
}
