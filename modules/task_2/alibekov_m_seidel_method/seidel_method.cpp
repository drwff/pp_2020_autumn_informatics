// Copyright 2020 Alibekov Murad
#include <mpi.h>
#include <vector>
#include <random>
#include <ctime>
#include <algorithm>
#include "../../../modules/task_2/alibekov_m_seidel_method/seidel_method.h"

std::vector<double> generate_A(int size) {
    std::mt19937 gen;
    gen.seed(static_cast<unsigned int>(time(0)));
    std::vector<double> A(size * size);
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            A[i * size + j] = (i == j) ?
                std::abs(static_cast<int>(gen()) % 100) + 100. * (size - 1) :
                static_cast<int>(gen()) % 100;
    return A;
}

std::vector<double> generate_b(int size) {
    std::mt19937 gen;
    gen.seed(static_cast<unsigned int>(time(0)) + 7);
    std::vector<double> b(size);
    for (int i = 0; i < size; i++)
        b[i] = static_cast<double>(static_cast<int>(gen()) % 100);
    return b;
}

double d(const std::vector<double>& x, const std::vector<double>& y) {
    double max_dist = 0;
    int size = x.size();
    for (int i = 0; i < size; i++)
        if (std::fabs(x[i] - y[i]) > max_dist) max_dist = std::fabs(x[i] - y[i]);
    return max_dist;
}

double parallel_dot_product(const std::vector<double>& x, const std::vector<double>& y) {
    int proc_count, proc_rank, n = x.size();
    if (x.empty()) return 0;
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Comm_size(MPI_COMM_WORLD, &proc_count);
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);

    const int delta = n / proc_count;
    const int remain = n % proc_count;
    double local_sum = 0;
    double global_sum = 0;

    if (proc_rank == 0) {
        for (int proc = 1; proc < proc_count; proc++) {
            MPI_Send(&x[0] + proc * delta + (remain > proc ? proc : remain),
                delta + (remain > proc ? 1 : 0), MPI_DOUBLE, proc, 0, MPI_COMM_WORLD);
            MPI_Send(&y[0] + proc * delta + (remain > proc ? proc : remain),
                delta + (remain > proc ? 1 : 0), MPI_DOUBLE, proc, 1, MPI_COMM_WORLD);
        }
    }

    std::vector<double> local_x(delta + (remain > proc_rank ? 1 : 0) + 1);
    std::vector<double> local_y(delta + (remain > proc_rank ? 1 : 0) + 1);
    if (proc_rank == 0) {
        local_x = std::vector<double>(x.begin(), x.begin() + delta + (remain > proc_rank ? 1 : 0));
        local_y = std::vector<double>(y.begin(), y.begin() + delta + (remain > proc_rank ? 1 : 0));
    } else {
        MPI_Status status;
        MPI_Recv(&local_x[0], delta + (remain > proc_rank ? 1 : 0), MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &status);
        MPI_Recv(&local_y[0], delta + (remain > proc_rank ? 1 : 0), MPI_DOUBLE, 0, 1, MPI_COMM_WORLD, &status);
    }

    for (int i = 0; i < delta + (remain > proc_rank ? 1 : 0); i++)
        local_sum += local_x[i] * local_y[i];

    MPI_Reduce(&local_sum, &global_sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    return global_sum;
}

std::vector<double> solving_SLAE_sequential(const std::vector<double>& A, const std::vector<double>& b) {
    int proc_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);
    int size = b.size();
    std::vector<double> x(size);
    int epoch = 0;
    double dist = epsilon;

    do {
        for (int i = 0; i < size; i++) {
            double sum = 0;
            for (int j = 0; j < i; j++)
                sum += A[i * size + j] * x[j];
            for (int j = i + 1; j < size; j++)
                sum += A[i * size + j] * x[j];
            x[i] = (b[i] - sum) / A[i * size + i];
        }
        epoch++;

        std::vector<double> Ax(size);
        for (int i = 0; i < size; i++)
            for (int j = 0; j < size; j++)
                Ax[i] += A[i * size + j] * x[j];

        dist = d(Ax, b);
    } while ((dist >= epsilon) && (epoch < max_count));
    return x;
}

std::vector<double> solving_SLAE_parallel(const std::vector<double>& A, const std::vector<double>& b) {
    int proc_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);
    int size = b.size();
    std::vector<double> x(size);
    int epoch = 0;
    double dist = epsilon;

    do {
        for (int i = 0; i < size; i++) {
            std::vector<double> A_i(A.begin() + (i * size), A.begin() + (i * size + size));
            double dot = parallel_dot_product(A_i, x);
            x[i] = (b[i] - (dot - x[i] * A[i * size + i])) / A[i * size + i];
            MPI_Bcast(&x[i], 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
        }
        epoch++;

        std::vector<double> Ax(size);
        for (int i = 0; i < size; i++)
            for (int j = 0; j < size; j++)
                Ax[i] += A[i * size + j] * x[j];

        dist = d(Ax, b);
        MPI_Bcast(&dist, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    } while ((dist >= epsilon) && (epoch < max_count));
    return x;
}
