add_executable(Basic
        Basic.cpp PerspectiveHelper.cpp PerspectiveHelper.hpp)

target_link_libraries(Basic PRIVATE kryne-engine)

copyAssets(Basic)