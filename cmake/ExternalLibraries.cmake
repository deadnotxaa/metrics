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

set(CMAKE_CXX_FLAGS "-fexperimental-library")
