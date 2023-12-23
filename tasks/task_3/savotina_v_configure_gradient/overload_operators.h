// Copyright 2023 Savotina Valeria
#ifndef TASKS_TASK_3_SAVOTINA_V_CONFIGURE_GRADIENT_OVERLOAD_OPERATORS_H_
#define TASKS_TASK_3_SAVOTINA_V_CONFIGURE_GRADIENT_OVERLOAD_OPERATORS_H_

#include <vector>

template <typename T>
std::vector<T> operator+(const std::vector<T>& vecOne,
                         const std::vector<T>& vecTwo) {
  if (vecOne.size() != vecTwo.size()) return std::vector<T>();

  size_t size = vecOne.size();
  std::vector<T> resultVec(size);

  for (size_t i = 0; i < size; ++i) resultVec[i] = vecOne[i] + vecTwo[i];

  return resultVec;
}

template <typename T>
std::vector<T> operator-(const std::vector<T>& vecOne,
                         const std::vector<T>& vecTwo) {
  if (vecOne.size() != vecTwo.size()) return std::vector<T>();

  size_t size = vecOne.size();
  std::vector<T> resultVec(size);

  for (size_t i = 0; i < size; ++i) resultVec[i] = vecOne[i] - vecTwo[i];

  return resultVec;
}

template <typename T>
std::vector<T> operator*(const std::vector<T>& vec, const T& value) {
  std::vector<T> newVec;
  newVec.reserve(vec.size());

  for (const auto& elem : vec) newVec.emplace_back(value * elem);

  return newVec;
}

template <typename T>
std::vector<T> operator*(const T& value, const std::vector<T>& vec) {
  return vec * value;
}

#endif  // TASKS_TASK_3_SAVOTINA_V_CONFIGURE_GRADIENT_OVERLOAD_OPERATORS_H_
