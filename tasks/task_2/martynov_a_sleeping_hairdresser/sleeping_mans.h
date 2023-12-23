// Copyright 2023 Martynov Alexandr
#ifndef TASKS_TASK_2_MARTYNOV_A_SLEEPING_HAIRDRESSER_SLEEPING_MANS_H_
#define TASKS_TASK_2_MARTYNOV_A_SLEEPING_HAIRDRESSER_SLEEPING_MANS_H_

void visitor(int myrank);
void queue(int _seats, int runs);
void bearded_man(int myrank, int _seats, int client_count);

#define CHAIR_REQUEST  1
#define CHAIR_RESPONSE  2
#define HAIR_CUT_REQUEST 3
#define HAIR_CUT 4
#endif  // TASKS_TASK_2_MARTYNOV_A_SLEEPING_HAIRDRESSER_SLEEPING_MANS_H_
