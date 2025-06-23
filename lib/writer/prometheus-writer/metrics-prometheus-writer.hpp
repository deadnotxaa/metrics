#ifndef METRICS_PROMETHEUS_WRITER_HPP
#define METRICS_PROMETHEUS_WRITER_HPP

#include <atomic>
#include <thread>
#include <string>
#include <memory>
#include <regex>
#include <iostream>
#include <unordered_map>

#include <prometheus/exposer.h>
#include <prometheus/gauge.h>
#include <prometheus/registry.h>

#include "concurrentqueue.h"

#include "lib/writer/metrics-writer-interface.hpp"
#include "lib/metric/abstract-metric.hpp"

namespace metrics_task {

class MetricsPrometheusWriter final : public MetricsWriterInterface {
public:
    explicit MetricsPrometheusWriter(const std::vector<std::string>& metrics_names,
        const std::string& endpoint = "0.0.0.0:8080");

    void writeMetric(std::string metric) override;

    void stopWriting() override;

    ~MetricsPrometheusWriter() override;

private:
    void process();

    std::unique_ptr<prometheus::Exposer> exposer_;
    std::shared_ptr<prometheus::Registry> registry_;

    std::unordered_map<std::string, prometheus::Family<prometheus::Gauge>*> families;
    std::unordered_map<std::string, prometheus::Gauge*> gauges_;

    std::thread processing_thread_;
    std::atomic<bool> running_{false};
    std::string exposer_endpoint_;
    std::vector<std::string> metric_names_;

    moodycamel::ConcurrentQueue<std::string> writer_queue_;
};

} // metrics_task

#endif //METRICS_PROMETHEUS_WRITER_HPP
