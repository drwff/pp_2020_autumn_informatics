// Copyright 2020 Zaitsev Andrey
#ifndef MODULES_TASK_1_ZAITSEV_A_SCALAR_PRODUCT_SCALAR_PRODUCT_H_
#define MODULES_TASK_1_ZAITSEV_A_SCALAR_PRODUCT_SCALAR_PRODUCT_H_

#include <vector>

std::vector<int> createRandomVector(int size);
int scalar_product_of_part_vectors(const std::vector<int>& vec1, const std::vector<int>& vec2);
int scalar_product_parallel(const std::vector<int>& vec1, const std::vector<int>& vec2, int size);

#endif  // MODULES_TASK_1_ZAITSEV_A_SCALAR_PRODUCT_SCALAR_PRODUCT_H_
