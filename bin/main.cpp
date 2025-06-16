#include <iostream>
#include <thread>
#include <chrono>
#include <random> // Added for random number generation

#include "lib/metrics-processor/metrics-processor.hpp"
#include "lib/writer/file-writer/metrics-file-writer.hpp"
#include "lib/metric/metric.hpp"
#include "lib/metric-calculator/average/metric-average-calculator.hpp"

int main(int argc, char *argv[]) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> doubleDistribution(0.0, 1.0);
    std::uniform_int_distribution<int> msDistribution(10, 200);

    metrics_task::AbstractMetric* metric =
        new metrics_task::Metric<double>("CPU Burst", new metrics_task::MetricAverageCalculator<double>());

    metrics_task::AbstractMetric* metric2 =
        new metrics_task::Metric<int>("Average HTTP Code", new metrics_task::MetricAverageCalculator<int>());

    metrics_task::MetricsProcessor processor(
        {metric, metric2},
        new metrics_task::MetricsFileWriter(),
        std::chrono::milliseconds(3000)
    );

    processor.startProcessing();

    std::chrono::system_clock::time_point start = std::chrono::system_clock::now();
    std::chrono::system_clock::time_point end = std::chrono::system_clock::now() + std::chrono::seconds(60);

    while (start < end) {
        start = std::chrono::system_clock::now();

        metric->countMetric(doubleDistribution(gen));
        std::this_thread::sleep_for(std::chrono::milliseconds(msDistribution(gen)));
    }

    processor.stopProcessing();

    return 0;
}
