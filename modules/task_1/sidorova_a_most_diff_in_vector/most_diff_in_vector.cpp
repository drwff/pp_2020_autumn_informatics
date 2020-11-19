// Copyright 2020 Sidorova Alexandra

#include <mpi.h>
#include <vector>
#include <random>
#include <ctime>
#include <cmath>
#include <exception>
#include "./most_diff_in_vector.h"

std::vector<int> getRandomVector(size_t size) {
    std::mt19937 gen;
    gen.seed(static_cast<unsigned int>(time(0)));
    std::vector<int> vec(size);
    for (size_t i = 0; i < size; i++)
        vec[i] = gen() % 100;
    return vec;
}

unsigned int getSequentialSearch(const std::vector<int>& vec) {
    const size_t size = vec.size();
    if (size < 2)
        throw "[Error] Small vector size. The size must be greater 1.";

    unsigned int maxDiff = 0;
    for (size_t i = 0; i < size - 1; i++) {
        if (static_cast<unsigned int>(abs(vec[i] - vec[i + 1])) > maxDiff)
            maxDiff = abs(vec[i] - vec[i + 1]);
    }

    return maxDiff;
}

unsigned int getParallelSearch(const std::vector<int>& vec) {
    const size_t vectorSize = vec.size();
    if (vectorSize < 2)
        throw "[Error] Small vector size. The size must be greater 1.";

    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    const int delta = static_cast<int>((vectorSize - 1) / size);
    const int remainder = static_cast<int>((vectorSize - 1) % size);
    std::vector<int> localVec;

    if (rank == 0) {
        if (delta != 0) {
            for (int proc = 1; proc < size; proc++)
                MPI_Send(&vec[0] + proc * delta + remainder, delta + 1, MPI_INT, proc, 0, MPI_COMM_WORLD);
        }
    }

    unsigned int maxDiff = 0, localMaxDiff = 0;

    if (rank == 0) {
        if (size != 1 || delta != 0) {
            localVec.resize(delta + remainder + 1);
            localVec.assign(vec.begin(), vec.begin() + delta + remainder + 1);
            localMaxDiff = getSequentialSearch(localVec);
        } else {
            localMaxDiff = getSequentialSearch(vec);
        }
    } else if (delta != 0) {
        localVec.resize(delta + 1);
        MPI_Status status;
        MPI_Recv(&localVec[0], delta + 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        localMaxDiff = getSequentialSearch(localVec);
    }

    MPI_Reduce(&localMaxDiff, &maxDiff, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);

    return maxDiff;
}
