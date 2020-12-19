cmake_minimum_required(VERSION 3.17)
project(kryne_engine)

set(CMAKE_CXX_STANDARD 17)

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/CMake")

if (UNIX)
    set(CMAKE_CXX_STANDARD_LIBRARIES "${CMAKE_CXX_STANDARD_LIBRARIES} -ldl")
endif()


# Set up GLFW3
find_package(GLFW3 REQUIRED)
message(STATUS "Found GLFW3 in ${GLFW3_INCLUDE_DIR}")
include_directories(${GLFW3_INCLUDE_DIR})


# Set up GLM
set(GLM_TEST_ENABLE OFF)
add_subdirectory(external-libs/glm-0.9.9.5)
include_directories(external-libs/glm-0.9.9.5)


# Set up assimp
find_package(ASSIMP REQUIRED)
message(STATUS "Found assimp in ${ASSIMP_INCLUDE_DIR}")
include_directories(${ASSIMP_INCLUDE_DIR})


#Adding GLAD library
include_directories(external-libs/glad/include)
add_library(glad STATIC external-libs/glad/src/glad.c)


#Set up the used libs
set(LIBS glad ${GLFW3_LIBRARY} ${ASSIMP_LIBRARIES} glm)


# Set up boost library

if(WIN32)
    # Required, or else CMake won't find boost components
    set (Boost_USE_STATIC_LIBS 1)
endif()

find_package(Boost REQUIRED COMPONENTS filesystem system)
include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIR})
message(STATUS "Boost include dirs: ${Boost_INCLUDE_DIRS}")

set(LIBS ${LIBS} ${Boost_LIBRARIES})


# Set up Kryne Engine library
add_subdirectory(kryne-engine)


function(COPYASSETDIR TARGET ASSET)

    if(${UNIX})
        set(SYMLINK_COMMAND ln -s  ${CMAKE_SOURCE_DIR}/${ASSET}/ ${CMAKE_CURRENT_BINARY_DIR}/${ASSET})
    elseif(${WIN32})
        set(SYMLINK_COMMAND mklink /d ${CMAKE_CURRENT_BINARY_DIR}/${ASSET} ${CMAKE_SOURCE_DIR}/${ASSET}/ & exit 0)
    endif()

    add_custom_command(
            TARGET ${TARGET} POST_BUILD
            COMMAND ${SYMLINK_COMMAND})
endfunction()

function(COPYASSETS TARGET)
    copyAssetDir(${TARGET} "Resources")
endfunction()


# Setting up executables

add_subdirectory(Examples/SunLight)

add_subdirectory(Examples/Sphere)

add_subdirectory(Examples/Basic)