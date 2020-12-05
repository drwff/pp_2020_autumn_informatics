// Copyright 2020 Makarov Alexander
#include <mpi.h>
#include <vector>
#include <random>
#include "../../../modules/task_2/makarov_a_jacobi_method/jacobi_method.h"

std::vector<double> generate_A(int size) {
    // diagonally dominant matrix generator
    if (size <=0) return std::vector<double>();
    std::random_device rd;
    std::mt19937 gen(rd());
    std::vector<double> A(size * size);
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            if (i != j)
                A[i * size + j] = static_cast<double>(
                                      static_cast<int>(gen() % 1000) - 500);
    for (int i = 0; i < size; i++) {
        double abs_sum = 0;
        for (int j = 0; j < size; j++)
            if (i != j)
                abs_sum += std::fabs(A[i * size + j]);
        A[i * size + i] = abs_sum + static_cast<double>(gen() % 500);
    }
    return A;
}

std::vector<double> generate_b(int size) {
    if (size <=0) return std::vector<double>();
    std::random_device rd;
    std::mt19937 gen(rd());
    std::vector<double> b(size);
    for (int i = 0; i < size; i++)
        b[i] = static_cast<double>(
                              static_cast<int>(gen() % 1000) - 500);
    return b;
}

double getMaxDistance(const std::vector<double>& v1,
                      const std::vector<double>& v2) {
    double max_dist = 0.;
    for (unsigned int i = 0; i < v1.size(); i++) {
        double dist = std::fabs(v1[i] - v2[i]);
        if (dist > max_dist)
            max_dist = dist;
    }
    return max_dist;
}

double calculateError(const std::vector<double>& A,
                      const std::vector<double>& x,
                      const std::vector<double>& b) {
    int size = static_cast<int>(x.size());
    std::vector<double> Ax(size, 0.);
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            Ax[i] += A[i * size + j] * x[j];
    double error = getMaxDistance(Ax, b);
    return error;
}

std::vector<double> solveJacobiSequential(const std::vector<double>& A,
                                          const std::vector<double>& b) {
    int iter = 0;
    int size = static_cast<int>(b.size());
    std::vector<double> x(size);
    std::vector<double> x_prev(b);
    double error = 0.;
    do {
        for (int i = 0; i < size; i++) {
            x[i] = b[i];
            for (int j = 0; j < size; j++)
                if (i != j)
                    x[i] -= A[i * size + j] * x_prev[j];
            x[i] /= A[i * size + i];
        }
        for (int i = 0; i < size; i++)
            x_prev[i] = x[i];
        iter++;
        error = calculateError(A, x, b);
    } while (error > epsilon && iter < max_iter);
    return x;
}

std::vector<double> solveJacobiParallel(const std::vector<double>& A,
                                          const std::vector<double>& b) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int proc_count;
    MPI_Comm_size(MPI_COMM_WORLD, &proc_count);

    int size;
    if (rank == 0)
        size = static_cast<int>(b.size());
    MPI_Bcast(&size, 1, MPI_INT, 0, MPI_COMM_WORLD);
    // создание коммуникатора без лишних процессов
    MPI_Comm comm;
    if (rank < size) {
        MPI_Comm_split(MPI_COMM_WORLD, 0, rank, &comm);
    } else {
        MPI_Comm_split(MPI_COMM_WORLD, MPI_UNDEFINED, rank, &comm);
    }

    if (comm != MPI_COMM_NULL) {
        int new_rank;
        MPI_Comm_rank(comm, &new_rank);
        int new_proc_count;
        MPI_Comm_size(comm, &new_proc_count);

        // распределение количества строк по процессам
        int* counts_b = nullptr;
        int* displs_b = nullptr;
        int* counts_A = nullptr;
        int* displs_A = nullptr;
        if (new_rank == 0) {
            int delta = size / new_proc_count;
            int remaind = size % new_proc_count;
            counts_b = new int[new_proc_count];
            displs_b = new int[new_proc_count];
            counts_A = new int[new_proc_count];
            displs_A = new int[new_proc_count];
            for (int i = 0; i < new_proc_count; i++) {
                counts_b[i] = delta;
                displs_b[i] = 0;
            }
            for (int i = 0; i < remaind; i++)
                counts_b[i]++;
            for (int i = 1; i < new_proc_count; i++)
                displs_b[i] += displs_b[i - 1] + counts_b[i - 1];
            for (int i = 0; i < new_proc_count; i++) {
                counts_A[i] = counts_b[i] * size;
                displs_A[i] = displs_b[i] * size;
            }
        }

        double error = 0.;
        int iter = 0;
        // узнаем сколько получать строк
        int local_size;
        MPI_Scatter(counts_b, 1, MPI_INT, &local_size, 1, MPI_INT, 0, comm);

        std::vector<double> x_global;
        std::vector<double> x_local(local_size);
        std::vector<double> x_prev(size);
        if (rank == 0) {
            x_global.resize(size);
            x_prev = b;
        }
        // передать/получить элементы b
        std::vector<double> b_local(local_size);
        MPI_Scatterv(b.data(), counts_b, displs_b, MPI_DOUBLE, b_local.data(),
                     local_size, MPI_DOUBLE, 0, comm);
        // передать/получить строки А
        std::vector<double> A_local(local_size * size);
        MPI_Scatterv(A.data(), counts_A, displs_A, MPI_DOUBLE, A_local.data(),
                     local_size * size, MPI_DOUBLE, 0, comm);
        // получить номер первой строки (для диагональных элементов)
        int first_row_num;
        MPI_Scatter(displs_b, 1, MPI_INT, &first_row_num, 1, MPI_INT, 0, comm);

        do {
            // передать/получить результат предыдущей итерации
            MPI_Bcast(x_prev.data(), size, MPI_DOUBLE, 0, comm);
            // обработать свои куски
            for (int i = 0; i < local_size; i++) {
                x_local[i] = b_local[i];
                for (int j = 0; j < size; j++)
                    if (j != first_row_num + i)
                        x_local[i] -= A_local[i * size + j] * x_prev[j];
                x_local[i] /= A_local[i * size + first_row_num + i];
            }
            iter++;

            // собрать результаты этой итерации
            MPI_Gatherv(x_local.data(), local_size, MPI_DOUBLE,
                        x_global.data(), counts_b, displs_b,
                        MPI_DOUBLE, 0, comm);
            if (rank == 0) {
                for (int i = 0; i < size; i++)
                    x_prev[i] = x_global[i];
                error = calculateError(A, x_global, b);
            }
            // передать/получить ошибку
            MPI_Bcast(&error, 1, MPI_DOUBLE, 0, comm);
        } while (error > epsilon && iter < max_iter);
        // принять результат от всех процессоров
        MPI_Comm_free(&comm);
        if (rank == 0) {
            delete[] counts_b;
            delete[] displs_b;
            delete[] counts_A;
            delete[] displs_A;
        }
        return x_global;
    } else {
        return std::vector<double>();
    }
}
