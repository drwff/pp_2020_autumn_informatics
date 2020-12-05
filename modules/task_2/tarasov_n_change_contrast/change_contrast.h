// Copyright 2020 Tarasov Nikita
#ifndef MODULES_TASK_2_TARASOV_N_CHANGE_CONTRAST_CHANGE_CONTRAST_H_
#define MODULES_TASK_2_TARASOV_N_CHANGE_CONTRAST_CHANGE_CONTRAST_H_

#include <vector>

std::vector<int> set_random_img(int _width, int _high);
void print_img(std::vector<int> pic, int _high, int _width);
std::vector<int> changeContrast(const std::vector<int> &pic, int _width, int _high, int _correction);
std::vector<int> changeContrastParallel(const std::vector<int> &pic, int _width, int _high, int _correction);

#endif  // MODULES_TASK_2_TARASOV_N_CHANGE_CONTRAST_CHANGE_CONTRAST_H_
