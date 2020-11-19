// Copyright 2020 Tashirev Ivan
#include <mpi.h>
#include<vector>
#include <ctime>
#include<random>
#include<algorithm>

std::vector<int> getRandVector(int size) {
    std::mt19937 engine;
    engine.seed(std::time(nullptr));
    std::vector<int> vec(size);
    for (int i = 0; i < size; i++) {
        vec[i] = engine() % 100;
    }
    return vec;
}
int getVectorMinValue(std::vector<int>vec) {
    return *std::min_element(vec.begin(), vec.end());
}
int getVectorMinValueParallel(std::vector<int>vec, int sz) {
    int ProcNum, ProcRank;
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    int delta = sz / ProcNum;
    int temp = sz - delta * ProcNum;
    if (ProcRank == 0) {
        for (int proc = 1; proc < ProcNum; proc++) {
            MPI_Send(&vec[0] + temp + proc * delta, delta,
                MPI_INT, proc, 0, MPI_COMM_WORLD);
        }
    }
    int size;
    if (ProcRank == 0) size = temp + delta;
    else
        size = delta;
    std::vector<int> loc_vec(size);
    if (ProcRank == 0) {
        loc_vec = std::vector<int>(vec.begin(),
            vec.begin() + delta + temp);
    } else {
        MPI_Status status;
        MPI_Recv(&loc_vec[0], delta, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
    }
    int loc_min = getVectorMinValue(loc_vec);
    int glob_min;
    MPI_Reduce(&loc_min, &glob_min, 1, MPI_INT, MPI_MIN, 0, MPI_COMM_WORLD);
    return glob_min;
}


