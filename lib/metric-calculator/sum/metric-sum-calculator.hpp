#ifndef METRIC_SUM_CALCULATOR_HPP
#define METRIC_SUM_CALCULATOR_HPP

#include <future>
#include <string>
#include <any>
#include <vector>

#include "lib/metric-calculator/metric-calculator-interface.hpp"

namespace metrics_task {

template<typename T>
class MetricSumCalculator final : public MetricCalculatorInterface {
public:
    void calculate(const std::string& metric_name, std::vector<std::any> metric_values, std::promise<std::string> metric_promise) override {
        T accumulator = 0;
        for (auto& value : metric_values) {
            accumulator += std::any_cast<T>(value);
        }

        metric_promise.set_value(" \"" + metric_name + "\"" + " " + std::to_string(accumulator));
    }

};

} // metrics_task


#endif //METRIC_SUM_CALCULATOR_HPP
