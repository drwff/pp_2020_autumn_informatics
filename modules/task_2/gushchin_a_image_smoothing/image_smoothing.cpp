// Copyright 2020 Gushchin Artem
#include <mpi.h>
#include <random>
#include <vector>
#include <algorithm>
#include "../../../modules/task_2/gushchin_a_image_smoothing/image_smoothing.h"

template<typename T>
inline T clamp(T v, int max, int min) {
    if (v > max)
        return max;
    else if (v < min)
        return min;
    return v;
}

std::vector<uchar> generateRandomMatrix(const int rows, const int cols) {
    std::random_device rd;
    std::mt19937 gen;
    gen.seed(rd());

    std::vector<uchar> randomMatrix(rows * cols);

    for (int i = 0; i < rows * cols; i++)
        randomMatrix[i] = gen() % 255;

    return randomMatrix;
}

std::vector<uchar> medianFilter(const std::vector<uchar>& matrix, const int rows, const int cols, const int radius) {
    return rangeMedianFilter(matrix, rows, cols, radius, 0, 0, cols - 1, rows - 1);
}

// [startX, endX], [startY, endY] (inclusively)
std::vector<uchar> rangeMedianFilter(const std::vector<uchar>& matrix, const int rows, const int cols, const int radius,
                                     const int startX, const int startY, const int endX, const int endY) {
    std::vector<uchar> resultMatrix(static_cast<size_t>(rows) * cols);

    if (rows == 0 || cols == 0)
        return resultMatrix;

    int rank = 0, size = 4;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int windowSize = (2 * radius + 1) * (2 * radius + 1);
    std::vector<uchar> colors(windowSize);

    for (int y = startY; y <= endY; y++) {
        for (int x = startX; x <= endX; x++) {
            int k = 0;
            for (int i = -radius; i <= radius; i++) {
                for (int j = -radius; j <= radius; j++) {
                    int X = clamp(x + j, cols - 1, 0);
                    int Y = clamp(y + i, rows - 1, 0);
                    uchar color = matrix[static_cast<size_t>(X) + static_cast<size_t>(cols) * Y];
                    colors[k++] = color;
                }
            }
            std::sort(colors.begin(), colors.end());
            resultMatrix[static_cast<size_t>(x) + static_cast<size_t>(cols) * y] = colors[windowSize / 2];
        }
    }

    return resultMatrix;
}

std::vector<uchar> parallelMedianFilter(std::vector<uchar> matrix, const int rows, const int cols, const int radius) {
    if (!rows || !cols)
        throw "Rows and columns number should be greater than 0";

    int rank = 0, size = 4;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    const int delta = rows / size;
    const int remainder = rows % size;

    std::vector<int> startRows(size), endRows(size), rowsCounts(size), startRowsWithSurroundings(size),
                     endRowsWithSurroundings(size), rowsWithSurroundingsCount(size),
                     localStartRows(size), localEndRows(size);

    std::vector<int> displaces(size), sendCounts(size);

    for (int i = 0; i < size; i++) {
        if (i != 0)
            startRows[i] = endRows[static_cast<size_t>(i)-1] + 1;

        if (i < remainder)
            endRows[i] = startRows[i] + delta;
        else
            endRows[i] = startRows[i] + delta - 1;

        rowsCounts[i] = endRows[i] - startRows[i] + 1;

        if (rowsCounts[i]) {
            startRowsWithSurroundings[i] = std::max(startRows[i] - radius, 0);
            endRowsWithSurroundings[i] = std::min(endRows[i] + radius, rows - 1);

            rowsWithSurroundingsCount[i] = endRowsWithSurroundings[i] - startRowsWithSurroundings[i] + 1;

            sendCounts[i] = (endRowsWithSurroundings[i] - startRowsWithSurroundings[i] + 1) * cols;
            displaces[i] = startRowsWithSurroundings[i] * cols;

            localStartRows[i] = startRows[i] - startRowsWithSurroundings[i];
            localEndRows[i] = endRows[i] - startRowsWithSurroundings[i];
        }
    }

    std::vector<uchar> localMatrix(sendCounts[rank]);

    MPI_Scatterv(matrix.data(), sendCounts.data(), displaces.data(), MPI_UNSIGNED_CHAR,
                  localMatrix.data(), sendCounts[rank], MPI_UNSIGNED_CHAR, 0, MPI_COMM_WORLD);

    std::vector<uchar> localResultMatrix = rangeMedianFilter(localMatrix, rowsWithSurroundingsCount[rank],
                                                             cols, radius, 0, localStartRows[rank],
                                                             cols - 1, localEndRows[rank]);

    std::vector<uchar> truncatedLocalMatrix;
    if (rowsCounts[rank] != 0) {
        for (int y = localStartRows[rank]; y <= localEndRows[rank]; y++) {
            for (int x = 0; x <= cols - 1; x++) {
                truncatedLocalMatrix.push_back(localResultMatrix[static_cast<size_t>(x) +
                                                                 static_cast<size_t>(cols) * y]);
            }
        }
    }

    std::vector<int> recvCounts(size), recvDisplaces(size);
    int k = 0;
    for (int i = 0; i < size; i++) {
        recvCounts[i] = rowsCounts[i] * cols;

        recvDisplaces[i] = k;
        k += recvCounts[i];
    }

    std::vector<uchar> resultMatrix;
    if (rank == 0)
        resultMatrix.resize(static_cast<size_t>(rows) * cols);

    MPI_Gatherv(truncatedLocalMatrix.data(), recvCounts[rank], MPI_UNSIGNED_CHAR,
                resultMatrix.data(), recvCounts.data(), recvDisplaces.data(), MPI_UNSIGNED_CHAR, 0, MPI_COMM_WORLD);

    return resultMatrix;
}
