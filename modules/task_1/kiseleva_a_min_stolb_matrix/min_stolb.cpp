// Copyright 2020 Kiseleva Anastasia
#include "../../../modules/task_1/kiseleva_a_min_stolb_matrix/min_stolb.h"
#include <mpi.h>
#include <vector>

std::vector<std::vector<int>> RandomMatrix(int str, int stlb) {
    std::mt19937 gen;
    gen.seed(static_cast<unsigned int>(time(0)));
    std::vector<std::vector<int>> matrix(str*stlb);
    std::vector<int> stolb(stlb);
    for (int i = 0; i < str; i++) {
        stolb.clear();
        for (int j = 0; j < stlb; j++) {
            stolb.push_back(gen() % 100);
        }
        matrix[i] = stolb;
    }
    return matrix;
}

std::vector<int> VVconvertV(std::vector<std::vector<int>> matrix, int str, int stlb) {
    std::vector<int> linm(str * stlb);
    for (int i = 0; i < str; i++) {
        for (int j = 0; j < stlb; j++) {
            linm[i * stlb + j] = matrix[i][j];
        }
    }
    return linm;
}

std::vector<int> Min(std::vector<int> linm, int str, int stlb) {
    int size, rank;
    std::vector<int> res(stlb);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    if (size > stlb) {
        MPI_Comm comm_world;
        MPI_Group group_world;
        comm_world = MPI_COMM_WORLD;
        MPI_Comm_group(comm_world, &group_world);
        int *members = new int[stlb];
        for (int i = 0; i < stlb; i++) {
            members[i] = i;
        }
        MPI_Group newworld;
        MPI_Group_incl(group_world, stlb, members, &newworld);
        MPI_Comm comm_newworld;
        MPI_Comm_create(comm_world, newworld, &comm_newworld);
        MPI_Group_rank(newworld, &rank);
        int size_;
        MPI_Group_size(newworld, &size_);
        int min = INT_MAX;
        int delta = 1;
        if (comm_newworld != MPI_COMM_NULL) {
            MPI_Bcast(linm.data(), str * stlb, MPI_INT, 0, comm_newworld);
            for (int i = delta * rank; i < delta * (rank + 1); i++) {
                min = linm[i];
                for (int j = 0; j < str; j++) {
                    if (linm[i + j * stlb] <= min) {
                        min = linm[i + j * stlb];
                    }
                }
                if (rank == 0) {
                    res[i] = min;
                } else {
                    MPI_Send(&min, 1, MPI_INT, 0, 15, comm_newworld);
                }
            }
            if (rank == 0) {
                for (int i = 1; i < size_; i++) {
                    for (int j = 0; j < delta; j++) {
                        MPI_Status status;
                        MPI_Recv(&min, 1, MPI_INT, i, 15, comm_newworld, &status);
                        res[i * delta + j] = min;
                    }
                }
            }
        }
    } else {
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        int min = INT_MAX;
        int delta = stlb / size;
        if (stlb % size != 0) {
            delta++;
        }
        MPI_Bcast(linm.data(), str * stlb, MPI_INT, 0, MPI_COMM_WORLD);
        for (int i = delta * rank; i < delta * (rank + 1); i++) {
            min = linm[i];
            for (int j = 0; j < str; j++) {
                if (linm[i + j * stlb] <= min) {
                    min = linm[i + j * stlb];
                }
            }
            if (rank == 0) {
                res[i] = min;
            } else {
                MPI_Send(&min, 1, MPI_INT, 0, 15, MPI_COMM_WORLD);
            }
        }
        if (rank == 0) {
            for (int i = 1; i < size; i++) {
                for (int j = 0; j < delta; j++) {
                    MPI_Status status;
                    MPI_Recv(&min, 1, MPI_INT, i, 15, MPI_COMM_WORLD, &status);
                    res[i * delta + j] = min;
                }
            }
        }
    }
    return res;
}
