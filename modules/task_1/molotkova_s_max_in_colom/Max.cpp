// Copyright 2020 Molotkova Svetlana
#include <stdlib.h>
#include <math.h>
#include <mpi.h>
#include <cmath>
#include <ctime>
#include <vector>
#include <random>
#include <iomanip>
#include <map>
#include <algorithm>
#include "../../../modules/task_1/molotkova_s_max_in_colom/Max.h"

std::vector<double> Get_max_parallel(std::vector<double> matrix, int stolb, int stroka) {
    int ProcNum, ProcRank;
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    MPI_Status Status;
    int offset = stolb / ProcNum;
    int rem = stolb % ProcNum;
    std::vector<double> part_m(stroka * offset);
    if (ProcRank == 0) {
        for (int i = 1; i < ProcNum; i++) {
            for (int j = 0; j < offset; j++) {
                for (int k = 0; k < stroka; k++) {
                    part_m[k * offset + j] = matrix[k * stolb + i * offset + j];
                }
            }
            MPI_Send(part_m.data(), offset * stroka, MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
        }
        for (int j = 0; j < offset; j++) {
            for (int k = 0; k < stroka; k++) {
                part_m[k * offset + j] = matrix[k * stolb + j];
            }
        }
    } else {
        MPI_Recv(part_m.data(), offset * stroka, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &Status);
    }
    std::vector<double> max(ProcRank == 0 ? stolb : offset);
    for (int i = 0; i < offset; i++) {
        max[i] = findmax(i, part_m, stroka, offset);
    }
    if (ProcRank == 0) {
        for (int i = 1; i < ProcNum; i++) {
            MPI_Recv(max.data() + i * offset, offset, MPI_DOUBLE, i, 0, MPI_COMM_WORLD, &Status);
        }
    } else {
        MPI_Send(max.data(), offset, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
    }

    if (ProcRank == 0) {
        if (rem != 0) {
            for (int j = stolb - rem; j < stolb; j++)
                max[j] = findmax(j, matrix, stroka, stolb);
        }
    }
    return max;
}
std::vector<double> Get_max_sequestional(std::vector<double> matrix, int stolb, int stroka) {
    std::vector<double> max(stolb);
    for (int i = 0; i < stolb; i++)
        max[i] = findmax(i, matrix, stroka, stolb);
    return max;
}
std::vector<double> Matrix(int stolb, int stroka) {
    std::mt19937 gen;
    gen.seed(static_cast<unsigned int>(time(0)));
    std::vector<double> ptrarray(stroka * stolb);
    for (int count_row = 0; count_row < stroka; count_row++)
        for (int count_column = 0; count_column < stolb; count_column++)
            ptrarray[count_row * stolb + count_column] = gen() % 10 + 1.0 / static_cast<double>((gen() % 10 + 1.0));
    return ptrarray;
}
double findmax(int stolbnum, std::vector<double> arr, int stroka, int stolb) {
    double max = 0;
    max = arr[stolbnum];
    for (int i = 0; i < stroka; i++)
        if (arr[i * stolb + stolbnum] > max)
            max = arr[i * stolb + stolbnum];
    return max;
}
