// Copyright 2020 Sidorova Alexandra
#ifndef MODULES_TASK_3_SIDOROVA_A_RADIX_SORT_RADIX_SORT_H_
#define MODULES_TASK_3_SIDOROVA_A_RADIX_SORT_RADIX_SORT_H_

#include <vector>

std::vector<double> getRandomVector(const size_t size);

std::vector<double> radixSortParallel(const std::vector<double>& src);
std::vector<double> radixSortSequential(const std::vector<double>& src);
std::vector<double> radixSortPass(const std::vector<double>& src, const uint16_t offset);
std::vector<double> radixSortLastPass(const std::vector<double>& src, const uint16_t offset);

void batcherNetwork(const size_t processCount);
void buildBatcherNetwork(const std::vector<size_t>& processRanks);
void buildBatcherComparators(const std::vector<size_t>& leftVector, const std::vector<size_t>& rightVector);

#endif  // MODULES_TASK_3_SIDOROVA_A_RADIX_SORT_RADIX_SORT_H_
