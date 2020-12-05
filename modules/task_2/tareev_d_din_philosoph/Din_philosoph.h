// Copyright 2020 Tareev Daniil
#ifndef MODULES_TASK_2_TAREEV_D_DIN_PHILOSOPH_DIN_PHILOSOPH_H_
#define MODULES_TASK_2_TAREEV_D_DIN_PHILOSOPH_DIN_PHILOSOPH_H_
#include <list>

void diningPhilosofers(int myRank, int ProcSize, int time);
void table(bool* dinner, bool* fork, int ProcSize, std::list<int> ph_queue);
void philosofers(int time);
bool check_hunger(int ProcSize, bool* dinner, int out_buffer);

#endif  // MODULES_TASK_2_TAREEV_D_DIN_PHILOSOPH_DIN_PHILOSOPH_H_
