#ifndef METRICS_WRITER_INTERFACE_HPP
#define METRICS_WRITER_INTERFACE_HPP

#include <string>

namespace metrics_task {

class MetricsWriterInterface {
public:
    virtual void writeMetric(std::string metric) = 0;

    virtual void stopWriting() = 0;

    virtual ~MetricsWriterInterface() = default;
};

} // metrics_task

#endif //METRICS_WRITER_INTERFACE_HPP
