// Copyright 2023 Nesterov Alexander
#ifndef TASKS_TASK_1_YURIN_A_TRAPEZOIDAL_RULE_TRAPEZOIDAL_RULE_H_
#define TASKS_TASK_1_YURIN_A_TRAPEZOIDAL_RULE_TRAPEZOIDAL_RULE_H_

#include <functional>

double IntegrateWithTrapezoidRuleSequential(
        const std::function<double(const double argument)>& function,
        double leftBound,
        double rightBound,
        double step);

double IntegrateWithTrapezoidRuleParallel(
        const std::function<double(const double argument)>& function,
        double leftBound,
        double rightBound,
        double step);


#endif  // TASKS_TASK_1_YURIN_A_TRAPEZOIDAL_RULE_TRAPEZOIDAL_RULE_H_
