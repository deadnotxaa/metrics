#include <thread>
#include <chrono>
#include <random>

#include <lib/metric-calculator/sum/metric-sum-calculator.hpp>
#include "lib/metrics-processor/metrics-processor.hpp"
#include "lib/writer/file-writer/metrics-file-writer.hpp"
#include "lib/metric/metric.hpp"
#include "lib/metric-calculator/average/metric-average-calculator.hpp"
#include "lib/writer/prometheus-writer/metrics-prometheus-writer.hpp"

int main([[maybe_unused]] int argc, [[maybe_unused]] char *argv[]) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution doubleDistribution(0.0, 2.0);
    std::uniform_int_distribution msDistribution(1, 50);

    // Create Metrics (also it supports not only double and int, but also any other types)
    metrics_task::AbstractMetric* metric =
        new metrics_task::Metric<double>("CPU Burst", new metrics_task::MetricAverageCalculator<double>());

    metrics_task::AbstractMetric* metric2 =
        new metrics_task::Metric<int>("HTTP Requests RPS", new metrics_task::MetricSumCalculator<int>());

    // Create Processor (he will process all metrics with specified interval and then send them to the specific writer)
    metrics_task::MetricsProcessor processor(
        {metric, metric2},
        new metrics_task::MetricsFileWriter(),
        std::chrono::milliseconds(1000)
    );

    // Start our processing. It will process metrics every second
    processor.startProcessing();

    std::chrono::system_clock::time_point start = std::chrono::system_clock::now();
    const std::chrono::system_clock::time_point end = std::chrono::system_clock::now() + std::chrono::seconds(600);

    while (start < end) {
        start = std::chrono::system_clock::now();

        metric->countMetric(doubleDistribution(gen));
        metric2->countMetric(1);
        std::this_thread::sleep_for(std::chrono::milliseconds(msDistribution(gen)));
    }

    // At the end we stop processing and writing metrics
    processor.stopProcessing();

    return 0;
}
