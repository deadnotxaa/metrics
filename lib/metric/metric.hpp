#ifndef METRIC_HPP
#define METRIC_HPP

#include <string>
#include <any>
#include <chrono>

#include "concurrentqueue.h"

#include "abstract-metric.hpp"
#include "lib/metric-calculator/metric-calculator-interface.hpp"

namespace metrics_task {

template <typename T>
class Metric final : public AbstractMetric {
public:
    Metric(std::string name, MetricCalculatorInterface* metric_calculator)
        : metric_name_(std::move(name)), metric_calculator_(metric_calculator) {}

    void countMetric(const std::any& metric_info) override {
        metric_queue_.enqueue(std::any_cast<T>(metric_info));
    }

    std::future<std::string> getCalculatedMetric() override {
        std::promise<std::string> metric_calculated;
        std::future<std::string> metric_calculated_future = metric_calculated.get_future();

        size_t current_queue_size = metric_queue_.size_approx();
        std::vector<T> extracted_metrics(current_queue_size);

        size_t actual_dequeued = metric_queue_.try_dequeue_bulk(extracted_metrics.data(), current_queue_size);

        extracted_metrics.resize(actual_dequeued);

        std::vector<std::any> extracted_metrics_any;
        extracted_metrics_any.reserve(extracted_metrics.size());
        for (const auto& i : extracted_metrics) {
            extracted_metrics_any.emplace_back(i);
        }

        std::thread some_worker([
            mc = std::move(metric_calculated),
            metric_name = metric_name_,
            metric_values = extracted_metrics_any,
            calculator = metric_calculator_]() mutable
            {
                calculator->calculate(metric_name, metric_values, std::move(mc));
            });

        some_worker.detach();

        return metric_calculated_future;
    }

    std::any getLastQueueElement() override {
        T item;
        if (metric_queue_.try_dequeue(item)) {
            return std::any{item};
        }
        return std::any{};
    }

    ~Metric() override = default;

private:
    std::string metric_name_{};
    moodycamel::ConcurrentQueue<T> metric_queue_;
    MetricCalculatorInterface* metric_calculator_;
};

} // metrics_task
#endif //METRIC_HPP
