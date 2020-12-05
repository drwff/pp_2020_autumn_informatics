// Copyright 2020 Kulandin Denis
#include <mpi.h>
#include <ctime>
#include <iostream>
#include <random>
#include <utility>
#include <vector>
#include "../../../modules/task_3/kulandin_d_quick_sort/quick_sort.h"

std::mt19937 gen(time(0));

int* randomVector(int sz) {
    int* ans = new int[sz];
    for (int i = 0; i < sz; ++i) {
        ans[i] = gen();
    }
    return ans;
}

int* copyVector(int* a, int sz) {
    int* ans = new int[sz];
    for (int i = 0; i < sz; ++i) {
        ans[i] = a[i];
    }
    return ans;
}

void division(int* a, int l, int r, int* t) {
    int mid = a[(l + r) / 2];
    while (l <= r) {
        while (a[l] < mid) l++;
        while (a[r] > mid) r--;
        if (l >= r) break;
        std::swap(a[l++], a[r--]);
    }
    *t = r;
}

void quickSort(int* a, int l, int r) {
    if (l >= r) return;
    int t = 0;
    division(a, l, r, &t);
    quickSort(a, l, t);
    quickSort(a, t + 1, r);
}

void parallelQuickSortImpl(int* a, int arrSize, int curRank, int maxRank, int power) {
    MPI_Status status;

    int nextProc = curRank + (1 << power);
    ++power;
    if (nextProc > maxRank) {
        quickSort(a, 0, arrSize - 1);
        return;
    }
    int t = 0;
    division(a, 0, arrSize - 1, &t);
    if (t + 1 < arrSize - t - 1) {
        MPI_Send(a, t + 1, MPI_INT, nextProc, t, MPI_COMM_WORLD);
        parallelQuickSortImpl(a + t + 1, arrSize - t - 1, curRank, maxRank, power);
        MPI_Recv(a, t + 1, MPI_INT, nextProc, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
    } else {
        MPI_Send(a + t + 1, arrSize - t - 1, MPI_INT, nextProc, t + 1, MPI_COMM_WORLD);
        parallelQuickSortImpl(a, t + 1, curRank, maxRank, power);
        MPI_Recv(a + t + 1, arrSize - t - 1, MPI_INT, nextProc, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
    }
}

void parallelQuickSort(int* a, int length) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int power = 0;

    while ((1 << power) <= rank) {
        ++power;
    }

    if (!rank) {
        parallelQuickSortImpl(a, length, rank, size - 1, power);
    } else {
        MPI_Status status;
        int subSize;
        MPI_Probe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        MPI_Get_count(&status, MPI_INT, &subSize);

        int source = status.MPI_SOURCE;
        int* tmp = new int[subSize];
        MPI_Recv(tmp, subSize, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        parallelQuickSortImpl(tmp, subSize, rank, size - 1, power);
        MPI_Send(tmp, subSize, MPI_INT, source, 0, MPI_COMM_WORLD);
    }
}
