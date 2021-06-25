set(BASIC_SOURCE_FILES
        Basic.cpp
        PerspectiveHelper.cpp PerspectiveHelper.hpp)

add_executable(BasicGL ${BASIC_SOURCE_FILES})
add_executable(BasicVK ${BASIC_SOURCE_FILES})

linkKryneEngine(BasicGL GL)
linkKryneEngine(BasicVK VK)

copyAssets(BasicGL)
copyAssets(BasicVK)