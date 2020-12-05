// Copyright 2020 Belik Julia
#ifndef MODULES_TASK_3_BELIK_J_RADIX_SORT_BATCHER_RADIXSORTB_H_
#define MODULES_TASK_3_BELIK_J_RADIX_SORT_BATCHER_RADIXSORTB_H_

#include <vector>

std::vector<int> MergeBatcher(std::vector<int> vec, int n);
std::vector<int> Shuffle(std::vector<int> vec);
std::vector<int> EvenMerge(const std::vector<int>& v1, const std::vector<int>& v2);
std::vector<int> OddMerge(const std::vector<int>& v1, const std::vector<int>& v2);
std::vector<int> Merge(std::vector<int> vec, int evencount, int oddcount);
std::vector<int> Transpos(std::vector<int> vec, int evencount, int oddcount);
std::vector<int> RadixSort(std::vector<int> vec);
std::vector<int> Vector(int n);

#endif  // MODULES_TASK_3_BELIK_J_RADIX_SORT_BATCHER_RADIXSORTB_H_
