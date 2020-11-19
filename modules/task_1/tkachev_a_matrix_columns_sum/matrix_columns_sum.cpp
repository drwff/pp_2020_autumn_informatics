// Copyright 2020 Tkachev Alexey
#include <mpi.h>
#include <stdlib.h>
#include <vector>
#include <ctime>
#include <iostream>
#include <random>
#include <cassert>
#include <cmath>
#include "../../../../modules/task_1/tkachev_a_matrix_columns_sum/matrix_columns_sum.h"

std::vector<int> generateRandomMatrix(int count_rows, int count_columns) {
    assert(count_columns != 0);
    assert(count_rows != 0);

    std::vector<int> matrix(count_rows * count_columns);

    std::mt19937 generator(time(0));
    std::uniform_int_distribution<> int_distribution(-50, 50);

    for (uint64_t i = 0; i < matrix.size(); i++) {
        matrix[i] = int_distribution(generator);
    }

    return matrix;
}

std::vector<int> getSequentialColumnsSum(std::vector<int> vector, int count_columns, int count_rows,
                                        uint64_t count_processes, int process_id, int offset) {
    assert(count_processes != 0);
    assert(count_columns != 0);
    assert(count_rows != 0);
    assert(offset != 0);
    assert(vector.size() != 0);

    std::vector<int> sum_columns;

    int check_sum = 0;

    for (int item : vector) {
        check_sum += item;
    }

    if (check_sum == 0) {
        return std::vector<int>(count_columns, 0);
    }

    if (count_processes == 1) {
        for (int current_column = 0; current_column < count_columns; current_column++) {
            int local_column_sum = 0;

            for (int current_row = 0; current_row < count_rows; current_row++) {
                local_column_sum += vector[current_column + current_row * count_columns];
            }

            sum_columns.push_back(local_column_sum);
        }
    } else {
        int current_row = 0;

        if (process_id * offset < count_columns) {
            current_row = 1;
        } else {
            for (current_row = 2; current_row < count_rows; current_row++) {
                if (process_id * offset < current_row * count_columns) {
                    break;
                }
            }
        }

        if (process_id * offset + (offset-1) >= count_columns * current_row) {
            int head_size = count_columns * current_row - offset * process_id;
            std::vector<int> head;
            std::vector<int> tail(count_columns, 0);

            for (int i = 0; (count_columns * (current_row - 1) + i < process_id * offset) &&
                            (head_size < count_columns); i++) {
                head.push_back(0);
            }

            for (int i = 0; i < head_size; i++) {
                head.push_back(vector[i]);
            }

            for (int i = 0; i < count_columns; i++) {
                for (uint64_t j = 0; head_size + j * count_columns + i < vector.size(); j++) {
                    tail[i] += vector[head_size + j * count_columns + i];
                }
            }

            for (int i = 0; i < count_columns; i++) {
                sum_columns.push_back(head[i] + tail[i]);
            }
        } else {
            for (int i = 0; (count_columns * (current_row - 1) + i) < process_id * offset; i++) {
                sum_columns.push_back(0);
            }

            for (int item : vector) {
                sum_columns.push_back(item);
            }

            for (int i = 0; process_id * offset + (offset-1) + i < count_columns * current_row - 1; i++) {
                sum_columns.push_back(0);
            }
        }
    }

    return sum_columns;
}

std::vector<int> getParallelColumnsSum(std::vector<int> matrix, int count_columns, int count_rows) {
    assert(count_columns != 0);
    assert(count_rows != 0);
    assert(matrix.size() != 0);

    int processes_count;
    int process_rank, offset;
    uint64_t MAX_COUNT_PROCESS_TO_USE = 0;

    MPI_Comm_rank(MPI_COMM_WORLD, &process_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &processes_count);

    uint64_t uintprocesses_count = abs(processes_count);

    assert(uintprocesses_count != 0);

    if (uintprocesses_count >= matrix.size()) {
        MAX_COUNT_PROCESS_TO_USE = matrix.size();
        offset = 1;
    } else {
        if (matrix.size() % uintprocesses_count != 0) {
            offset = round(matrix.size() / uintprocesses_count + 0.5);
        } else {
            if (matrix.size() % uintprocesses_count == 0) {
                offset = matrix.size() / uintprocesses_count;
            }
        }

        if (matrix.size() % 2 != 0) {
            MAX_COUNT_PROCESS_TO_USE = matrix.size();
            offset = 1;
        } else {
            if (matrix.size() % 2 == 0) {
                MAX_COUNT_PROCESS_TO_USE = round(matrix.size() / offset + 0.5);
            }
        }
    }

    if (MAX_COUNT_PROCESS_TO_USE > uintprocesses_count) {
        offset = matrix.size();
        MAX_COUNT_PROCESS_TO_USE = 1;

        for (uint64_t i = 0; i < offset * uintprocesses_count; i++) {
            matrix.push_back(0);
        }
    }

    if (process_rank == 0) {
        if (MAX_COUNT_PROCESS_TO_USE <= uintprocesses_count || matrix.size() % uintprocesses_count != 0) {
            for (uint64_t i = 0; i < offset * (uintprocesses_count - MAX_COUNT_PROCESS_TO_USE) + offset; i++) {
                matrix.push_back(0);
            }
        }
    }

    if (process_rank == 0) {
        for (uint64_t process_id = 1; process_id < uintprocesses_count; process_id++) {
            MPI_Send(&matrix[0] + offset * process_id, offset, MPI_INT, process_id, 0, MPI_COMM_WORLD);
        }
    }

    std::vector<int> local_vec(offset);

    if (process_rank == 0) {
        local_vec = std::vector<int>(matrix.begin(), matrix.begin() + offset);
    } else {
        MPI_Status status;
        MPI_Recv(&local_vec[0], offset, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
    }

    std::vector<int> global_columns_sum(count_columns);
    std::vector<int> sequential_columns_sum = getSequentialColumnsSum(
        local_vec, count_columns, count_rows, uintprocesses_count, process_rank, offset);

    MPI_Reduce(
        sequential_columns_sum.data(), global_columns_sum.data(), count_columns, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    return global_columns_sum;
}
