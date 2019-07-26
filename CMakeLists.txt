cmake_minimum_required(VERSION 3.14)
project(3D_demos)

set(CMAKE_CXX_STANDARD 17)


#Setting up GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(GLFW)


add_subdirectory(glm-0.9.9.5)
include_directories(glm-0.9.9.5)


add_subdirectory(assimp)
include_directories(assimp/include)


#Adding GLAD library
include_directories(glad/include)
add_library(glad glad/src/glad.c assimp/include/assimp/config.h)


# Set up common library
include_directories("common/include")
add_library(common
        common/src/Shader.cpp common/include/common/Shader.h
        common/src/Scene.cpp common/include/common/Scene.h
        common/src/VertexArray.cpp common/include/common/VertexArray.h
        common/src/Window.cpp common/include/common/Window.h
        common/include/common/Camera/Camera.h
        common/src/Camera/BasicCamera.cpp common/include/common/Camera/BasicCamera.h
        common/src/GeometricShapes.cpp common/include/common/GeometricShapes.h
        common/src/BaseObject.cpp common/include/common/BaseObject.h
        common/src/HierarchicalNode.cpp common/include/common/HierarchicalNode.h
        common/src/Camera/TrackballCamera.cpp common/include/common/Camera/TrackballCamera.h
        common/src/Light/PointLight.cpp common/include/common/Light/PointLight.h
        common/src/Light/DirectionalLight.cpp common/include/common/Light/DirectionalLight.h
        common/src/AdditionalParameters.cpp common/include/common/AdditionalParameters.h
        common/src/stb_image.cpp common/include/common/stb_image.h common/src/Texture2D.cpp common/include/common/Texture2D.h common/src/RenderTargets/PostProcess.cpp common/include/common/RenderTargets/PostProcess.h common/src/RenderTargets/FinalPostProcess.cpp common/include/common/RenderTargets/FinalPostProcess.h)
target_link_libraries(common glad glfw assimp glm)


find_package(Boost COMPONENTS system filesystem REQUIRED)
include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIR})


# Setting up executables

add_executable(SunLight SunLight/src/main.cpp)
target_link_libraries(SunLight glad glfw assimp common)

add_executable(Sphere Sphere/main.cpp Sphere/SphereNormalMapping.cpp Sphere/SphereNormalMapping.h Sphere/SphereBasic.cpp Sphere/SphereBasic.h Sphere/SphereTexture.cpp Sphere/SphereTexture.h Sphere/SphereParallax.cpp Sphere/SphereParallax.h)
target_link_libraries(Sphere glad glfw assimp common ${Boost_LIBRARIES})