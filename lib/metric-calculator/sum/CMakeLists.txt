add_library(metric-sum-calculator metric-sum-calculator.hpp metric-sum-calculator.cpp)

target_link_libraries(metric-sum-calculator
        INTERFACE metric-calculator-interface
)

target_include_directories(metric-sum-calculator
        PUBLIC ${PROJECT_SOURCE_DIR}
)
