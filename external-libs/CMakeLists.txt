set(EASTL_ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/eastl")

message(STATUS "Setting up EASTL")

set(EASTL_INCLUDE_DIRS
        "${EASTL_ROOT_DIR}/include"
        PARENT_SCOPE)

set(EASTL_LIBRARY debug ${EASTL_ROOT_DIR}/build/Debug/EASTL.lib optimized ${EASTL_ROOT_DIR}/build/Release/EASTL.lib PARENT_SCOPE)