#include "metrics-prometheus-writer.hpp"

namespace metrics_task {

MetricsPrometheusWriter::MetricsPrometheusWriter(const std::vector<std::string>& metric_names,
    const std::string& endpoint) : exposer_endpoint_(endpoint) {


    std::vector<std::string> prometheus_names;
    for (auto& metric_name : metric_names) {
        std::string prometheus_name = metric_name;

        std::ranges::replace(prometheus_name, ' ', '_');
        std::ranges::transform(prometheus_name, prometheus_name.begin(),
                               [](const unsigned char c){ return std::tolower(c);}
        );

        prometheus_names.push_back(prometheus_name);
    }

    metric_names_ = prometheus_names;

    for (size_t i = 0; i < metric_names.size(); ++i) {
        metric_names_low_map_[metric_names[i]] = prometheus_names[i];
    }

    exposer_ = std::make_unique<prometheus::Exposer>(endpoint);
    registry_ = std::make_shared<prometheus::Registry>();

    exposer_->RegisterCollectable(registry_);

    for (auto& i : metric_names_) {
        const auto family = &prometheus::BuildGauge()
            .Name(i)
            .Help(i + " Metric")
            .Register(*registry_);

        families[i] = family;
        gauges_[i] = &family->Add({});
    }

    running_ = true;
    processing_thread_ = std::thread(&MetricsPrometheusWriter::process, this);
}

void MetricsPrometheusWriter::writeMetric(std::string metric) {
    writer_queue_.try_enqueue(std::move(metric));
}

void MetricsPrometheusWriter::stopWriting() {
    running_ = false;
    if (processing_thread_.joinable()) {
        processing_thread_.join();
    }
}

MetricsPrometheusWriter::~MetricsPrometheusWriter() {
    stopWriting();
}

void MetricsPrometheusWriter::process() {
    const std::regex pattern("\"([^\"]+)\"\\s+([^\\s\"]+)");

    while (running_) {
        std::string data;

        if (!writer_queue_.try_dequeue(data)) {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            continue;
        }

        size_t firstQuotePos = data.find('"');

        if (firstQuotePos != std::string::npos && firstQuotePos > 0) {
            std::string metricsData = data.substr(firstQuotePos);

            auto searchStart(metricsData.cbegin());
            std::smatch matches;

            while (std::regex_search(searchStart, metricsData.cend(), matches, pattern)) {
                if (matches.size() >= 3) {
                    std::string metric_name = matches[1].str();
                    std::string metric_value = matches[2].str();
                    gauges_[metric_names_low_map_[metric_name]]->Set(std::stod(metric_value));
                    searchStart = matches.suffix().first;
                }
            }
        }
    }
}

} // metrics_task
