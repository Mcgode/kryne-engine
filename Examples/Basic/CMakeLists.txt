add_executable(Basic
        Basic.cpp)

target_link_libraries(Basic PRIVATE kryne-engine)

copyAssets(Basic)