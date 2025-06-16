add_library(metrics-processor metrics-processor.hpp metrics-processor.cpp)

target_link_libraries(metrics-processor
        PUBLIC metric
        PUBLIC metrics-writer-interface
        PUBLIC dp::thread-pool
)

target_include_directories(metrics-processor
        PUBLIC ${PROJECT_SOURCE_DIR}
)
