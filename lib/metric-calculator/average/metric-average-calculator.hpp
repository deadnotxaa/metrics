#ifndef METRIC_AVERAGE_CALCULATOR_HPP
#define METRIC_AVERAGE_CALCULATOR_HPP

#include "lib/metric-calculator/metric-calculator-interface.hpp"

namespace metrics_task {

template<typename T>
class MetricAverageCalculator final : public MetricCalculatorInterface {
    void calculate(const std::string& metric_name, std::vector<std::any> metric_values, std::promise<std::string> metric_promise) override {
        T accumulator = 0;
        for (auto& value : metric_values) {
            accumulator += std::any_cast<T>(value);
        }

        metric_promise.set_value(" \"" + metric_name + "\"" + " " +
            std::to_string(static_cast<double>(accumulator) / static_cast<double>(metric_values.size())));
    }
};

} // metrics_task

#endif //METRIC_AVERAGE_CALCULATOR_HPP
