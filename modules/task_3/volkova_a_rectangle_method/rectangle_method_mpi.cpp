// Copyright 2020 Volkova Anastasia
#include <mpi.h>
#include <iostream>
#include "../../../modules/task_3/volkova_a_rectangle_method/rectangle_method_mpi.h"

double function1(double x, double y, double z) {
    return x*y*z;
}
double function2(double x, double y, double z) {
    return x * x * x * y + 2 * z * z * y + 10 * x * y * z;
}
double function3(double x, double y, double z) {
    return (5 * x + 2 * y) * z;
}
double function4(double x, double y, double z) {
    return x * x + x * y * z - 10*z * z;
}

double integralFunction(double (*f)(double, double, double),
                        double ax, double bx,
                        double ay, double by,
                        double az, double bz,
                        int n, int m, int k) {
    const double hx = (bx - ax) / static_cast<double>(n);
    const double hy = (by - ay) / static_cast<double>(m);
    const double hz = (bz - az) / static_cast<double>(k);

    double ans = 0;
    double X, Y, Z;
    for (double i = 0; i < n; ++i) {
        for (double j = 0; j < m; ++j) {
            for (double q = 0; q < k;  ++q) {
                X = static_cast<double>(ax) + i * hx + 0.5 * hx;
                Y = static_cast<double>(ay) + j * hy + 0.5 * hy;
                Z = static_cast<double>(az) + q * hz + 0.5 * hz;
                ans += f(X, Y, Z);
            }
        }
    }
    ans *= hx * hy * hz;
    return ans;
}

double oneDimensionalIntegral(double (*f)(double, double, double),
                              double X, double Y,
                              double az, double bz,
                              int k, double hz) {
    double tmp = 0, Z;
    for (double q = 0; q < k; ++q) {
        Z = static_cast<double>(az) + q * hz + 0.5 * hz;
        tmp += f(X, Y, Z);
    }
    return tmp;
}

double calcParallel( double (*f)(double, double, double),
                     double X, double Y,
                     double az, double bz,
                     int k, double hz) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    double ans = 0;
    double local_integral = 0;
    int local_size = k / size;
    double curr_az = az + rank * local_size * hz;
    double curr_bz = curr_az + local_size * hz;
    if (rank == size - 1) {
        int K = k - local_size * (size - 1);
        local_integral = oneDimensionalIntegral(f, X, Y, curr_az, bz, K, hz);
    } else {
        local_integral = oneDimensionalIntegral(f, X, Y, curr_az, curr_bz, local_size, hz);
    }
    MPI_Reduce(&local_integral, &ans, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    return ans;
}

double getCalculatedIntegral( double (*f)(double, double, double),
                              double ax, double bx,
                              double ay, double by,
                              double az, double bz,
                              int n, int m, int k) {
    const double hx = (bx - ax) / static_cast<double>(n);
    const double hy = (by - ay) / static_cast<double>(m);
    const double hz = (bz - az) / static_cast<double>(k);
    double ans = 0;
    double X, Y;
    for (double i = 0; i < n; ++i) {
        for (double j = 0; j < m; ++j) {
            X = static_cast<double>(ax) + i * hx + 0.5 * hx;
            Y = static_cast<double>(ay) + j * hy + 0.5 * hy;
            ans += calcParallel(f, X, Y, az, bz, k, hz);
        }
    }
    ans *= hx * hy * hz;
    return ans;
}
