// Copyright 2020 Tarasov Nikita
#include <mpi.h>
#include <iostream>
#include <ctime>
#include <random>
#include <vector>
#include <limits>
#include "../../../modules/task_3/tarasov_n_quick_sort/quick_sort.h"

std::vector<int> random_array(int _size) {
    if (_size <= 0) throw "ErrorLentgh";
    std::default_random_engine random;
    random.seed(static_cast<unsigned int>(std::time(0)));
    std::vector<int> arr(_size);
    for (int i = 0; i < static_cast<int>(arr.size()); i++)
        arr[i] = random() % 100;
    return arr;
}

void print_array(std::vector<int> vec) {
    for (int i = 0; i < static_cast<int>(vec.size()); i++)
        std::cout << vec[i] << " ";
    std::cout << std::endl;
}

void quick_sort(std::vector<int> *vec, int left, int right) {
    if (static_cast<int>(vec->size()) < 1) {
        throw "ErrorLentgh";
    }
    if (static_cast<int>(vec->size()) == 1) {
        return;
    }

    int l = left, r = right;
    int pivot = (*vec)[(left + right) / 2];
    int temp;
    do {
        while ((*vec)[l] < pivot) {
            l++;
        }
        while ((*vec)[r] > pivot) {
            r--;
        }
        if (l <= r) {
            temp = (*vec)[l];
            (*vec)[l] = (*vec)[r];
            (*vec)[r] = temp;
            l++;
            r--;
        }
    } while (l <= r);

    if (l < right) {
        quick_sort(vec, l, right);
    }
    if (left < r) {
        quick_sort(vec, left, r);
    }
}

void quick_sort_mpi(std::vector<int> *vec) {
    if (vec->size() < 1) {
        throw "ErrorLentgh";
    }
    if (vec->size() == 1) {
        return;
    }

    int mpisize, mpirank;
    int mpiroot = 0;

    MPI_Comm_size(MPI_COMM_WORLD, &mpisize);
    MPI_Comm_rank(MPI_COMM_WORLD, &mpirank);

    int size = static_cast<int>(vec->size());
    int delta = size / mpisize;
    int rem = size % mpisize;

    if (delta < 1) quick_sort(vec, 0, size - 1);

    int scount_size = mpisize;
    int displs_size = mpisize;

    std::vector<int> scount(scount_size);
    std::vector<int> displs(displs_size);

    if (rem != 0)
        scount[0] = delta + rem;
    else
        scount[0] = delta;
    for (int i = 1; i < scount_size; i++) {
        scount[i] = delta;
    }
    displs[0] = 0;
    for (int i = 1; i < displs_size; i++) {
        displs[i] = displs[i - 1] + scount[i - 1];
    }

    std::vector<int> rec_buf(scount[mpirank]);

    MPI_Scatterv(&(*vec)[0], &scount[0], &displs[0], MPI_INT,
        &rec_buf[0], scount[mpirank], MPI_INT, mpiroot, MPI_COMM_WORLD);

    quick_sort(&rec_buf, 0, scount[mpirank] - 1);

    int j = 0;
    int i = 0;

    while (i < size) {
        int min_buf;
        int global_counter = 0;
        int counter = 0;
        MPI_Allreduce(&rec_buf[j], &min_buf, 1, MPI_INT, MPI_MIN, MPI_COMM_WORLD);
        if (rec_buf[j] == min_buf) {
            rec_buf[j] = std::numeric_limits<int>::max();
            if (j < static_cast<int>(rec_buf.size()) - 1) j++;

            global_counter++;
        }
        MPI_Reduce(&global_counter, &counter, 1, MPI_INT, MPI_SUM, mpiroot, MPI_COMM_WORLD);
        if (mpirank == mpiroot) {
            int tmp = i + counter;
            for (int k = i; k < tmp; k++) {
                (*vec)[k] = min_buf;
                i++;
            }
        }
        MPI_Bcast(&i, 1, MPI_INT, mpiroot, MPI_COMM_WORLD);
    }
}
