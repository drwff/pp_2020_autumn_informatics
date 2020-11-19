// Copyright 2020 Solovev Alexandr
#include <mpi.h>
#include <time.h>
#include <random>
#include <algorithm>
#include <string>
#include "../../../modules/task_1/solovev_a_count_letters/count_letters.h"
constexpr char base[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890!,.&^$#@*()-_+=";


int calculateCountSequental(std::string result) {
    return  std::count_if(result.begin(), result.end(),
        [](char c) { return ((c >= 'A') && (c <= 'Z')) || ((c >= 'a') && (c <= 'z')); });
}

int calculateCountParallel(const std::string result, int elements_count) {
    int process_count, process_rank;
    MPI_Comm_size(MPI_COMM_WORLD, &process_count);
    MPI_Comm_rank(MPI_COMM_WORLD, &process_rank);
    int delta = elements_count / process_count;
    int remain = elements_count - delta * process_count;
    if (process_rank == 0) {
        for (int process_num = 1; process_num < process_count; process_num++)
            MPI_Send(&result[remain + process_num * delta], delta, MPI_CHAR,
                process_num, 0, MPI_COMM_WORLD);
    }
    std::string part_string;
    part_string.resize(process_rank == 0 ? delta + remain : delta);
    if (process_rank == 0) {
        part_string = { result.begin(), result.begin() + delta + remain };
    } else {
        MPI_Status status;
        MPI_Recv(&part_string[0], delta, MPI_CHAR, 0, 0, MPI_COMM_WORLD, &status);
    }
    int sum = 0;
    int part_sum = calculateCountSequental(part_string);
    MPI_Reduce(&part_sum, &sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    return sum;
}

std::string CreateRandomString(int elems_count) {
    std::string result;
    result.resize(elems_count);
    srand(static_cast<unsigned>(time(0)));
    for (int i = 0; i < elems_count; i++)
        result[i] = base[rand() % 77];
    return result;
}
