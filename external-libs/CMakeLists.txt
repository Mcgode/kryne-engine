
# Set up boost library

set(BOOST_RUNTIME_LINK static)
add_subdirectory(boost)

set_target_properties(boost_filesystem PROPERTIES Type "External")
SetTargetFlags(boost_filesystem)

set(BoostLibs boost_variant boost_filesystem boost_format boost_lexical_cast PARENT_SCOPE)


#Set up EASTL
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


# Set up GLFW3
set(BUILD_SHARED_LIBS OFF)
set(USE_MSVC_RUNTIME_LIBRARY_DLL OFF)
add_subdirectory(glfw)

set_target_properties(glfw PROPERTIES Type "External")
SetTargetFlags(glfw)