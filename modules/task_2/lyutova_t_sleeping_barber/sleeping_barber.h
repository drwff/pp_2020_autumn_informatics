// Copyright 2020 Lyutova Tanya
#ifndef MODULES_TASK_2_LYUTOVA_T_SLEEPING_BARBER_SLEEPING_BARBER_H_
#define MODULES_TASK_2_LYUTOVA_T_SLEEPING_BARBER_SLEEPING_BARBER_H_

void wait(double seconds);
void print(int id, const char* message, double number);
void print(int id, const char* message, int number);
void doBarber(int queue_size);
void doCustomer(int id);
void launch();
double getRandomTime();

#endif  // MODULES_TASK_2_LYUTOVA_T_SLEEPING_BARBER_SLEEPING_BARBER_H_
