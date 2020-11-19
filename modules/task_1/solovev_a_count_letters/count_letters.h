// Copyright 2020 Alexandr Solovev
#ifndef MODULES_TASK_1_SOLOVEV_A_COUNT_LETTERS_COUNT_LETTERS_H_
#define MODULES_TASK_1_SOLOVEV_A_COUNT_LETTERS_COUNT_LETTERS_H_

#include <string>
#include <algorithm>

std::string CreateRandomString(int elems_count);
int calculateCountSequental(std::string a);
int calculateCountParallel(const std::string a, int elements_count);

#endif  // MODULES_TASK_1_SOLOVEV_A_COUNT_LETTERS_COUNT_LETTERS_H_
