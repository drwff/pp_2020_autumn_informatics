// Copyright 2020 Loganov Andrei
#ifndef  MODULES_TASK_2_LOGANOV_A_SMOOTHING_SMOOTHING_H_
#define  MODULES_TASK_2_LOGANOV_A_SMOOTHING_SMOOTHING_H_
#include <vector>
void arrdel(int** arr, int rows);
int clamp(int v, int max, int min);
int calculateavg(int** im, int x, int y, int radius, int cols, int rows);
int** avg_point(int** im, int cols, int rows, int radius);
int** ParallelSmoothing(int** image, int cols, int rows, int radius);
#endif  //  MODULES_TASK_2_LOGANOV_A_SMOOTHING_SMOOTHING_H_

