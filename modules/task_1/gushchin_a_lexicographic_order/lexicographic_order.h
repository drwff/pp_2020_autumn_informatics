// Copyright 2020 Gushchin Artem
#ifndef MODULES_TASK_1_GUSHCHIN_A_LEXICOGRAPHIC_ORDER_LEXICOGRAPHIC_ORDER_H_
#define MODULES_TASK_1_GUSHCHIN_A_LEXICOGRAPHIC_ORDER_LEXICOGRAPHIC_ORDER_H_

#include <string>

std::string getRandomString(size_t length);
int sequentialIsLexicographicOrder(const std::string& string1, const std::string& string2);
int parallelIsLexicographicOrder(const std::string& string1, const std::string& string2,
                                       size_t length1, size_t length2);

#endif  // MODULES_TASK_1_GUSHCHIN_A_LEXICOGRAPHIC_ORDER_LEXICOGRAPHIC_ORDER_H_
