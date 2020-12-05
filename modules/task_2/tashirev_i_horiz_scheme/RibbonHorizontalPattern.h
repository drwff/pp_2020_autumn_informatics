// Copyright 2020 Tashirev Ivan
#ifndef MODULES_TASK_2_TASHIREV_I_HORIZ_SCHEME_RIBBONHORIZONTALPATTERN_H_
#define MODULES_TASK_2_TASHIREV_I_HORIZ_SCHEME_RIBBONHORIZONTALPATTERN_H_

#include <vector>

std::vector<int> getVector(int size);
std::vector<int> getMatrix(int line, int column);
std::vector<int> multMatrixSeq(std::vector<int> matr, int lines, int columns, std::vector<int> vec, int vecSize);
std::vector<int> multMatrixParallel(std::vector<int> matr, int lines, int columns, std::vector<int> vec, int vecSize);
#endif  // MODULES_TASK_2_TASHIREV_I_HORIZ_SCHEME_RIBBONHORIZONTALPATTERN_H_
