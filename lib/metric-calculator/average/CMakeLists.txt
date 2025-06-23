add_library(metric-average-calculator metric-average-calculator.hpp metric-average-calculator.cpp)

target_link_libraries(metric-average-calculator
        INTERFACE metric-calculator-interface
        INTERFACE unique_concurrentqueue
)

target_include_directories(metric-average-calculator
        PUBLIC ${PROJECT_SOURCE_DIR}
        INTERFACE ${cunique_concurrentqueue_SOURCE_DIR}
)
