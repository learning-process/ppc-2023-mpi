// Copyright 2023 Nesterov Alexander

#ifndef MODULES_REFERENCE_SUM_OF_VECTOR_ELEMENTS_REF_TASK_HPP_
#define MODULES_REFERENCE_SUM_OF_VECTOR_ELEMENTS_REF_TASK_HPP_

#include <gtest/gtest.h>
#include <vector>
#include <memory>
#include <numeric>
#include "core/include/task.hpp"

namespace ppc {
namespace reference {

template<class InOutType>
class SumOfVectorElements : public ppc::core::Task {
 public:
    explicit SumOfVectorElements(std::shared_ptr<ppc::core::TaskData> taskData_) : Task(taskData_) {}
    bool pre_processing() override {
        internal_order_test();
        // Init vectors
        input_ = std::vector<InOutType>(taskData->inputs_count[0]);
        auto tmp_ptr = reinterpret_cast<InOutType*>(taskData->inputs[0]);
        for (int i = 0; i < taskData->inputs_count[0]; i++) {
            input_[i] = tmp_ptr[i];
        }
        // Init value for output
        sum = 0;
        return true;
    }

    bool validation() override {
        internal_order_test();
        // Check count elements of output
        if (taskData->outputs_count[0] == 1) {
            return true;
        } else {
            return false;
        }
    }

    bool run() override {
        internal_order_test();
        sum = std::accumulate(input_.begin(), input_.end(), 0);
        return true;
    }

    bool post_processing() override {
        internal_order_test();
        reinterpret_cast<InOutType*>(taskData->outputs[0])[0] = sum;
        return true;
    }

 private:
    std::vector<InOutType> input_;
    InOutType sum;
};

}  // namespace reference
}  // namespace ppc

#endif  // MODULES_REFERENCE_SUM_OF_VECTOR_ELEMENTS_REF_TASK_HPP_
