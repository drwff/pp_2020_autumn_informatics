// Copyright 2020 Bakaeva Maria
#include "../../../modules/task_2/bakaeva_m_horizontal_scheme/gauss_horizontal_scheme.h"
#include <mpi.h>
#include <stddef.h>
#include <math.h>
#include <ctime>
#include <random>


void getSequentialGauss(const double* matrixGlobal, double* result, int size) {
    double* matrix = new double[size * size + size];
    for (int i = 0; i < size * size + size; i++) {
        matrix[i] = matrixGlobal[i];
    }

    // Прямой ход метода Гаусса
    int pivotRow;
    int* pivotPos = new int[size];
    int* pivotIter = new int[size];

    for (int i = 0; i < size; i++) {
        pivotIter[i] = -1;
        pivotPos[i] = 0;
    }

    int iter;  // Номер текущей итерации прямого хода

    for (iter = 0; iter < size; iter++) {
        // Ищем ведущую строку
        double maxValue = 0;
        for (int i = 0; i < size; i++) {
            if ((pivotIter[i] == -1) && ((fabs(matrix[i * (size + 1) + iter]) > maxValue))) {
                pivotRow = i;
                maxValue = matrix[i * (size + 1) + iter];
                }
        }

        pivotPos[iter] = pivotRow;
        pivotIter[pivotRow] = iter;

        // Вычитание
        double pivotValue, pivotFactor;
        pivotValue = matrix[pivotRow * (size + 1) + iter];
        for (int i = 0; i < size; i++) {
            if (pivotIter[i] == -1) {
                pivotFactor = matrix[i * (size + 1) + iter] / pivotValue;
                for (int j = iter; j < size; j++) {
                    matrix[i * (size + 1) + j] -= pivotFactor * matrix[pivotRow * (size + 1) + j];
                }

                matrix[i * (size + 1) + size] -= pivotFactor * matrix[pivotRow * (size + 1) + size];
            }
        }
    }

    // Обратный ход метода Гаусса
    int rowIdx, row;
    for (int i = (size - 1); i >= 0; i--) {
        rowIdx = pivotPos[i];
        result[i] = matrix[rowIdx * (size + 1) + size] / matrix[rowIdx * (size + 1) + i];
        matrix[rowIdx * (size + 1) + i] = 1;

        for (int j = 0; j < i; j++) {
            row = pivotPos[j];
            matrix[row * (size + 1) + size] -= matrix[row * (size + 1) + i] * result[i];
            matrix[row * (size + 1) + i] = 0;
        }
    }

    delete[] matrix;
    delete[] pivotPos;
    delete[] pivotIter;
}

void getParallelGauss(const double* matrixGlobal, double* result, int cols, int rows) {
    double* matrix = new double[rows * cols];
    for (int i = 0; i < rows * cols; i++) {
        matrix[i] = matrixGlobal[i];
    }

    int procSize, rank;
    //Число задействованных процессов
    MPI_Comm_size(MPI_COMM_WORLD, &procSize);
    //Получение номера текущего процесса в рамках коммуникатора
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // Количество строк для каждого процесса
    const int delta = rows / procSize;
    // Остаток
    const int remainder = rows % procSize;

    int p;
    if (rank < remainder) {
        p = delta + 1;
    } else {
        p = delta;
    }

    double* subMatrix = new double[cols * p];
    int* send_counts = new int[procSize];
    int* displs = new int[procSize];

    displs[0] = 0;
    for (int i = 0; i < procSize; i++) {
        if (i < remainder) {
            send_counts[i] = (delta + 1) * cols;
        } else {
            send_counts[i] = delta * cols;
        }
        if (i > 0) {
            displs[i] = displs[i - 1] + send_counts[i - 1];
        }
    }

    // Отправление данных всем процессам
    MPI_Scatterv(matrix, send_counts, displs, MPI_DOUBLE, subMatrix, p * cols, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    double* pivotRow = new double[cols];

    for (int i = 0; i < displs[rank] / cols; i++) {
        int root = 0;
        int sum = 0;
        for (int j = 0; j < procSize; j++) {
            sum += send_counts[j] / cols;
            if (i < sum) {
                root = j;
                break;
            }
            root++;
        }

        MPI_Bcast(pivotRow, cols, MPI_DOUBLE, root, MPI_COMM_WORLD);

        for (int j = 0; j < send_counts[rank] / cols; j++) {
            double s = pivotRow[i] / subMatrix[j * cols + i];
            for (int z = i; z < cols; z++) {
                subMatrix[j * cols + z] = s * subMatrix[j * cols + z] - pivotRow[z];
            }
        }
    }

    for (int i = 0; i < send_counts[rank] / cols; i++) {
        for (int j = 0; j < cols; j++) {
            pivotRow[j] = subMatrix[i * cols + j];
        }
        MPI_Bcast(pivotRow, cols, MPI_DOUBLE, rank, MPI_COMM_WORLD);

        for (int j = i + 1; j < send_counts[rank] / cols; j++) {
            double s = pivotRow[displs[rank] / cols + i] / subMatrix[j * cols + i + displs[rank] / cols];
            for (int z = i + displs[rank] / cols; z < cols; z++) {
                subMatrix[j * cols + z] = s * subMatrix[j * cols + z] - pivotRow[z];
            }
        }
    }

    int root = 0;
    for (int i = displs[rank] / cols + send_counts[rank] / cols; i < rows; i++) {
        int sum = 0;
        for (int j = 0; j < procSize; j++) {
            sum += send_counts[j] / cols;
            if (i < sum) {
                root = j;
                break;
            }
            root++;
        }
        MPI_Bcast(pivotRow, cols, MPI_DOUBLE, root, MPI_COMM_WORLD);
    }

    MPI_Gatherv(subMatrix, p * cols, MPI_DOUBLE, matrix, send_counts, displs, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        for (int i = rows - 1; i >= 0; i--) {
            double b = matrix[i * cols + cols - 1];
            for (int j = rows - 1; j >= i + 1; j--) {
                b -= matrix[i * cols + j] * result[j];
            }
            result[i] = b / matrix[i * cols + i];
        }
    }

    delete[] matrix;
    delete[] subMatrix;
    delete[] send_counts;
    delete[] displs;
    delete[] pivotRow;
}
