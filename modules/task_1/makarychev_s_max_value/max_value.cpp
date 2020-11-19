// Copyright 2020 Makarychev Sergey
#include "../../../modules/task_1/makarychev_s_max_value/max_value.h"
#include <mpi.h>
#include <iostream>
#include <random>
#include <algorithm>
#include <vector>
#include <ctime>


std::vector<int> getRandomMatrix(int rows, int cols) {
  int sizeM = rows * cols;
  if (sizeM < 0)
    throw "Wrong matrix size";
  std::mt19937 gen;
  gen.seed(static_cast<unsigned int>(time(0)));
  std::vector<int> randV(sizeM);
  for (int i = 0; i < sizeM; i++) {
    randV[i] = gen() % 100;
  }
  return randV;
}

int getMaxElemSequential(const std::vector<int>& locVec) {
  int maxElement = *std::max_element(locVec.begin(), locVec.end());
  return maxElement;
}

int getMaxElemParallel(const std::vector<int>& mat, int rows, int cols) {
  int sizeM = rows * cols;
  if (sizeM <= 0)
    throw "Wrong matrix size";
  int ProcNum, ProcRank;
  MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
  int delta = sizeM / ProcNum;
  int rem = sizeM % ProcNum;
  if (ProcRank == 0) {
    for (int proc = 1; proc < ProcNum; proc++) {
      MPI_Send(mat.data() + rem + proc * delta, delta, MPI_INT, proc, 0, MPI_COMM_WORLD);
  }
  }
  int locSize;
  if (ProcRank == 0) {
    locSize = delta + rem;
  } else {
    locSize = delta;
  }
  std::vector<int> locVec(locSize);
  if (ProcRank == 0) {
    locVec = std::vector<int>(mat.begin(), mat.begin() + locSize);
  } else {
    MPI_Status status;
    MPI_Recv(locVec.data(), locSize, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
  }
  int locMax = getMaxElemSequential(locVec);
  int globMax;
  MPI_Reduce(&locMax, &globMax, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);
  return globMax;
}
