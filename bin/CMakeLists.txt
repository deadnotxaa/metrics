add_executable(${PROJECT_NAME} main.cpp)

target_include_directories(${PROJECT_NAME}
        PUBLIC ${PROJECT_SOURCE_DIR}
)

target_link_libraries(${PROJECT_NAME}
        PUBLIC metrics-writer-interface
        PUBLIC metrics-prometheus-writer
        PUBLIC metrics-processor
        PUBLIC metric
)
