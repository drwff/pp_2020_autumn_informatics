// Copyright 2020 Gogov Vladislav
#include <mpi.h>
#include <iostream>
#include <vector>
#include <random>
#include <cmath>
#include <limits>
#include "../../../modules/task_2/gogov_v_block_striped_columnwise/block_striped_columnwise.h"

bool assertVectors(const std::vector<double>& vec1, const std::vector<double>& vec2) {
    if (vec1.size() != vec2.size())
        throw "Different size";
    for (size_t i = 0; i < vec1.size(); i++) {
        if ((std::fabs(vec1[i] - vec2[i]) >= std::numeric_limits<double>::epsilon() * 100000000.0))
            return false;
    }
    return true;
}

void printVector(const std::vector<double>& vec, int vecSize) {
    std::cout << "Vector print:" << std::endl;
    for (int i = 0; i < vecSize; i++)
        std::cout << vec[i] << std::endl;
}

void printMatrix(const std::vector<double>& vec, int rows, int cols) {
    std::cout << "Matrix print:" << std::endl;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            std::cout << vec[i * cols + j] << "  ";
        }
        std::cout << std::endl;
    }
}

std::vector<double> transpositionMatrix(const std::vector<double>& matrix, int rows, int cols) {
    std::vector<double> result(rows * cols);
    size_t ind = 0;
    for (int i = 0; i < cols; i++) {
        for (int j = 0; j < rows; j++)
            result[ind++] = matrix[j * cols + i];
    }
    return result;
}

Matrix createRandomMatrix(int rows, int cols) {
    std::random_device rd;
    std::mt19937 mersenne(rd());
    std::uniform_real_distribution<> urd(-50.0 , 50.0);
    Matrix matrix(rows * cols);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            matrix[i * cols + j] = static_cast<double>(urd(mersenne) / 1e10 * 1e10);
        }
    }
    return matrix;
}

std::vector<double> multMatrixByVectorSequential(const Matrix& matrix, int rows, int cols,
                                            const std::vector<double>& vec, int vecSize) {
    if (cols != vecSize) {
        throw("Invalid size");
    }
    std::vector<double> result(rows, 0);
    for (int i = 0; i < cols; i++) {
        for (int j = 0; j < rows; j++) {
            result[j] += matrix[j * cols + i] * vec[i];
        }
    }
    return result;
}

std::vector<double> multColsByVector(const Matrix& matrix, int rows, int cols,
                                    const std::vector<double>& vec, int vecSize) {
    if (rows != vecSize) {
        throw("Invalid size");
    }
    std::vector<double> result(cols, 0);
    for (int i = 0; i < cols; i++) {
        for (int j = 0; j < rows; j++) {
            result[i] += matrix[j * cols + i] * vec[j];
        }
    }
    return result;
}

void arraysDistribution(int* sendNum, int* sendInd, int* sendNumSize, int* sendMatrixInd,
                        int n, int procNum, int remain, int count) {
    sendNum[0] = (remain != 0 ? n + 1 : n);
    sendInd[0] = 0;
    sendNumSize[0] = sendNum[0] * count;
    sendMatrixInd[0] = 0;
    for (int i = 1; i < procNum; i++) {
        sendNum[i] = (i < remain ? n + 1 : n);
        sendNumSize[i] = sendNum[i] * count;
        sendInd[i] = sendInd[i - 1] + sendNum[i - 1];
        sendMatrixInd[i] = sendMatrixInd[i - 1] + sendNumSize[i - 1];
    }
}

std::vector<double> sumMatrixByCols(const std::vector<double>& vec, int rows, int cols) {
    std::vector<double> result(cols, 0);
    for (int i = 0; i < cols; i++) {
        double temp = 0;
        for (int j = 0; j < rows; j++) {
            temp += vec[j * cols + i];
        }
        result[i] = temp;
    }
    return result;
}

std::vector<double> multMatrixByVectorParallel(const Matrix& matrix, int rows, int cols,
                                            const std::vector<double>& vec, int vecSize) {
    if (cols != vecSize) {
        throw("Invalid size");
    }

    int procNum, procRank;
    MPI_Comm_size(MPI_COMM_WORLD, &procNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
    int n = cols / procNum;
    int remain = cols - n * procNum;

    //  Определение массивов для ScatterV
    std::vector<int> sendNum(procNum);
    std::vector<int> sendInd(procNum);
    std::vector<int> sendNumSize(procNum);
    std::vector<int> sendMatrixInd(procNum);
    arraysDistribution(sendNum.data(), sendInd.data(), sendNumSize.data(), sendMatrixInd.data(),
                        n, procNum, remain, rows);

    //  Отправка вектора
    std::vector<double> partVector(sendNum[procRank]);
    MPI_Scatterv(vec.data(), sendNum.data(), sendInd.data(), MPI_DOUBLE,
                partVector.data(), sendNum[procRank], MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // Отправка транспонированной матрицы
    std::vector<double> colVector(sendNumSize[procRank]);
    MPI_Scatterv(matrix.data(), sendNumSize.data(), sendMatrixInd.data(), MPI_DOUBLE,
                colVector.data(), sendNumSize[procRank], MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // Умножение столбцов на вектор
    std::vector<double> localVector = multColsByVector(colVector, sendNum[procRank], rows,
                                                        partVector, sendNum[procRank]);

    //  Сбор промежуточных векторов
    std::vector<double> tempMatrix(procNum * rows);
    MPI_Gather(localVector.data(), static_cast<int>(localVector.size()), MPI_DOUBLE,
                tempMatrix.data(), rows, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // Суммирование столбцов
    std::vector<double> resultVector(0);
    if (procRank == 0) {
        resultVector = sumMatrixByCols(tempMatrix, procNum, rows);
    }
    return resultVector;
}
