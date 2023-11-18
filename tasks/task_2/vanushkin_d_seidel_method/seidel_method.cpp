// Copyright 2023 Vanushkin Dmitry
#include "task_2/vanushkin_d_seidel_method/seidel_method.h"


DoubleVector SequentialSeidelMethod(const DoubleMatrix &alpha, const DoubleVector &beta, double eps) {
    size_t solutionsCount = beta.size();

    DoubleVector stepAnswer(solutionsCount, 0);
    DoubleVector nextStepAnswer = stepAnswer;

    do {

        stepAnswer = nextStepAnswer;

        for (size_t x = 0; x < solutionsCount; ++x) {

            double currentAnswer = 0;
            for (size_t i = 0; i < solutionsCount; i++) {
                currentAnswer += alpha[x][i] * nextStepAnswer[i];
            }

            currentAnswer += beta[x];

            nextStepAnswer[x] = currentAnswer;
        }

    } while (norm(stepAnswer, nextStepAnswer) > eps);


    return nextStepAnswer;
}

double norm(const DoubleVector &v1, const DoubleVector &v2) {
    double result = 0;

    for (size_t i = 0; i < v1.size(); i++) {
        result += pow(v2[i] - v1[i], 2);
    }

    return sqrt(result);
}

DoubleVector ParallelSeidelMethod(const DoubleMatrix &alpha, const DoubleVector &beta, double eps) {
    return DoubleVector();
}
