// Copyright 2020 Novozhilova Ekaterina
#include <mpi.h>
#include <vector>
#include <random>
#include <ctime>
#include "../../../modules/task_1/novozhilova_e_vector_sum/vector_sum.h"


std::vector<int> GenVector(int size) {
    std::mt19937 gen;
    gen.seed(static_cast<unsigned int>(time(0)));
    std::vector<int> v(size);
    for (int i = 0; i < size ; i++) {
        v[i] = gen();
    }
    return v;
}

int GetSeqSum(std::vector<int> seq_v) {
    int seq_size = seq_v.size();
    int seq_sum = 0;
    for (int i = 0; i < seq_size; i++) {
        seq_sum+=seq_v[i];
    }
    return seq_sum;
}

int GetParSum(std::vector<int> v, int v_size) {
    int Comm_size, Comm_rank;

    MPI_Comm_size(MPI_COMM_WORLD, &Comm_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &Comm_rank);

    int check = v_size%Comm_size;
    while (check != 0) {
        v.push_back(0);
        v_size++;
        check = v_size%Comm_size;
    }

    int divide = v_size/Comm_size;
    std::vector<int> seq_v(divide);
    if (Comm_rank == 0) {
        for (int i = 1; i < Comm_size; i++) {
            MPI_Send(&v[0] + i * divide, divide, MPI_INT, i, 0, MPI_COMM_WORLD);
        }
        seq_v = std::vector<int>(v.begin(), v.begin() + divide);
    } else {
        MPI_Status status;
        MPI_Recv(&seq_v[0], divide, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
    }
    int sum = 0;
    int seq_sum = GetSeqSum(seq_v);
    MPI_Reduce(&seq_sum, &sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    return sum;
}
