#ifndef METRICS_FILE_WRITER_HPP
#define METRICS_FILE_WRITER_HPP

#include <iostream>
#include <chrono>

#include "lib/writer/metrics-writer-interface.hpp"

namespace metrics_task {
    class MetricsFileWriter final : public MetricsWriterInterface {
    public:
        void writeMetric(const std::string& metric) override {
            std::cout << metric << std::endl;
        }

    private:

    };
} // metrics_task

#endif //METRICS_FILE_WRITER_HPP
