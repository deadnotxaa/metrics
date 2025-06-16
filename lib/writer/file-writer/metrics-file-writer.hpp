#ifndef METRICS_FILE_WRITER_HPP
#define METRICS_FILE_WRITER_HPP

#include <iostream>
#include <fstream>
#include <atomic>

#include "concurrentqueue.h"

#include "lib/writer/metrics-writer-interface.hpp"

namespace metrics_task {

class MetricsFileWriter final : public MetricsWriterInterface {
public:
    explicit MetricsFileWriter(const std::string& file_path = "metrics.txt") { // TODO: use std::filesystem and add check for dir existence
        file_stream_.open(file_path, std::ios::out | std::ios::app);
        running_ = true;
        writing_thread_ = std::thread(&MetricsFileWriter::process, this);
    }

    void writeMetric(std::string metric) override {
        writer_queue_.try_enqueue(std::move(metric));
    }

    void stopWriting() override {
        running_ = false;
        if (writing_thread_.joinable()) {
            writing_thread_.join();
        }
    }

    ~MetricsFileWriter() override {
        stopWriting();
    }

private:
    void process() {
        while (running_) {
            std::string data;
            if (writer_queue_.try_dequeue(data)) {
                file_stream_ << data;
                file_stream_.flush();
            } else {
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
        }

        std::string data;
        while (writer_queue_.try_dequeue(data)) {
            file_stream_ << data;
        }
        file_stream_.flush();
    }

    std::thread writing_thread_;
    std::ofstream file_stream_{};
    std::atomic<bool> running_{false};

    moodycamel::ConcurrentQueue<std::string> writer_queue_;
};

} // metrics_task

#endif //METRICS_FILE_WRITER_HPP
