// Copyright 2020 Ekaterina Emelkhovsky
#include <mpi.h>
#include <vector>
#include <string>
#include <random>
#include <ctime>
#include <algorithm>
#include <iostream>
#include "../../../modules/task_2/emelkhovsky_e_lent_horizontal_only_A/lent_horizontal_only_A.h"



std::vector<int> randMatrix(int r, int c) {
    std::mt19937 gen;
    gen.seed(static_cast<unsigned int>(time(0)));
    std::vector<int> vector_value(c * r);
    for (int i = 0; i < c; i++) {
        for (int j = 0; j < r; j++) {
            vector_value[i * r + j] = gen() % 10;
        }
    }
    return vector_value;
}


std::vector<int> getSequentialOperations(std::vector<int> vector_value, std::vector<int> vector_value_2, int r_lens) {
    std::vector<int> result(vector_value.size());
    int sum = 0;
    for (unsigned int i = 0; i < vector_value.size() / r_lens; i++) {
        for (int j = 0; j < r_lens; j++) {
            sum = 0;
            for (int k = 0; k < r_lens; k++) {
                sum += vector_value[i * r_lens + k] * vector_value_2[j + k * r_lens];
            }
            result[i * r_lens + j] = sum;
        }
    }
    return result;
}

std::vector<int> getParallelOperations(std::vector<int> main_vector_value,
std::vector<int> main_vector_value_2, int r) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int delta = r / size;
    std::vector<int> local_vector_value(delta * r);
    std::vector<int> local_vector_value_2(r * r);
    std::vector<int> matrix_res(r * r);
    MPI_Status status;

    if (rank == 0) {
        for (int process = 1; process < size; process++) {
            MPI_Send(&main_vector_value[0] + process * delta * r, delta * r, MPI_INT, process, 0, MPI_COMM_WORLD);
            MPI_Send(&main_vector_value_2[0], r * r, MPI_INT, process, 1, MPI_COMM_WORLD);
        }
    }

    if (rank == 0) {
        local_vector_value = std::vector<int>(main_vector_value.begin(), main_vector_value.begin() + delta * r);
        local_vector_value_2 = std::vector<int>(main_vector_value_2.begin(), main_vector_value_2.begin() + r * r);
    } else {
        MPI_Recv(&local_vector_value[0], delta * r, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        MPI_Recv(&local_vector_value_2[0], delta * delta * r  * r, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
    }
    std::vector<int> res_vector = getSequentialOperations(local_vector_value, local_vector_value_2, r);
    MPI_Allgather(res_vector.data(), delta * r, MPI_INT, matrix_res.data(), delta * r, MPI_INT, MPI_COMM_WORLD);
    if (rank == 0) {
        local_vector_value = std::vector<int>(main_vector_value.begin() + size * delta * r, main_vector_value.end());
        std::vector<int> ostatok = getSequentialOperations(local_vector_value, local_vector_value_2, r);
        for (unsigned int i = 0; i < ostatok.size(); i++) {
            matrix_res[size * delta * r + i] = ostatok[i];
        }
    }

    return matrix_res;
}
