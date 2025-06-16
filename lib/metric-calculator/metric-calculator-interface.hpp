#ifndef METRIC_CALCULATOR_INTERFACE_HPP
#define METRIC_CALCULATOR_INTERFACE_HPP

#include <vector>
#include <any>

namespace metrics_task {

class MetricCalculatorInterface {
public:
    virtual void calculate(const std::string&, std::vector<std::any>, std::promise<std::string>) = 0;

    virtual ~MetricCalculatorInterface() = default;
};

} // metrics_task

#endif //METRIC_CALCULATOR_INTERFACE_HPP
