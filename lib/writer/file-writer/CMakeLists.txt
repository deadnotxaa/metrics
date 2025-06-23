add_library(metrics-file-writer metrics-file-writer.hpp metrics-file-writer.cpp)

target_link_libraries(metrics-file-writer
        PUBLIC unique_concurrentqueue
        INTERFACE metrics-writer-interface
)

target_include_directories(metrics-file-writer
        PUBLIC ${unique_concurrentqueue_SOURCE_DIR}
        PUBLIC ${PROJECT_SOURCE_DIR}
)
