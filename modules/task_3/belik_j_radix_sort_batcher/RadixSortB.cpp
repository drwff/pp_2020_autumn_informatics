// Copyright 2020 Belik Julia
#include <mpi.h>
#include <ctime>
#include <vector>
#include <random>
#include <algorithm>
#include <iostream>
#include <utility>
#include "../../../modules/task_3/belik_j_radix_sort_batcher/RadixSortB.h"
std::vector<int> MergeBatcher(std::vector<int> vec, int n) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    const int countint = n / size;
    const int rem = n % size;
    std::vector<int> locvec;
    if ((n < size * 2) || (size == 1)) {
        if (rank == 0)
            vec = RadixSort(vec);
        return vec;
    }
    if (rank == 0)
        locvec.reserve(n);
    locvec.resize(rank == 0 ? countint + rem : countint);
    MPI_Status status;
    if (rank == 0) {
        for (int i = 0; i < countint + rem; i++) {
            locvec[i] = vec[i];
        }
        for (int i = 1; i < size; i++) {
            MPI_Send(vec.data() + i * countint + rem, countint, MPI_INT, i, 1, MPI_COMM_WORLD);
        }
    } else {
        MPI_Recv(locvec.data(), countint, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
    }
    int nummerge = 1, pow = 2;
    while (pow < size) {
        pow *= 2;
        nummerge++;
    }
    locvec = RadixSort(locvec);
    locvec = Shuffle(locvec);
    int mergecount = 2, offset = 1, lengths, lengthr;
    for (int i = 0; i < nummerge; i++) {
        std::vector<int> res, even, odd;
        if ((rank % mergecount == 0) && (rank + offset < size)) {
            lengths = locvec.size() / 2;
            MPI_Sendrecv(&lengths, 1, MPI_INT, rank + offset, 0, &lengthr, 1, MPI_INT, rank + offset,
                0, MPI_COMM_WORLD, &status);
            res.resize(lengthr / 2 + lengthr % 2);
            MPI_Sendrecv(&locvec[locvec.size() / 2 + locvec.size() % 2], lengths, MPI_INT, rank + offset, 0,
                &res.front(), lengthr / 2 + lengthr % 2, MPI_INT, rank + offset, 0, MPI_COMM_WORLD, &status);
            even = EvenMerge(locvec, res);
            odd.resize(lengthr / 2 + locvec.size() / 2);
            MPI_Recv(&odd.front(), lengthr / 2 + locvec.size() / 2, MPI_INT, rank + offset,
                0, MPI_COMM_WORLD, &status);
            locvec.resize(even.size() + odd.size());
            for (size_t i = 0; i < even.size(); i++)
                locvec[i] = even[i];
            for (size_t i = 0; i < odd.size(); i++)
                locvec[i + even.size()] = odd[i];
            if (i + 1 != nummerge)
                locvec = Transpos(locvec, even.size(), odd.size());
            else
                locvec = Merge(locvec, even.size(), odd.size());
        }
        if ((rank - offset >= 0) && ((rank - offset) % mergecount == 0)) {
            lengths = locvec.size();
            MPI_Sendrecv(&lengths, 1, MPI_INT, rank - offset, 0, &lengthr, 1, MPI_INT, rank - offset,
                0, MPI_COMM_WORLD, &status);
            res.resize(lengthr);
            MPI_Sendrecv(locvec.data(), lengths / 2 + lengths % 2, MPI_INT, rank - offset, 0,
                res.data(), lengthr, MPI_INT, rank - offset, 0, MPI_COMM_WORLD, &status);
            odd = OddMerge(locvec, res);
            MPI_Send(odd.data(), odd.size(), MPI_INT, rank - offset, 0, MPI_COMM_WORLD);
        }
        mergecount *= 2;
        offset *= 2;
    }
    return locvec;
}
std::vector<int> Shuffle(std::vector<int> vec) {
    std::vector<int> tmp(vec.size());
    for (size_t i = 0; i < vec.size(); i++)
        tmp[i / 2 + (i % 2) * (vec.size() / 2 + vec.size() % 2)] = vec[i];
    return tmp;
}
std::vector<int> EvenMerge(const std::vector<int>& v1, const std::vector<int>& v2) {
    if (v1.empty())
        return v2;
    if (v2.empty())
        return v1;
    std::vector<int> res(v1.size() / 2 + v1.size() % 2 + v2.size());
    size_t i = 0, j = 0, k = 0;
    while ((i < (v1.size() / 2 + v1.size() % 2)) && (j < (v2.size()))) {
        if (v1[i] < v2[j])
            res[k++] = v1[i++];
        else
            res[k++] = v2[j++];
    }
    while (i < v1.size() / 2 + v1.size() % 2)
        res[k++] = v1[i++];
    while (j < v2.size())
        res[k++] = v2[j++];
    return res;
}
std::vector<int> OddMerge(const std::vector<int>& v1, const std::vector<int>& v2) {
    if (v1.empty())
        return v2;
    if (v2.empty())
        return v1;
    std::vector<int> res(v1.size() / 2 + v2.size());
    size_t i = v1.size() / 2 + v1.size() % 2, j = 0, k = 0;
    while ((i < v1.size()) && (j < v2.size())) {
        if (v1[i] < v2[j])
            res[k++] = v1[i++];
        else
            res[k++] = v2[j++];
    }
    while (i < v1.size())
        res[k++] = v1[i++];
    while (j < v2.size())
        res[k++] = v2[j++];
    return res;
}
std::vector<int> Merge(std::vector<int> vec, int evencount, int oddcount) {
    std::vector<int> res(vec.size());
    int i = 0, j = 0, k = 0;
    while ((i < evencount) && (j < oddcount)) {
        res[k++] = vec[i++];
        res[k++] = vec[evencount + j];
        j++;
    }
    while (i < evencount)
        res[k++] = vec[i++];
    for (size_t h = 1; h < res.size() - 1; h += 2) {
        if (res[h] > res[h + 1])
            std::swap(res[h], res[h + 1]);
    }
    return res;
}
std::vector<int> Transpos(std::vector<int> vec, int evencount, int oddcount) {
    if (evencount - oddcount == 2) {
        std::vector<int> res(vec.size());
        int i = 0, j = 0, k = 0;
        while ((i < evencount) && (j < oddcount)) {
            res[k++] = vec[i++];
            res[k++] = vec[evencount + j];
            j++;
        }
        while (i < evencount)
            res[k++] = vec[i++];
        for (size_t h = 1; h < res.size() - 1; h += 2) {
            if (res[h] > res[h + 1])
                std::swap(res[h], res[h + 1]);
        }
        res = Shuffle(res);
        return res;
    } else {
        for (int i = 0; i < evencount - 1; i++)
            if (vec[1 + i] < vec[evencount + i])
                std::swap(vec[1 + i], vec[evencount + i]);
        return vec;
    }
}
std::vector<int> RadixSort(std::vector<int> vec) {
    int max = 0, pos = 1;
    std::vector<int> res(vec.size());
    if ((vec.size() == 1) || (vec.size() == 0))
        return vec;
    for (size_t i = 1; i < vec.size(); i++)
        if (max < vec[i])
            max = vec[i];
    while (max / pos > 0) {
        int countdigit[10] = { 0 };
        for (size_t i = 0; i < vec.size(); i++)
            countdigit[vec[i] / pos % 10]++;
        for (int i = 1; i < 10; i++)
            countdigit[i] += countdigit[i - 1];
        for (int i = vec.size() - 1; i >= 0; i--)
            res[--countdigit[vec[i] / pos % 10]] = vec[i];
        for (size_t i = 0; i < vec.size(); i++)
            vec[i] = res[i];
        pos *= 10;
    }
    return vec;
}
std::vector<int> Vector(int n) {
    std::mt19937 gen;
    gen.seed(static_cast<unsigned int>(time(0)));
    std::vector<int> vec(n);
    for (int i = 0; i < n; i++)
        vec[i] = gen() % 10;
    return vec;
}
