// Copyright 2020 Schekotilova Julia
#include "../../modules/task_1/schekotilova_ju_alter_sign/alter_sign.h"
#include <vector>

std::vector<int> getRandomVector(int sz) {
  std::mt19937 gen;
  gen.seed(static_cast<unsigned int>(time(0)));
  std::vector<int> v(sz);
  for (int i = 1; i < sz; i++) v[i] = gen() % 100;
  return v;
}

int  getSequentialOperations(std::vector<int> v, int size_v) {
  size_v = v.size();
  if (size_v == 0) return 0;
  int res = 0;
  for (int i = 1; i < size_v; i++)  {
    if (((v[i - 1] < 0) && (v[i] >= 0)) ||
        ((v[i - 1] >= 0) && (v[i] <0))) res++;
  }
  return res;
}

int getParallelOperations(std::vector<int> vec, int count_size_vec) {
  int size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  const int delta = count_size_vec / size;

  if (rank == 0) {
    for (int proc = 1; proc < size; proc++) {
      MPI_Send(vec.data() + proc * delta, delta, MPI_INT, proc, 0, MPI_COMM_WORLD);
    }
  }

  std::vector<int> local_v(delta);
  const int size_v = local_v.size();
  if (rank == 0)  {
    local_v = std::vector<int>(vec.begin(), vec.begin() + delta);
  } else {
    MPI_Status st;
    MPI_Recv(local_v.data(), delta, MPI_INT, 0, 0, MPI_COMM_WORLD, &st);
  }
  int global = 0;
  int local = getSequentialOperations(local_v, size_v);
  MPI_Reduce(&local, &global, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
  return global;
}
