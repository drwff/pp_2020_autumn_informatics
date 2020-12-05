// Copyright 2020 Volkova Anastasia
#ifndef MODULES_TASK_3_VOLKOVA_A_RECTANGLE_METHOD_RECTANGLE_METHOD_MPI_H_
#define MODULES_TASK_3_VOLKOVA_A_RECTANGLE_METHOD_RECTANGLE_METHOD_MPI_H_

double function1(double x, double y, double z);
double function2(double x, double y, double z);
double function3(double x, double y, double z);
double function4(double x, double y, double z);

double integralFunction(double (*f)(double, double, double),
                        double ax, double bx,
                        double ay, double by,
                        double az, double bz,
                        int n, int m, int k);
double calcParallel(double (*f)(double, double, double),
                    double X, double Y,
                    double az, double bz,
                    int k, double hz);
double getCalculatedIntegral(double (*f)(double, double, double),
                             double ax, double bx,
                             double ay, double by,
                             double az, double bz,
                             int n, int m, int k);
double oneDimensionalIntegral(double (*f)(double, double, double),
                              double X, double Y,
                              double az, double bz,
                              int k, double hz);

#endif  // MODULES_TASK_3_VOLKOVA_A_RECTANGLE_METHOD_RECTANGLE_METHOD_MPI_H_
