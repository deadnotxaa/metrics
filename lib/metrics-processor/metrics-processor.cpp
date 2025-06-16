#include "metrics-processor.hpp"
#include <sstream>
#include <iomanip>
#include <chrono>

metrics_task::MetricsProcessor::MetricsProcessor(const std::vector<AbstractMetric*>& metrics,
                                                MetricsWriterInterface* writer,
                                                const std::chrono::milliseconds processing_interval)
    : metrics_(metrics), writer_(writer), processing_interval_in_ms(processing_interval) {}

void metrics_task::MetricsProcessor::startProcessing() {
    std::lock_guard lock(mutex_);
    if (!running_) {
        start_time_ = std::chrono::system_clock::now();
        running_ = true;
        processing_thread_ = std::thread(&MetricsProcessor::loopMetricsProcessing, this);
    }
}

void metrics_task::MetricsProcessor::stopProcessing() {
    {
        std::lock_guard lock(mutex_);
        running_ = false;
    }
    cv_.notify_all();

    if (processing_thread_.joinable()) {
        processing_thread_.join();
    }

    writer_->stopWriting();
}

void metrics_task::MetricsProcessor::loopMetricsProcessing() {
    while (true) {
        {
            std::unique_lock lock(mutex_);
            if (!running_) {
                break;
            }
        }

        auto timestamp_copy = start_time_;

        auto futures_ptr = std::make_shared<std::vector<std::future<std::string>>>();
        futures_ptr->reserve(metrics_.size());

        for (const auto& metric : metrics_) {
            futures_ptr->emplace_back(metric->getCalculatedMetric());
        }

        pool.enqueue_detach([this, futures_ptr, timestamp_copy] {
            std::ostringstream resulting_string;

            auto time_t_timestamp = std::chrono::system_clock::to_time_t(timestamp_copy);
            auto tm_timestamp = std::localtime(&time_t_timestamp);

            auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
                timestamp_copy.time_since_epoch() % std::chrono::seconds(1)
            ).count();

            resulting_string << std::put_time(tm_timestamp, "%Y-%m-%d %H:%M:%S");
            resulting_string << '.' << std::setfill('0') << std::setw(3) << ms;

            for (auto& future : *futures_ptr) {
                resulting_string << future.get();
            }
            resulting_string << std::endl;

            if (writer_) {
                writer_->writeMetric(resulting_string.str());
            }
        });

        start_time_ += processing_interval_in_ms;

        std::unique_lock lock(mutex_);
        cv_.wait_until(lock, start_time_, [this] {
            return !running_;
        });

        if (!running_) {
            break;
        }
    }
}
