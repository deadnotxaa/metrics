add_library(metrics-prometheus-writer metrics-prometheus-writer.hpp metrics-prometheus-writer.cpp)

target_link_libraries(metrics-prometheus-writer
        PUBLIC prometheus-cpp::core prometheus-cpp::pull
        PUBLIC unique_concurrentqueue
        INTERFACE metrics-writer-interface
)

target_include_directories(metrics-prometheus-writer
        PUBLIC ${unique_concurrentqueue_SOURCE_DIR}
        PUBLIC ${PROJECT_SOURCE_DIR}
        PUBLIC ${prometheus-cpp_SOURCE_DIR}/core/include
        PUBLIC ${prometheus-cpp_SOURCE_DIR}/pull/include
        PUBLIC ${prometheus-cpp_BINARY_DIR}/core/include
        PUBLIC ${prometheus-cpp_BINARY_DIR}/pull/include
)
