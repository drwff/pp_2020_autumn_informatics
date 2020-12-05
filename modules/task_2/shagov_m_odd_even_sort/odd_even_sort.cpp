// Copyright 2020 Shagov Maksim
#include <mpi.h>
#include <iostream>
#include <ctime>
#include <random>
#include <algorithm>
#include <vector>
#include "../../../modules/task_2/shagov_m_odd_even_sort/odd_even_sort.h"

std::vector<int> createRandomVector(int  sz) {
    std::vector<int> result(sz);
    std::random_device rd;
    std::mt19937 mersenne(rd());
    std::uniform_real_distribution<> urd(-50.0 , 50.0);
    for (int i = 0; i < sz; i++) {
        result[i] = static_cast<int>(urd(mersenne));
    }
    return result;
}

void bubbleSort(int *arr, int arr_size) {
     for (int i = 0; i < arr_size - 1; i++) {
        for (int j = 0; j < arr_size - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

void blockMergeArrays(int *arr1, int *arr2, int size1, int size2) {
    int i1 = 0, i2 = 0, k = 0;
    std::vector<int> result(size1 + size2);
    while ((i1 < size1) && (i2 < size2)) {
        if (arr1[i1] < arr2[i2]) {
            result[k] = arr1[i1];
            i1++;
            k++;
        } else {
            result[k] = arr2[i2];
            i2++;
            k++;
        }
    }
    while ((i1 == size1) && (i2 < size2)) {
        result[k] = arr2[i2];
        i2++;
        k++;
    }
    while ((i2 == size2) && (i1 < size1)) {
        result[k] = arr1[i1];
        i1++;
        k++;
    }
    k = 0;
    for (int i = 0; i < size1; i++, k++) {
        arr1[i] = result[k];
    }
    for (int i = 0; i < size2; i++, k++) {
        arr2[i] = result[k];
    }
}

void oddEvenParallelSort(int* arr, int arr_size) {
    double startT = 0;
    int rank, procCount;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &procCount);
    if (rank == 0) {
        startT = MPI_Wtime();
    }
    MPI_Status status;
    int elements_count = arr_size;
    int elements_part_count = elements_count / procCount;
    int remainder = elements_count % procCount;
    int *revcount = new int[procCount];
    int *displs = new int[procCount];
    revcount[0] = elements_part_count + remainder;
    displs[0] = 0;
    std::vector<int> part_vector(rank == 0 ? elements_part_count + remainder : elements_part_count);
    if (procCount > elements_count) {
        if (rank == 0) {
            for (int i = 0; i < arr_size; i++) {
                part_vector[i] = arr[i];
            }
        }
    } else {
        for (int i = 1; i < procCount; i++) {
            revcount[i] = elements_part_count;
            displs[i] = elements_part_count * i + remainder;
        }
        MPI_Scatterv(arr, revcount, displs, MPI_INT, &part_vector[0], revcount[rank], MPI_INT, 0, MPI_COMM_WORLD);
    }
    bubbleSort(part_vector.data(), part_vector.size());
    int m = 0;
    std::vector<int> sub;
    if (procCount <= elements_count) {
    for (int i = 0; i < procCount + procCount/2; i++) {
        if (i % 2 == 1) {
            if (rank % 2 == 1) {
                if (rank < procCount - 1) {
                    elements_part_count = part_vector.size();
                    MPI_Send(&elements_part_count, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);
                    MPI_Send(&part_vector[0], elements_part_count, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);
                    MPI_Recv(&m, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD, &status);
                    sub.resize(m);
                    MPI_Recv(&sub[0], m, MPI_INT, rank + 1, 0, MPI_COMM_WORLD, &status);
                    blockMergeArrays(part_vector.data(), sub.data(), part_vector.size(), sub.size());
            }
            } else {
                if (rank > 0) {
                    MPI_Send(&elements_part_count, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD);
                    MPI_Send(&part_vector[0], elements_part_count, MPI_INT, rank - 1, 0, MPI_COMM_WORLD);
                    MPI_Recv(&m, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, &status);
                    sub.resize(m);
                    MPI_Recv(&sub[0], m, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, &status);
                    blockMergeArrays(sub.data(), part_vector.data(), sub.size(), part_vector.size());
                }
            }
        } else {
            if (rank % 2 == 0) {
                if (rank < procCount - 1) {
                    elements_part_count = part_vector.size();
                    MPI_Send(&elements_part_count, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);
                    MPI_Send(&part_vector[0], elements_part_count, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);
                    MPI_Recv(&m, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD, &status);
                    sub.resize(m);
                    MPI_Recv(&sub[0], m, MPI_INT, rank + 1, 0, MPI_COMM_WORLD, &status);
                    blockMergeArrays(part_vector.data(), sub.data(), part_vector.size(), sub.size());
                }
            } else {
                MPI_Send(&elements_part_count, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD);
                MPI_Send(&part_vector[0], elements_part_count, MPI_INT, rank - 1, 0, MPI_COMM_WORLD);
                MPI_Recv(&m, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, &status);
                sub.resize(m);
                MPI_Recv(&sub[0], m, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, &status);
                blockMergeArrays(sub.data(), part_vector.data(), sub.size(), part_vector.size());
        }
    }
    }
    MPI_Gatherv(&part_vector[0], revcount[rank], MPI_INT,
    arr, revcount, displs, MPI_INT, 0, MPI_COMM_WORLD);
} else {
    if (rank == 0) {
            for (int i = 0; i < arr_size; i++) {
                arr[i] = part_vector[i];
            }
    }
}
    if (rank == 0) {
        double endT = MPI_Wtime();
        std::cout << "MPI time: " << endT - startT << std::endl;
    }
}
