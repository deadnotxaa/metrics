add_library(abstract_metric INTERFACE abstract-metric.hpp)
add_library(metric INTERFACE metric.hpp)

target_link_libraries(metric
        INTERFACE unique_concurrentqueue
        INTERFACE metric-calculator-interface
)

target_include_directories(metric
        INTERFACE ${cunique_concurrentqueue_SOURCE_DIR}
)
