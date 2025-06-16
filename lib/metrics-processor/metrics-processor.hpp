#ifndef METRICS_REGISTER_HPP
#define METRICS_REGISTER_HPP

#include <vector>
#include <chrono>

#include "thread_pool/thread_pool.h"

#include "lib/metric/abstract-metric.hpp"
#include "lib/writer/metrics-writer-interface.hpp"

namespace metrics_task {

class MetricsProcessor {
public:
    MetricsProcessor(const std::vector<AbstractMetric*>& metrics,
                     MetricsWriterInterface* writer,
                     std::chrono::milliseconds processing_interval);

    void startProcessing();

    void stopProcessing();

private:
    void loopMetricsProcessing();

    std::vector<AbstractMetric*> metrics_;
    MetricsWriterInterface* writer_;

    std::chrono::milliseconds processing_interval_in_ms;
    std::chrono::time_point<std::chrono::system_clock> start_time_;

    std::thread processing_thread_;
    std::condition_variable cv_;

    dp::thread_pool<> pool;
};

} // metrics_task

#endif //METRICS_REGISTER_HPP
