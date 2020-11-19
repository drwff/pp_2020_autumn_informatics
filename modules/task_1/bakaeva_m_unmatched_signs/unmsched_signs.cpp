// Copyright 2020 Bakaeva Maria
#include <mpi.h>
#include <ctime>
#include <random>
#include <stdexcept>
#include <cstring>
#include "./unmached_signs.h"

static int offset = 0;

char* generateString(int length) {
    std::mt19937 gen;
    gen.seed(static_cast<unsigned int>(time(0)) + (offset += 10));
    char* str = new char[length + 1];
    const char alpNum[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";

    for (int i = 0; i < length; i++) {
        str[i] = alpNum[gen() % (sizeof(alpNum) - 1)];
    }

    str[length] = '\0';
    return str;
}

int getSequentialUnmachedSignsCount(const char* str1, const char* str2) {
    if (strlen(str1) != strlen(str2)) {
        throw "Error: Vectors have different length";
    }
    int diff = 0;

    for (size_t i = 0; i < strlen(str1); i++) {
        if (str1[i] != str2[i]) {
            diff++;
        }
    }

    return diff;
}

int getParallelUnmachedSignsCount(const char* str1_global, const char* str2_global) {
    if (strlen(str1_global) != strlen(str2_global)) {
        throw "Error: Vectors have different length";
    }

    int size, rank;

    //Число задействованных процессов
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    //Получение номера текущего процесса в рамках коммуникатора
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int local_diff = 0, global_diff = 0;
    int length = static_cast<int>(strlen(str1_global));

    //Длина промежутка для каждого процесса
    const int delta = static_cast<int>(length / size);
    //Длина остатка от всей строки
    const int remainder = static_cast<int>(length % size);

    if (rank == 0) {
        for (int process = 1; process < size; process++) {
            MPI_Send(&str1_global[0] + process * delta + remainder, delta,
                MPI_CHAR, process, 0, MPI_COMM_WORLD);
            MPI_Send(&str2_global[0] + process * delta + remainder, delta,
                MPI_CHAR, process, 1, MPI_COMM_WORLD);
        }
    }

    char* str1_local;
    char* str2_local;

    if (rank == 0) {
        str1_local = new char[delta + remainder + 1];
        str2_local = new char[delta + remainder + 1];
        for (int i = 0; i < (delta + remainder); i++) {
            str1_local[i] = str1_global[i];
            str2_local[i] = str2_global[i];
        }

        str1_local[delta + remainder] = '\0';
        str2_local[delta + remainder] = '\0';

        local_diff = getSequentialUnmachedSignsCount(str1_local, str2_local);
    } else {
        str1_local = new char[delta + 1];
        str2_local = new char[delta + 1];
        MPI_Status status;
        MPI_Recv(&str1_local[0], delta, MPI_CHAR, 0, 0, MPI_COMM_WORLD, &status);
        MPI_Recv(&str2_local[0], delta, MPI_CHAR, 0, 1, MPI_COMM_WORLD, &status);

        str1_local[delta] = '\0';
        str2_local[delta] = '\0';

        local_diff = getSequentialUnmachedSignsCount(str1_local, str2_local);
    }

    delete[] str1_local;
    delete[] str2_local;

    MPI_Reduce(&local_diff, &global_diff, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    return global_diff;
}
