add_executable(BenchmarkMultiThreading
        main.cpp
        WorkComponent.cpp WorkComponent.hpp)

target_link_libraries(BenchmarkMultiThreading PRIVATE kryne-engine)

copyAssets(BenchmarkMultiThreading)