#ifndef ABSTRACT_METRIC_HPP
#define ABSTRACT_METRIC_HPP

#include <any>
#include <future>

namespace metrics_task {

class AbstractMetric {
public:
    // Method for adding metric to the queue for future processing
    virtual void countMetric(const std::any&) = 0;

    // Method for getting the calculated metric
    virtual std::future<std::string> getCalculatedMetric() = 0;

    // Method for getting the last element from the queue
    virtual std::any getLastQueueElement() = 0;

    virtual ~AbstractMetric() = default;
};

} // metrics_task

#endif //ABSTRACT_METRIC_HPP
