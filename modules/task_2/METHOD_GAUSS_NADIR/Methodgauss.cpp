// Copyright 2020 Nadir mohammed
#include<mpi.h>
#include<iostream>
#include<iomanip>
#include<vector>
#include "../../../modules/task_2/METHOD_GAUSS_NADIR/Methodgauss.h"

double* methodGauss(std::vector<std::vector<double>> array, double* solution, int row) {
    double temp, s;
    for (int j = 0; j < row - 1; j++) {
        for (int i = j + 1; i < row; i++) {
            temp = array[i][j] / array[j][j];

            for (int k = 0; k < row + 1; k++)
                array[i][k] -= array[j][k] * temp;
        }
    }

    for (int i = row - 1; i >= 0; i--) {
        s = 0;
        for (int j = i + 1; j < row; j++)
            s += array[i][j] * solution[j];

        if (array[i][i] == 0) {
            solution[i] = 0;
        } else {
            solution[i] = (array[i][row] - s) / array[i][i];
        }
    }
    for (int i = 0; i < row; i++)
        std::cout << std::setw(8) << std::setprecision(4) << solution[i] << std::endl;
    return solution;
}
void methodGaussParallel(const double* array, double* solution, int row, int col) {
    int size, rank;
    double* array_temp = new double[row * col];
    for (int i = 0; i < row * col; i++) { array_temp[i] = array[i]; }
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int n_Row = row / size;
    int rest_Row = row % size;

    int stan_Row;

    if (rank < rest_Row) {
        stan_Row = n_Row + 1;
    } else {
        stan_Row = n_Row;
    }

    double* sub_arr = new double[col * stan_Row];
    int* n_El = new int[size];
    int* n_disp = new int[size];

    n_disp[0] = 0;
    for (int i = 0; i < size; i++) {
        if (i < rest_Row) {
            n_El[i] = (n_Row + 1) * col;
        } else {
            n_El[i] = n_Row * col;
        }
        if (i > 0) {
            n_disp[i] = n_disp[i - 1] + n_El[i - 1];
        }
    }
    MPI_Scatterv(array_temp, n_El, n_disp, MPI_DOUBLE, sub_arr, stan_Row * col, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    double* part_trans = new double[col];


    for (int i = 0; i < n_disp[rank] / col; i++) {
        int rac = 0, sum = 0;
        for (int j = 0; j < size; j++) {
            sum += n_El[j] / col;
            if (i < sum) { rac = j; break; }  rac++;
        }
        MPI_Bcast(part_trans, col, MPI_DOUBLE, rac, MPI_COMM_WORLD);
        for (int j = 0; j < n_El[rank] / col; j++) {
            double s = part_trans[i] / sub_arr[j * col + i];
            for (int z = i; z < col; z++) { sub_arr[j * col + z] = s * sub_arr[j * col + z] - part_trans[z]; }
        }
    }
    for (int i = 0; i < n_El[rank] / col; i++) {
        for (int j = 0; j < col; j++) { part_trans[j] = sub_arr[i * col + j]; }
        MPI_Bcast(part_trans, col, MPI_DOUBLE, rank, MPI_COMM_WORLD);
        for (int j = i + 1; j < n_El[rank] / col; j++) {
            double s = part_trans[n_disp[rank] / col + i] / sub_arr[j * col + i + n_disp[rank] / col];
            for (int z = i + n_disp[rank] / col; z < col; z++) {
                sub_arr[j * col + z] = s * sub_arr[j * col + z] - part_trans[z];
            }
        }
    }
    int rac = 0;
    for (int i = n_disp[rank] / col + n_El[rank] / col; i < row; i++) {
        int sum = 0;
        for (int j = 0; j < size; j++) {
            sum += n_El[j] / col;
            if (i < sum) { rac = j; break; }
            rac++;
        }
        MPI_Bcast(part_trans, col, MPI_DOUBLE, rac, MPI_COMM_WORLD);
    }
    MPI_Gatherv(sub_arr, stan_Row * col, MPI_DOUBLE, array_temp, n_El, n_disp, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    if (rank == 0) {
        for (int i = row - 1; i >= 0; i--) {
            double b = array_temp[i * col + col - 1];
            for (int j = row - 1; j >= i + 1; j--) {
                b -= array_temp[i * col + j] * solution[j];
            }
            solution[i] = b / array_temp[i * col + i];
        }
    }


    for (int i = 0; i < row; i++) {
        std::cout << std::setprecision(4) << std::setw(8) << std::fixed << solution[i] << std::endl;

        if ((i + 1) % col == 0) { std::cout << std::endl; }
    }
    std::cout << "\n --------------------------------------------------------------------------\n";

    delete[] array_temp;
    delete[] sub_arr;
    delete[] n_El;
    delete[] n_disp;
    delete[] part_trans;
}
