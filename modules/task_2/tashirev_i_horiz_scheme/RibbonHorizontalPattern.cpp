// Copyright 2020 Tashirev Ivan
#include <mpi.h>
#include<vector>
#include <ctime>
#include<random>
#include<algorithm>
#include "../../../modules/task_2/tashirev_i_horiz_scheme/RibbonHorizontalPattern.h"

std::vector<int> getVector(int size) {
    std::mt19937 engine;
    engine.seed(std::time(nullptr));
    std::vector<int> vec(size);
    for (int i = 0; i < size; i++) {
        vec[i] = engine() % 100;
    }
    return vec;
}
std::vector<int> getMatrix(int line, int column) {
    std::mt19937 engine;
    engine.seed(std::time(nullptr));
    std::vector<int> vec(line * column);
    for (int i = 0; i < line * column; i++) {
        vec[i] = engine() % 100;
    }
    return vec;
}
std::vector<int> multMatrixSeq(std::vector<int> matr, int lines, int columns, std::vector<int> vec, int vecSize) {
    std::vector<int> result(lines);
    int sum = 0;
    for (int line = 0; line < lines; line++) {
        sum = 0;
        for (int column = 0; column < columns; column++) {
            sum += matr[line * columns + column] * vec[column];
        }
        result[line] = sum;
    }
    return result;
}

std::vector<int> multMatrixParallel(std::vector<int> matr, int lines, int columns, std::vector<int> vec, int vecSize) {
    int procNum, procRank, sum, rowsPart, size;
    MPI_Status status;

    MPI_Comm_size(MPI_COMM_WORLD, &procNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);

    rowsPart = lines / procNum;
    size = rowsPart * columns;

    if (procRank != 0) {
        vec = std::vector<int>(columns);
        matr = std::vector<int>(size);
    }

    MPI_Bcast(vec.data(), columns, MPI_INT, 0, MPI_COMM_WORLD);

    if (procRank == 0) {
        for (int currProc = 1; currProc < procNum; currProc++) {
            MPI_Send(matr.data() + currProc * size, size, MPI_INT, currProc, 0, MPI_COMM_WORLD);
        }
    }

    if (procRank != 0) {
        MPI_Recv(matr.data(), size, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
    }

    std::vector<int> returnVec(lines);
    std::vector<int> resultVec(rowsPart);
    if (procRank == 0) {
        for (int row = 0; row < rowsPart; row++) {
            sum = 0;
            for (int column = 0; column < columns; column++) {
                sum += matr[row * columns + column] * vec[column];
            }
            returnVec[row] = sum;
        }
    } else {
        for (int row = 0; row < rowsPart; row++) {
            sum = 0;
            for (int column = 0; column < columns; column++) {
                sum += matr[row * columns + column] * vec[column];
            }
            resultVec[row] = sum;
        }
    }

    if (procRank != 0) {
        MPI_Send(resultVec.data(), rowsPart, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }

    if (procRank == 0) {
        for (int currProc = 1; currProc < procNum; currProc++) {
            MPI_Recv(returnVec.data() + currProc * rowsPart, rowsPart, MPI_INT, currProc,
                0, MPI_COMM_WORLD, &status);
        }

        for (int i = 0; i < lines % procNum; i++) {
            sum = 0;
            for (int j = 0; j < columns; j++) {
                sum += matr[(rowsPart * procNum + i) * columns + j] * vec[j];
            }
            returnVec[rowsPart * procNum + i] = sum;
        }
    }

    return returnVec;
}
