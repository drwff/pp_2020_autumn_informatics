// Copyright 2020 Sidorova Alexandra

#include <mpi.h>
#include <memory>
#include <random>
#include <ctime>
#include <vector>
#include <cstring>
#include <algorithm>
#include <limits>
#include <utility>
#include "../../../modules/task_3/sidorova_a_radix_sort/radix_sort.h"

#define EPSILON 0.0001
#define BITS 256

std::vector<std::pair<size_t, size_t>> comparators;
static uint16_t offset = 0;

std::vector<double> getRandomVector(const size_t size) {
    if (size <= 0)
        throw "[Error] Incorrect array size. The size must be positive";

    std::mt19937 gen;
    gen.seed(static_cast<unsigned int>(time(0) + offset * 10));
    offset++;
    std::vector<double> vector(size);
    for (double& value : vector)
        value = gen() % 100;

    return vector;
}

std::vector<double> radixSortParallel(const std::vector<double>& src) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int srcSize = -1;
    if (rank == 0)
        srcSize = src.size();
    MPI_Bcast(&srcSize, 1, MPI_INT, 0, MPI_COMM_WORLD);

    if (srcSize <= 0)
        throw "[Error] Incorrect array size. The size must be positive";

    std::vector<double> dst(srcSize);
    if (size == 1 || size >= srcSize) {
        if (rank == 0)
            dst = radixSortSequential(src);
        return dst;
    }

    std::vector<double> tmp;
    size_t fictiveValues = 0;
    if (rank == 0) {
        tmp = src;

        while (srcSize % size) {
            tmp.push_back(std::numeric_limits<double>::max());
            ++srcSize;
            ++fictiveValues;
        }
    }

    MPI_Bcast(&srcSize, 1, MPI_INT, 0, MPI_COMM_WORLD);
    dst.resize(srcSize);

    int localSize = srcSize / size;

    std::vector<double> localVector(localSize);
    std::vector<double> bufferVector(localSize);
    std::vector<double> tmpVector(localSize);

    MPI_Scatter(tmp.data(), localSize, MPI_DOUBLE, &localVector[0], localSize, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    localVector = radixSortSequential(localVector);

    batcherNetwork(size);

    for (size_t i = 0; i < comparators.size(); ++i) {
        size_t first = comparators[i].first;
        size_t second = comparators[i]. second;
        MPI_Status status;

        if (rank == static_cast<int>(first)) {
            MPI_Send(localVector.data(), localSize, MPI_DOUBLE, second, 0, MPI_COMM_WORLD);
            MPI_Recv(&bufferVector[0], localSize, MPI_DOUBLE, second, 0, MPI_COMM_WORLD, &status);

            for (size_t locIdx = 0, bufferIdx = 0, tmpIdx = 0; tmpIdx < static_cast<size_t>(localSize); ++tmpIdx) {
                double localValue = localVector[locIdx];
                double bufferValue = bufferVector[bufferIdx];

                if (localValue < bufferValue) {
                    tmpVector[tmpIdx] = localValue;
                    ++locIdx;
                } else {
                    tmpVector[tmpIdx] = bufferValue;
                    ++bufferIdx;
                }
            }

            localVector = tmpVector;
        } else if (rank == static_cast<int>(second)) {
            MPI_Recv(&bufferVector[0], localSize, MPI_DOUBLE, first, 0, MPI_COMM_WORLD, &status);
            MPI_Send(localVector.data(), localSize, MPI_DOUBLE, first, 0, MPI_COMM_WORLD);

            int startIndex = localSize - 1;
            for (int locIdx = startIndex, bufferIdx = startIndex, tmpIdx = startIndex; tmpIdx >= 0; --tmpIdx) {
                double localValue = localVector[locIdx];
                double bufferValue = bufferVector[bufferIdx];

                if (localValue > bufferValue) {
                    tmpVector[tmpIdx] = localValue;
                    --locIdx;
                } else {
                    tmpVector[tmpIdx] = bufferValue;
                    --bufferIdx;
                }
            }

            localVector = tmpVector;
        }
    }

    MPI_Gather(localVector.data(), localSize, MPI_DOUBLE, &dst[0], localSize, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    if (rank == 0 && fictiveValues != 0)
        dst.erase(dst.begin() + srcSize - fictiveValues, dst.end());

    return dst;
}

std::vector<double> radixSortSequential(const std::vector<double>& src) {
    const size_t size = src.size();
    if (size <= 0)
        throw "[Error] Incorrect array size. The size must be positive";

    std::vector<double> dst(size);
    std::vector<double> tmp(src);

    for (size_t i = 0; i < 7; ++i) {
        dst = radixSortPass(tmp, static_cast<uint16_t>(i));
        std::swap(tmp, dst);
    }

    return radixSortLastPass(tmp, 7);
}

std::vector<double> radixSortPass(const std::vector<double>& src, const uint16_t offset) {
    const size_t size = src.size();
    std::vector<double> dst(size);
    unsigned char* bytes = (unsigned char*)src.data();
    size_t* counters = new size_t[BITS];
    memset(counters, 0, BITS * sizeof(size_t));

    for (size_t i = 0; i < size; ++i)
        counters[bytes[8 * i + offset]]++;

    size_t sum = 0;
    for (size_t i = 0; i < BITS; ++i) {
        size_t tmp = counters[i];
        counters[i] = sum;
        sum += tmp;
    }

    for (size_t i = 0; i < size; ++i) {
        size_t idx = 8 * i + offset;
        dst[counters[bytes[idx]]] = src[i];
        counters[bytes[idx]]++;
    }

    delete[] counters;

    return dst;
}

std::vector<double> radixSortLastPass(const std::vector<double>& src, const uint16_t offset) {
    const size_t size = src.size();
    std::vector<double> dst(size);
    unsigned char* bytes = (unsigned char*)src.data();
    size_t* counters = new size_t[BITS];
    memset(counters, 0, BITS * sizeof(size_t));

    for (size_t i = 0; i < size; ++i)
        counters[bytes[8 * i + offset]]++;

    size_t countOfNegative = 0;
    for (size_t i = 128; i < BITS; ++i)
        countOfNegative += counters[i];

    size_t sum = countOfNegative;
    for (size_t i = 0; i < 128; ++i) {
        size_t tmp = counters[i];
        counters[i] = sum;
        sum += tmp;
    }

    sum = 0;
    for (size_t i = BITS - 1; i >= 128; --i) {
        counters[i] += sum;
        sum = counters[i];
    }

    for (size_t i = 0; i < size; ++i) {
        size_t index = 8 * i + offset;

        if (bytes[index] < 128)
            dst[counters[bytes[index]]++] = src[i];
        else
            dst[--counters[bytes[index]]] = src[i];
    }

    delete[] counters;

    return dst;
}

void batcherNetwork(const size_t processCount) {
    if (processCount <= 1)
        return;

    std::vector<size_t> processRanks(processCount);
    for (size_t i = 0; i < processCount; ++i)
        processRanks[i] = i;

    buildBatcherNetwork(processRanks);
}

void buildBatcherNetwork(const std::vector<size_t>& processRanks) {
    size_t size = processRanks.size();

    if (size <= 1)
        return;

    std::vector<size_t> leftVector(size / 2);
    std::vector<size_t> rightVector(size / 2 + size % 2);

    std::copy(processRanks.begin(), processRanks.begin() + leftVector.size(), leftVector.begin());
    std::copy(processRanks.begin() + leftVector.size(), processRanks.end(), rightVector.begin());

    buildBatcherNetwork(leftVector);
    buildBatcherNetwork(rightVector);
    buildBatcherComparators(leftVector, rightVector);
}

void buildBatcherComparators(const std::vector<size_t>& leftVector, const std::vector<size_t>& rightVector) {
    size_t processCount = leftVector.size() + rightVector.size();

    if (processCount <= 1)
        return;

    if (processCount == 2) {
        comparators.push_back(std::pair<size_t, size_t>(leftVector[0], rightVector[0]));
        return;
    }

    std::vector<size_t> leftVectorOdd, leftVectorEven;
    std::vector<size_t> rightVectorOdd, rightVectorEven;

    for (size_t i = 0; i < leftVector.size(); ++i)
        i % 2 ? leftVectorEven.push_back(leftVector[i]) : leftVectorOdd.push_back(leftVector[i]);
    for (size_t i = 0; i < rightVector.size(); ++i)
        i % 2 ? rightVectorEven.push_back(rightVector[i]) : rightVectorOdd.push_back(rightVector[i]);

    buildBatcherComparators(leftVectorOdd, rightVectorOdd);
    buildBatcherComparators(leftVectorEven, rightVectorEven);

    std::vector<size_t> result(processCount);
    std::copy(leftVector.begin(), leftVector.end(), result.begin());
    std::copy(rightVector.begin(), rightVector.end(), result.begin() + leftVector.size());

    for (size_t i = 1; i < processCount - 1; i += 2)
        comparators.push_back(std::pair<size_t, size_t>(result[i], result[i + 1]));
}
