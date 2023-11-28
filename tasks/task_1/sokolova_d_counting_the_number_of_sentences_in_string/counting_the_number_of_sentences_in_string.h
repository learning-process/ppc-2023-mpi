// Copyright 2023 Sokolova Daria
#ifndef TASKS_TASK_1_SOKOLOVA_D_COUNTING_THE_NUMBER_OF_SENTENCES_IN_STRING_COUNTING_THE_NUMBER_OF_SENTENCES_IN_STRING_H_
#define TASKS_TASK_1_SOKOLOVA_D_COUNTING_THE_NUMBER_OF_SENTENCES_IN_STRING_COUNTING_THE_NUMBER_OF_SENTENCES_IN_STRING_H_

#include <mpi.h>
#include <gtest/gtest.h>
#include <vector>
#include <string>
#include <random>
#include <boost/mpi/communicator.hpp>
#include <boost/mpi/collectives.hpp>


int countSentences(std::string line);
int parallelCountSentencesInString(const std::string& str);
std::string getRandomSentence();

#endif  // TASKS_TASK_1_SOKOLOVA_D_COUNTING_THE_NUMBER_OF_SENTENCES_IN_STRING_COUNTING_THE_NUMBER_OF_SENTENCES_IN_STRING_H_
