#include <thread>
#include <chrono>
#include <random>

#include "lib/metrics-processor/metrics-processor.hpp"
#include "lib/writer/file-writer/metrics-file-writer.hpp"
#include "lib/metric/metric.hpp"
#include "lib/metric-calculator/average/metric-average-calculator.hpp"

int main(int argc, char *argv[]) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution doubleDistribution(0.0, 1.0);
    std::uniform_int_distribution msDistribution(10, 200);

    metrics_task::AbstractMetric* metric =
        new metrics_task::Metric<double>("CPU Burst", new metrics_task::MetricAverageCalculator<double>());

    metrics_task::AbstractMetric* metric2 =
        new metrics_task::Metric<int>("Average HTTP Code", new metrics_task::MetricAverageCalculator<int>());

    metrics_task::MetricsProcessor processor(
        {metric, metric2},
        new metrics_task::MetricsFileWriter(),
        std::chrono::milliseconds(1000)
    );

    processor.startProcessing();

    std::chrono::system_clock::time_point start = std::chrono::system_clock::now();
    const std::chrono::system_clock::time_point end = std::chrono::system_clock::now() + std::chrono::seconds(50);

    while (start < end) {
        start = std::chrono::system_clock::now();

        metric->countMetric(doubleDistribution(gen));
        metric2->countMetric(static_cast<int>(doubleDistribution(gen) * 1000));
        std::this_thread::sleep_for(std::chrono::milliseconds(msDistribution(gen)));
    }

    processor.stopProcessing();

    return 0;
}
