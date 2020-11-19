// Copyright 2020 Kuznetsov Nikita
#include <mpi.h>
#include <vector>
#include <ctime>
#include <random>
#include <utility>
#include "../../../modules/task_1/kuznetsov_vec_errors/vec_mpi.h"

std::vector<int> randV(int size) {
  std::mt19937 rand_vec;
  if (size <= 0)
    throw - 1;
  std::vector<int> tmp(size);
  rand_vec.seed(static_cast<unsigned int>(time(0)));
  for (int i = 0; i < static_cast<int>(tmp.size()); i++)
    tmp[i] = rand_vec() % 100;
  return tmp;
}

std::vector<int> vecNormalize(std::vector<int> a) {
  for (int i = 0; i < static_cast<int>(a.size()) - 1; i++) {
    for (int j = 0; j < static_cast<int>(a.size()) - i - 1; j++) {
      if (a[j] > a[j + 1]) {
        std::swap(a[j], a[j + 1]);
      }
    }
  }
  return a;
}

int parallelVector(const std::vector<int>& a, int n) {
  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  int res = n / size;
  int remain = n - res * size;
  if (rank == 0) {
    for (int i = 1; i < size; i++) {
      if (i < size)
        MPI_Send(&a[0] + i * res + remain - 1, res + 1, MPI_INT, i, 0, MPI_COMM_WORLD);
    }
  }
  std::vector<int> tmp;
  if (rank == 0) {
  tmp.resize(res + remain);
  tmp = { a.begin(), a.begin() + res + remain };
  } else {
  tmp.resize(res + 1);
  MPI_Status status;
  MPI_Recv(&tmp[0], res + 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
  }
  int count_sum = countError(tmp);
  int result = 0;
  MPI_Reduce(&count_sum, &result, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
  return result;
}

int countError(const std::vector<int>& a) {
  int count = 0;
  for (int i = 0; i < static_cast<int>(a.size()) - 1; i++) {
    if (a[i] > a[i + 1])
      count++;
  }
  return count;
}
