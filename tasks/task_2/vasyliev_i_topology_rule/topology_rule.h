// Copyright 2023 Vasyliev Ivan
#ifndef TASKS_TASK_2_VASYLIEV_I_TOPOLOGY_RULE_TOPOLOGY_RULE_H_
#define TASKS_TASK_2_VASYLIEV_I_TOPOLOGY_RULE_TOPOLOGY_RULE_H_

void RULE_SEND(const boost::mpi::communicator& world, int val, int dst, int tag);
void RULE_RECV(const boost::mpi::communicator& world, int* val, int src, int tag);
void RULE_HELP(const boost::mpi::communicator& world, int src, int dst, int tag);

#endif  // TASKS_TASK_2_VASYLIEV_I_TOPOLOGY_RULE_TOPOLOGY_RULE_H_
