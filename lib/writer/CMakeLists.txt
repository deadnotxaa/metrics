add_library(metrics-writer-interface INTERFACE metrics-writer-interface.hpp)

add_library(metrics-file-writer INTERFACE file-writer/metrics-file-writer.hpp)
add_library(metrics-prometheus-writer INTERFACE prometheus-writer/metrics-prometheus-writer.hpp)
