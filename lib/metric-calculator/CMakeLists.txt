add_library(metric-calculator-interface INTERFACE metric-calculator-interface.hpp)
add_library(metric-average-calculator INTERFACE average/metric-average-calculator.hpp)

target_link_libraries(metric-average-calculator
        INTERFACE unique_concurrentqueue
)

target_include_directories(metric-average-calculator
        INTERFACE ${cunique_concurrentqueue_SOURCE_DIR}
)
