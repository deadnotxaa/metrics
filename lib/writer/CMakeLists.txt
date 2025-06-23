add_library(metrics-writer-interface INTERFACE metrics-writer-interface.hpp)

add_subdirectory(prometheus-writer)
add_subdirectory(file-writer)
