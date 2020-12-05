// Copyright 2020 Gogov Vladislav
#include <mpi.h>
#include <iostream>
#include <vector>
#include <random>
#include <cmath>
#include <limits>
#include "../../../modules/task_3/gogov_v_algorithm_fox/algorithm_fox.h"

bool assertMatrix(const Matrix& matrixA, const Matrix& matrixB) {
    if (matrixA.size() != matrixB.size())
        throw "Different size";
    for (size_t i = 0; i < matrixA.size(); i++) {
        if ((std::fabs(matrixA[i] - matrixB[i]) >= std::numeric_limits<double>::epsilon() * 1000000000.0))
            return false;
    }
    return true;
}

bool checkSize(int* size) {
    int procNum, sqrtProcNum, procRank;
    MPI_Comm_size(MPI_COMM_WORLD, &procNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
    sqrtProcNum = static_cast<int>(sqrt(procNum));
    if (procNum != sqrtProcNum * sqrtProcNum) {
        return false;
    }
    if (*size % sqrtProcNum != 0) {
        *size += sqrtProcNum - (*size % sqrtProcNum);
        if (procRank == 0) {
            std::cout << "Resizing the matrix: " << *size << std::endl;
        }
    }
    return true;
}

void printMatrix(const Matrix& matrix, int size) {
    std::cout << "Matrix print:" << std::endl;
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            std::cout << matrix[i * size + j] << "  ";
        }
        std::cout << std::endl;
    }
}

Matrix createRandomMatrix(int size) {
    std::random_device rd;
    std::mt19937 mersenne(rd());
    std::uniform_real_distribution<> urd(-50.0 , 50.0);
    Matrix matrix(size * size);
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            matrix[i * size + j] = static_cast<double>(urd(mersenne) / 1e10 * 1e10);
        }
    }
    return matrix;
}

void createGrid(Grid* grid, const int& procRank) {
    std::vector<int> dimensions(2, 0);
    std::vector<int> periods(2, 0);
    std::vector<int> coordinates(2, 0);
    std::vector<int> remainDims(2, 0);

    dimensions[0] = dimensions[1] = grid->dimGrid;
    periods[0] = periods[1] = 0;

    // Создание решетки
    MPI_Cart_create(MPI_COMM_WORLD, 2, dimensions.data(), periods.data(), 0, &grid->commGrid);

    // Определение координат процесса в решетке
    MPI_Cart_coords(grid->commGrid, procRank, 2, coordinates.data());
    grid->numRow = coordinates[0];
    grid->numCol = coordinates[1];

    // Коммуникатор для строк
    remainDims[0] = 0;
    remainDims[1] = 1;
    MPI_Cart_sub(grid->commGrid, remainDims.data(), &grid->commRow);

    // Коммуникатор для столбцов
    remainDims[0] = 1;
    remainDims[1] = 0;
    MPI_Cart_sub(grid->commGrid, remainDims.data(), &grid->commCols);
}

Matrix MultiplyMatrixSequential(const Matrix& matrixA, const Matrix& matrixB, int size) {
    std::vector<double> result(size * size, 0);
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            for (int k = 0; k < size; k++) {
                result[i * size + j] += matrixA[i * size + k] * matrixB[k * size + j];
            }
        }
    }
    return result;
}

void MultiplyMatrixAndSum(const Matrix& matrixA, const Matrix& matrixB, double* matrixC, int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            for (int k = 0; k < size; k++) {
                matrixC[i * size + j] += matrixA[i * size + k] * matrixB[k * size + j];
            }
        }
    }
}

Matrix MultiplyMatrixParallel(const Matrix& matrixA, const Matrix& matrixB, int size) {
    int procNum, procRank;
    MPI_Comm_size(MPI_COMM_WORLD, &procNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
    MPI_Status status;

    // Создание топологии
    Grid grid;
    grid.dimGrid = static_cast<int>(sqrt(procNum));
    createGrid(&grid, procRank);

    // Рассылка размера блока
    int blockSize;
    if (procRank == 0) {
        blockSize = size / grid.dimGrid;
    }
    MPI_Bcast(&blockSize, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // Определние типа данных - блок матрицы
    MPI_Datatype matrixBlock;
    MPI_Type_vector(blockSize, blockSize, grid.dimGrid * blockSize, MPI_DOUBLE, &matrixBlock);
    MPI_Type_commit(&matrixBlock);

    Matrix blockMatrixA(blockSize * blockSize, 0), blockMatrixB(blockSize * blockSize, 0);

    // Рассылка подматриц
    if (procRank == 0) {
        for (int i = 0; i < blockSize; i++) {
            for (int j = 0; j < blockSize; j++) {
                blockMatrixA[i * blockSize + j ] = matrixA[i * size + j];
                blockMatrixB[i * blockSize + j ] = matrixB[i * size + j];
            }
        }
        // Рассылка блоков матрицы
        for (int i = 1; i < procNum; i++) {
            int index = (i / grid.dimGrid) * size * blockSize + blockSize * (i % grid.dimGrid);
            MPI_Send(matrixA.data() + index, 1, matrixBlock, i, 0, grid.commGrid);
            MPI_Send(matrixB.data() + index, 1, matrixBlock, i, 1, grid.commGrid);
        }
    } else {
        // Примём блоков
        MPI_Recv(blockMatrixA.data(), blockSize * blockSize, MPI_DOUBLE, 0, 0, grid.commGrid, &status);
        MPI_Recv(blockMatrixB.data(), blockSize * blockSize, MPI_DOUBLE, 0, 1, grid.commGrid, &status);
    }

    Matrix blockMatrixC(blockSize * blockSize, 0);
    for (int l = 0; l < grid.dimGrid; l++) {
        Matrix tempA(blockSize * blockSize, 0);
        // Определение ведущего процесса в строке процессной решетки
        int pivot = (grid.numRow + l) % grid.dimGrid;
        // Копирование передаваемого блока в отдельный буфер памяти
        if (grid.numCol == pivot) {
            tempA = blockMatrixA;
        }
        MPI_Bcast(tempA.data(), blockSize * blockSize, MPI_DOUBLE, pivot, grid.commRow);

        // Умножение блоков
        MultiplyMatrixAndSum(tempA, blockMatrixB, blockMatrixC.data(), blockSize);

        // Циклический сдвиг блоков матрицы B в столбцах процессной решетки
        int dest = (grid.numRow == grid.dimGrid - 1 ? 0 : grid.numRow + 1);
        int source = (grid.numRow == 0 ? grid.dimGrid - 1 : grid.numRow - 1);
        MPI_Sendrecv_replace(blockMatrixB.data(), blockSize * blockSize, MPI_DOUBLE,
                            source, 0, dest, 0, grid.commCols, &status);
    }

    Matrix result(0);
    // Сбор данных
    if (procRank == 0) {
        result.resize(size * size);
        for (int i = 0; i < blockSize; i++) {
            for (int j = 0; j < blockSize; j++) {
                result[i * size + j ] = blockMatrixC[i * blockSize + j];
            }
        }
        for (int i = 1; i < procNum; i++) {
            MPI_Recv(result.data() + (i / grid.dimGrid) * size * blockSize + blockSize * (i % grid.dimGrid),
                    1, matrixBlock, i, 3, MPI_COMM_WORLD, &status);
        }
    } else {
        MPI_Send(blockMatrixC.data(), blockSize * blockSize, MPI_DOUBLE, 0, 3, MPI_COMM_WORLD);
    }
    MPI_Type_free(&matrixBlock);
    return result;
}
