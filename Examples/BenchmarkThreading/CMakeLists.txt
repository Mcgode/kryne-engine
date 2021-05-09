

## Multithreading benchmark

add_executable(BenchmarkMultiThreading
        main.cpp
        WorkComponent.cpp WorkComponent.hpp)

target_link_libraries(BenchmarkMultiThreading PRIVATE kryne-engine-gl)


## Single-threading benchmark

add_executable(BenchmarkSingleThreading
        main.cpp
        WorkComponent.cpp WorkComponent.hpp)

target_link_libraries(BenchmarkSingleThreading PRIVATE kryne-engine-gl-st)


## Copy assets

copyAssets(BenchmarkMultiThreading)