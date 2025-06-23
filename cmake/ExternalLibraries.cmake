include(FetchContent)

# concurrentqueue
FetchContent_Declare(
        unique_concurrentqueue
        GIT_REPOSITORY https://github.com/cameron314/concurrentqueue.git
        GIT_TAG master
)
FetchContent_MakeAvailable(unique_concurrentqueue)

add_library(unique_concurrentqueue INTERFACE)
target_include_directories(unique_concurrentqueue
        INTERFACE ${unique_concurrentqueue_SOURCE_DIR}
)

# thread-pool
FetchContent_Declare(
        thread-pool
        GIT_REPOSITORY https://github.com/DeveloperPaul123/thread-pool.git
        GIT_TAG 0.7.0
)
FetchContent_MakeAvailable(thread-pool)

# prometheus-cpp
FetchContent_Declare(
        prometheus-cpp
        GIT_REPOSITORY https://github.com/jupp0r/prometheus-cpp.git
        GIT_TAG v1.2.4
)

# disable zlib and libcurl
set(ENABLE_COMPRESSION OFF CACHE BOOL "Disable compression")
set(ENABLE_PUSH OFF CACHE BOOL "Disable push metrics")
set(ENABLE_TESTING OFF CACHE BOOL "Disable tests to speed up build")

FetchContent_MakeAvailable(prometheus-cpp)

if (APPLE)
    set(CMAKE_CXX_FLAGS "-fexperimental-library")
endif ()
