// Copyright 2020 Schekotilova Julia
#include "../../modules/task_2/schekotilova_ju_seidel_method/seidel_method.h"
#include <vector>

std::vector<double> getRandomVector(int sz) {
  std::mt19937 gen;
  gen.seed(static_cast<unsigned int>(time(0)));
  std::vector<double> v(sz);
  for (int i = 1; i < sz; i++) v[i] = gen() % 100;
  return v;
}

double evklid_norma(const std::vector<double>& vec) {
  double nrm = 0;
  for (auto i = vec.cbegin(); i != vec.cend(); ++i) nrm += *i * *i;
  return sqrt(nrm);
}

bool CompareTwoVectors(const std::vector<double>& vec1,
  const std::vector<double>& vec2, const int& size, double eps) {
  bool answer = false;
  for (int i = 0; i < size; i++) {
    if (std::abs(vec1[i] - vec2[i]) < eps) {
      answer = true;
    } else {
      answer = false;
      break;
      }
  }
  return answer;
}

std::vector<double> getSequentialOperations(const std::vector<double>& matr_A,
  const std::vector<double>& matr_B, int num, double eps) {
  std::vector<double> x(num, 0);
  std::vector<double> prev(num);
  std::vector<double> tmp(num);
  double sum;
  for (int i = 0; i < num; ++i) {
    x[i] = 0;
  }
  do {
    for (int i = 0; i < num; ++i) prev[i] = x[i];
    for (int i = 0; i < num; ++i) {
      sum = 0;
      for (int j = 0; j < i; ++j) sum += matr_A[i * num + j] * x[j];
      for (int j = i + 1; j < num; ++j)
        sum += matr_A[i * num + j] * prev[j];
      x[i] = (matr_B[i] - sum) / matr_A[i * num + i];
    }
    for (int i = 0; i < num; ++i) {
      tmp[i] = x[i] - prev[i];
    }
  } while (!CompareTwoVectors(x, prev, num, eps));
  return x;
}

std::vector<double> getParallelOperations(const std::vector<double>& matr_A,
  const std::vector<double>& matr_B, int num, double eps) {
  int rank, size;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<double> x(num), prev(num);
  int rnk = rank + 1;
  double loc_sum, glob_sum;
  int delta = num / size;
  int l = delta * rank;
  int r = delta * (rnk);

  if (((rank == (size - 1)) && (delta != 0)) || ((rank == 0) && (delta == 0)))
    r = num;
  double local_evk_norm, evklid_norm;
  evklid_norm = 0;
  do {
    for (int i = 0; i < num; i++) prev[i] = x[i];
    for (int i = 0; i < num; i++) {
      loc_sum = 0.0;
      glob_sum = 0.0;

      for (int j = l; j < r; j++) {
        if ((j >= 0) && (j < i)) {
          loc_sum += (matr_A[i * num + j] * x[j]);
        } else if ((j >= i + 1) && (j < num)) {
          loc_sum += (matr_A[i * num + j] * prev[j]);
        }
      }
      MPI_Allreduce(&loc_sum, &glob_sum, 1, MPI_DOUBLE,
        MPI_SUM, MPI_COMM_WORLD);
      x[i] = (matr_B[i] - glob_sum) / matr_A[i * num + i];
    }
    local_evk_norm = 0;
    for (int i = l; i < r; i++) {
      local_evk_norm += std::pow((x[i] - prev[i]), 2);
    }
    MPI_Allreduce(&local_evk_norm, &evklid_norm, 1,
      MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
  } while (!CompareTwoVectors(x, prev, num, eps));
  return x;
}
