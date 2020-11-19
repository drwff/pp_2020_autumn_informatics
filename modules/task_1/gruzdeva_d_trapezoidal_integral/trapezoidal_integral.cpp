// Copyright 2020 Gruzdeva Diana
#include "../../../modules/task_1/gruzdeva_d_trapezoidal_integral/trapezoidal_integral.h"

double getSequentialIntegral(double a, double b, int n, std::function<double(double)> function) {
    double width = (b - a) / n;
    double trapezoidal_integral = 0;
    for (int i = 0; i < n; i++) {
        double x1 = a + i * width;
        double x2 = a + (i + 1) * width;
        trapezoidal_integral += 0.5 * (x2 - x1) * (function(x1) + function(x2));
    }
    return trapezoidal_integral;
}

double getParallelIntegral(double a, double b, int n, std::function<double(double)> function) {
    int rank, size;
    double width = (b - a) / n;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int local_n = n / size;
    double result = 0,
           local_result = 0,
           local_a = a + rank * local_n * width,
           local_b = local_a + local_n * width;

    if (rank != size - 1) {
           local_result = getSequentialIntegral(local_a, local_b, local_n, function);
    } else {
           local_result = getSequentialIntegral(local_a, b, n - local_n * (size - 1), function);
    }

    MPI_Reduce(&local_result, &result, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    return result;
}

double callFunction(std::function<double(double)> function, double value) {
    return function(value);
}

double polinomFunction(double x) {
    return x * x * x + 2 * x * x + 4 * x + 6;
}

double compositeFunction(double x) {
    return exp(sin(x + 1));
}
