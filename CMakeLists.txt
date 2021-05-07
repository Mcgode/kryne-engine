cmake_minimum_required(VERSION 3.16)
project(kryne_engine)

set(CMAKE_CXX_STANDARD 17)

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/CMake")

if (UNIX)
    set(CMAKE_CXX_STANDARD_LIBRARIES "${CMAKE_CXX_STANDARD_LIBRARIES} -ldl -lpthread")
endif()


# Set up GLFW3
find_package(GLFW3 REQUIRED)
message(STATUS "Found GLFW3 in ${GLFW3_INCLUDE_DIR}")
include_directories(${GLFW3_INCLUDE_DIR})


# Set up GLM
find_package(GLM REQUIRED)
message(STATUS "Found GLM in ${GLM_INCLUDE_DIR}")
include_directories(${GLM_INCLUDE_DIR})


# Set up assimp
find_package(ASSIMP REQUIRED)
message(STATUS "Found assimp in ${ASSIMP_INCLUDE_DIR}")
include_directories(${ASSIMP_INCLUDE_DIR})


# Set up GLAD library
include_directories(external-libs/glad/include)
add_library(glad STATIC external-libs/glad/src/glad.c)


# Vulkan lib
find_package(Vulkan REQUIRED)
message(STATUS "Found Vulkan in ${Vulkan_INCLUDE_DIR}")
include_directories(${Vulkan_INCLUDE_DIR})


# Set up stb_image library
include_directories(external-libs/stb_image)
add_library(stb_image STATIC external-libs/stb_image/stb_image.cpp)

# Set up Dear IMGUI lib
add_subdirectory(external-libs/imgui)
include_directories(external-libs/imgui)


#Set up the used libs
set(LIBS stb_image glad ${GLFW3_LIBRARY} ${ASSIMP_LIBRARIES} DearIMGUI ${Vulkan_LIBRARIES})


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


## Helper functions

# Links a directory to a symbolic link in the target binary directory
function(copyAssetDir TARGET ASSET)

    if(${UNIX})
        set(SYMLINK_COMMAND ln -s  ${CMAKE_SOURCE_DIR}/${ASSET}/ ${CMAKE_CURRENT_BINARY_DIR}/${ASSET})
    elseif(${WIN32})
        set(SYMLINK_COMMAND mklink /d ${CMAKE_CURRENT_BINARY_DIR}/${ASSET} ${CMAKE_SOURCE_DIR}/${ASSET}/ & exit 0)
    endif()

    add_custom_command(
            TARGET ${TARGET} POST_BUILD
            COMMAND ${SYMLINK_COMMAND})
endfunction()

# Links the assets for a provided target
function(copyAssets TARGET)
    copyAssetDir(${TARGET} "Resources")
endfunction()

# Duplicate a library
function(duplicateLibrary LIBRARY NEW_LIB_NAME)
    get_target_property(sourceFiles ${LIBRARY} SOURCES)
    get_target_property(linkLibs ${LIBRARY} LINK_LIBRARIES)
    get_target_property(includeDirs ${LIBRARY} INCLUDE_DIRECTORIES)
    get_target_property(compileDefinitions ${LIBRARY} COMPILE_DEFINITIONS)
    get_target_property(compileOptions ${LIBRARY} COMPILE_OPTIONS)

    add_library(${NEW_LIB_NAME} STATIC
            ${sourceFiles}
            )

    target_include_directories(${NEW_LIB_NAME} PUBLIC
            ${includeDirs}
            )

    list(REMOVE_ITEM linkLibs
            originalLibrary
            )
    target_link_libraries(${NEW_LIB_NAME} PRIVATE
            ${linkLibs}
            )

    if (compileOptions)
        target_compile_options(${NEW_LIB_NAME} PRIVATE
                ${compileOptions}
                )
    endif()

    if (compileDefinitions)
        target_compile_definitions(${NEW_LIB_NAME} PRIVATE
                ${compileDefinitions}
                )
    endif()
endfunction()


# Set up Kryne Engine library
add_subdirectory(kryne-engine)


# Setting up executables

add_subdirectory(Examples/SunLight)

add_subdirectory(Examples/Sphere)

add_subdirectory(Examples/Basic)

add_subdirectory(Examples/BenchmarkThreading)

add_subdirectory(Examples/VulkanTest)