add_executable(BasicGL
        Basic.cpp PerspectiveHelper.cpp PerspectiveHelper.hpp)

target_link_libraries(BasicGL PRIVATE kryne-engine-gl)

copyAssets(BasicGL)