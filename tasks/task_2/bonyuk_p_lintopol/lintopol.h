  // Copyright 2023 Bonyuk Petr
#ifndef TASKS_TASK_2_BONYUK_P_LINTOPOL_LINTOPOL_H_
#define TASKS_TASK_2_BONYUK_P_LINTOPOL_LINTOPOL_H_

bool is_in_path(int current, int src, int dest, bool router);
int next_node(int current, bool router);
int prev_node(int current, bool router);
void Sends_data_lin_acr_the_topol(void* data, int count, MPI_Datatype datatype, int src,
    int dest, int tag, MPI_Comm comm);

#endif  //  TASKS_TASK_2_BONYUK_P_LINTOPOL_LINTOPOL_H_
