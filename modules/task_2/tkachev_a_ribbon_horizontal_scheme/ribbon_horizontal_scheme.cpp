// Copyright 2020 Tkachev Alexey
#include <mpi.h>
#include <stdlib.h>
#include <vector>
#include <ctime>
#include <iostream>
#include <random>
#include <cassert>
#include <cmath>
#include "../../../../modules/task_2/tkachev_a_ribbon_horizontal_scheme/ribbon_horizontal_scheme.h"
using std::vector;

std::vector<int> getRandomVector(int count_rows, int count_columns) {
    assert(count_columns != 0 && count_rows != 0);
    std::vector<int> matrix(count_rows * count_columns);

    std::mt19937 generator(time(0));
    std::uniform_int_distribution<> int_distribution(-50, 50);

    for (uint64_t i = 0; i < matrix.size(); i++) {
        matrix[i] = int_distribution(generator);
    }

    return matrix;
}

std::vector<int> multiplyOnVector(std::vector<int> matrix, std::vector<int> the_vector,
                                  int matrix_count_rows, int matrix_count_columns) {
    assert(uint64_t(matrix_count_columns) == the_vector.size());

    std::vector<int> result(matrix_count_rows, 0);

    for (int i = 0; i < matrix_count_rows; i++) {
        for (int j = 0; j < matrix_count_columns; j++) {
            result[i] += matrix[matrix_count_columns * i + j] * the_vector[j];
        }
    }

    return result;
}

std::vector<int> parallelMultiplyOnVector(std::vector<int> matrix, std::vector<int> the_vector,
                                          int matrix_count_rows, int matrix_count_columns) {
    assert(matrix.size() > 0 && matrix_count_rows > 0 && matrix_count_columns > 0);
    assert(uint64_t(matrix_count_columns) == the_vector.size());

    int count_useful_processes = 0;
    int world_process_rank, count_processes;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_process_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &count_processes);
    assert(count_processes > 0);

    if (matrix_count_rows <= count_processes) {
        count_useful_processes = matrix_count_rows;
    } else {
        if (matrix_count_rows > count_processes) {
            count_useful_processes = count_processes;
        }
    }

    MPI_Group world_group;
    MPI_Comm_group(MPI_COMM_WORLD, &world_group);

    int *ranks = new int[count_useful_processes];

    for (int i = 0; i < count_useful_processes; i++) {
        ranks[i] = i;
    }

    MPI_Group useful_processes_group;
    MPI_Group_incl(world_group, count_useful_processes, ranks, &useful_processes_group);
    delete[] ranks;

    MPI_Comm COMM_USEFUL_PROCESSES;
    MPI_Comm_create(MPI_COMM_WORLD, useful_processes_group, &COMM_USEFUL_PROCESSES);

    std::vector<int> return_result(matrix_count_rows);

    if (world_process_rank < count_useful_processes) {
        std::vector<int> data_size;
        std::vector<int> offsets_from_beginning;
        int process_rank;
        MPI_Comm_rank(COMM_USEFUL_PROCESSES, &process_rank);
        for (int i = 0; i < count_useful_processes; i++) {
            if (count_useful_processes == matrix_count_rows) {
                data_size.push_back(matrix_count_columns);
                offsets_from_beginning.push_back(matrix_count_columns * i);
            } else {
                if (matrix_count_rows % count_useful_processes == 0) {
                    data_size.push_back(
                        matrix_count_rows / count_useful_processes * matrix_count_columns);
                    offsets_from_beginning.push_back(
                        matrix_count_rows / count_useful_processes * matrix_count_columns * i);
                } else {
                    if (i == count_useful_processes - 1) {
                        if (matrix_count_rows % (count_useful_processes - 1) != 0) {
                            data_size.push_back(matrix.size() - round(
                                matrix_count_rows / (count_useful_processes - 1)) *
                                matrix_count_columns * (count_useful_processes - 1));
                        } else {
                            data_size.push_back(matrix.size() - round(
                                matrix_count_rows / count_useful_processes) *
                                matrix_count_columns * (count_useful_processes - 1));
                        }
                        offsets_from_beginning.push_back(matrix.size() - data_size[i]);
                        break;
                    } else {
                        if (count_useful_processes > 1) {
                            if (matrix_count_rows % (count_useful_processes - 1) != 0) {
                                data_size.push_back(
                                        round(matrix_count_rows / (count_useful_processes - 1)) *
                                        matrix_count_columns);
                            } else {
                                data_size.push_back(
                                        round(matrix_count_rows / count_useful_processes) *
                                        matrix_count_columns);
                            }
                            offsets_from_beginning.push_back(data_size[i] * i);
                        } else {
                            data_size.push_back(matrix.size());
                            offsets_from_beginning.push_back(0);
                        }
                    }
                }
            }
        }
        std::vector<int> recv_ribbon(data_size[process_rank]);

        if (process_rank == 0) {
            for (int pid = 1; pid < count_useful_processes; pid++) {
                MPI_Send(matrix.data() + offsets_from_beginning[pid],
                        data_size[pid], MPI_INT, pid,
                        0, COMM_USEFUL_PROCESSES);
            }
        }

        if (process_rank == 0) {
            recv_ribbon = std::vector<int>(
                matrix.begin(), matrix.begin() + data_size[0]);
        } else {
            MPI_Status recv_status;
            MPI_Recv(recv_ribbon.data(), data_size[process_rank],
                MPI_INT, 0, 0, COMM_USEFUL_PROCESSES, &recv_status);
        }

        if (process_rank == 0) {
            for (int pid = 1; pid < count_useful_processes; pid++) {
                MPI_Send(the_vector.data(), the_vector.size(),
                MPI_INT, pid, 0, COMM_USEFUL_PROCESSES);
            }
        }

        std::vector<int> inner_the_vector(matrix_count_columns);

        if (process_rank == 0) {
            inner_the_vector = std::vector<int>(the_vector.begin(), the_vector.end());
        } else {
            MPI_Status recv_status_vector;
            MPI_Recv(inner_the_vector.data(), matrix_count_columns, MPI_INT,
            0, 0, COMM_USEFUL_PROCESSES, &recv_status_vector);
        }

        std::vector<int> result_vector(data_size[process_rank] / matrix_count_columns, 0);

        if (recv_ribbon.size() > uint64_t(matrix_count_columns)) {
            for (uint64_t j = 0; j < result_vector.size(); j++) {
                for (int i = 0; i < matrix_count_columns; i++) {
                    result_vector[j] += recv_ribbon[i + j * matrix_count_columns] * inner_the_vector[i];
                }
            }
        } else {
            for (int i = 0; i < matrix_count_columns; i++) {
                result_vector[0] += recv_ribbon[i] * inner_the_vector[i];
            }
        }

        std::vector<int> offsets_in_return_result(count_useful_processes);

        offsets_in_return_result[0] = 0;
        for (int i = 1; i < count_useful_processes; i++) {
            offsets_in_return_result[i] = offsets_in_return_result[i-1] + data_size[i-1] / matrix_count_columns;
        }

        MPI_Gatherv(result_vector.data(), result_vector.size(), MPI_INT,
                    return_result.data(), data_size.data(),
                    offsets_in_return_result.data(), MPI_INT,
                    0, COMM_USEFUL_PROCESSES);

        MPI_Group_free(&useful_processes_group);
        MPI_Comm_free(&COMM_USEFUL_PROCESSES);
    }

    return return_result;
}
