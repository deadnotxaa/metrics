#ifndef METRICS_PROMETHEUS_WRITER_HPP
#define METRICS_PROMETHEUS_WRITER_HPP

#include "lib/writer/metrics-writer-interface.hpp"

namespace metrics_task {
class MetricsPrometheusWriter final : public MetricsWriterInterface {
public:
    void writeMetric(std::string metric) override;

    void stopWriting() override;

private:

};
} // metrics_task

#endif //METRICS_PROMETHEUS_WRITER_HPP
