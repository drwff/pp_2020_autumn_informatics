// Copyright 2020 Makarov Alexander
#ifndef MODULES_TASK_1_MAKAROV_A_STRING_COMPARE_STRING_COMPARE_H_
#define MODULES_TASK_1_MAKAROV_A_STRING_COMPARE_STRING_COMPARE_H_

#include <string>

std::string getRandomString(int sz);
int compareParallel(const std::string& global_str1,
                    const std::string& global_str2,
                    const size_t str_size1, const size_t str_size2);
int compareSequential(const std::string& global_str1,
                      const std::string& global_str2);

#endif  // MODULES_TASK_1_MAKAROV_A_STRING_COMPARE_STRING_COMPARE_H_
