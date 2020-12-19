add_executable(Basic
        main.cpp)

target_link_libraries(Basic PRIVATE kryne-engine)

copyAssets(Basic)