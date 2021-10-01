set(EASTL_ROOT_DIR "${PROJECT_SOURCE_DIR}/external-libs/eastl")

message(STATUS "Setting up EASTL")

set(EASTL_INCLUDE_DIRS
        "${EASTL_ROOT_DIR}/include"
        "${EASTL_ROOT_DIR}/test/packages/EAAssert/include"
        "${EASTL_ROOT_DIR}/test/packages/EABase/include/Common"
        "${EASTL_ROOT_DIR}/test/packages/EAMain/include"
        "${EASTL_ROOT_DIR}/test/packages/EAStdC/include"
        "${EASTL_ROOT_DIR}/test/packages/EATest/include"
        "${EASTL_ROOT_DIR}/test/packages/EAThread/include"
        PARENT_SCOPE)

add_subdirectory(./eastl)
set_property(TARGET EASTL PROPERTY Type "External")
SetTargetFlags(EASTL)