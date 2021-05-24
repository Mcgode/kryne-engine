set(BASIC_SOURCE_FILES
        Basic.cpp
        PerspectiveHelper.cpp PerspectiveHelper.hpp)

add_executable(BasicGL ${BASIC_SOURCE_FILES})
add_executable(BasicVK ${BASIC_SOURCE_FILES})

target_link_libraries(BasicGL PRIVATE kryne-engine-gl)
target_link_libraries(BasicVK PRIVATE kryne-engine-vk)

copyAssets(BasicGL)
copyAssets(BasicVK)