cmake_minimum_required(VERSION 3.14)
project(3D_demos)

set(CMAKE_CXX_STANDARD 20)


#Setting up GLFW
set(OpenGL_GL_PREFERENCE GLVND)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(external-libs/GLFW)


add_subdirectory(external-libs/glm-0.9.9.5)
include_directories(external-libs/glm-0.9.9.5)


set(ASSIMP_BUILD_ASSIMP_TOOLS OFF)
add_subdirectory(external-libs/assimp)
set(ASSIMP_INCLUDES external-libs/assimp/include ${PROJECT_BINARY_DIR}/assimp/include)
include_directories(${ASSIMP_INCLUDES})
message(STATUS ${PROJECT_BINARY_DIR}/assimp/include/assimp)


#Adding GLAD library
include_directories(external-libs/glad/include)
add_library(glad external-libs/glad/src/glad.c external-libs/assimp/include/assimp/config.h kryne-engine/include/kryne-engine/enums/MaterialSide.h)


set (Boost_DETAILED_FAILURE_MSG ON)
set (Boost_THREADAPI win32)
set (Boost_USE_STATIC_LIBS 1)

set (BASE_BOOST_ROOT "C:/Program Files/boost/boost_1_73_0")

set (BOOST_ROOT ${BASE_BOOST_ROOT}/boost)
set (ENV{BOOST_ROOT} ${BASE_BOOST_ROOT}/boost)

set (BOOST_LIBRARYDIR ${BASE_BOOST_ROOT}/stage/lib)
set (ENV{BOOST_LIBRARYDIR} ${BASE_BOOST_ROOT}/stage/lib)

set (BOOST_INCLUDEDIR ${BASE_BOOST_ROOT})
set (Boost_INCLUDE_DIR ${BASE_BOOST_ROOT})
set (ENV{BOOST_INCLUDEDIR} ${BASE_BOOST_ROOT})

set (Boost_DEBUG ON)  #<---------- Real life saver
find_package(Boost REQUIRED COMPONENTS filesystem system)
include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIR})
message(STATUS "Boost include dirs: ${Boost_INCLUDE_DIRS}")


# Set up common library
include_directories("kryne-engine/include")
add_library(kryne-engine STATIC
        kryne-engine/src/Material/Shader.cpp kryne-engine/include/kryne-engine/Material/Shader.h
        kryne-engine/src/Rendering/RenderScene.cpp kryne-engine/include/kryne-engine/Rendering/RenderScene.h
        kryne-engine/src/Core/VertexArray.cpp kryne-engine/include/kryne-engine/Core/VertexArray.h
        kryne-engine/src/Core/Window.cpp kryne-engine/include/kryne-engine/Core/Window.h
        kryne-engine/include/kryne-engine/Camera/OldCamera.h
        kryne-engine/src/Camera/BasicCamera.cpp kryne-engine/include/kryne-engine/Camera/BasicCamera.h
        kryne-engine/src/Core/GeometricShapes.cpp kryne-engine/include/kryne-engine/Core/GeometricShapes.h
        kryne-engine/src/3DObjects/BaseObject.cpp kryne-engine/include/kryne-engine/3DObjects/BaseObject.h
        kryne-engine/src/3DObjects/HierarchicalNode.cpp kryne-engine/include/kryne-engine/3DObjects/HierarchicalNode.h
        kryne-engine/src/Camera/TrackballCamera.cpp kryne-engine/include/kryne-engine/Camera/TrackballCamera.h
        kryne-engine/src/Light/PointLight.cpp kryne-engine/include/kryne-engine/Light/PointLight.h
        kryne-engine/src/Light/DirectionalLight.cpp kryne-engine/include/kryne-engine/Light/DirectionalLight.h
        kryne-engine/src/Core/AdditionalParameters.cpp kryne-engine/include/kryne-engine/Core/AdditionalParameters.h
        kryne-engine/src/Textures/stb_image.cpp kryne-engine/include/kryne-engine/Textures/stb_image.h
        kryne-engine/src/Textures/Texture2D.cpp kryne-engine/include/kryne-engine/Textures/Texture2D.h
        kryne-engine/src/RenderTargets/PostProcess.cpp kryne-engine/include/kryne-engine/RenderTargets/PostProcess.h
        kryne-engine/src/RenderTargets/FinalPostProcess.cpp kryne-engine/include/kryne-engine/RenderTargets/FinalPostProcess.h
        kryne-engine/src/Camera/FirstPersonCamera.cpp kryne-engine/include/kryne-engine/Camera/FirstPersonCamera.h
        kryne-engine/src/3DObjects/Model/ModelLoader.cpp kryne-engine/include/kryne-engine/3DObjects/Model/ModelLoader.h
        kryne-engine/src/3DObjects/Model/ModelMesh.cpp kryne-engine/include/kryne-engine/3DObjects/Model/ModelMesh.h
        kryne-engine/src/3DObjects/Skybox.cpp kryne-engine/include/kryne-engine/3DObjects/Skybox.h
        kryne-engine/src/Textures/TextureCubeMap.cpp kryne-engine/include/kryne-engine/Textures/TextureCubeMap.h
        kryne-engine/include/kryne-engine/Constants/CubeVertexData.h
        kryne-engine/include/kryne-engine/enums/SkyboxDrawOrder.h
        kryne-engine/include/kryne-engine/enums/BaseObject.h
        kryne-engine/src/Rendering/MainRenderer.cpp kryne-engine/include/kryne-engine/Rendering/MainRenderer.h
        kryne-engine/include/kryne-engine/Rendering/RenderPass.h
        kryne-engine/src/Rendering/ShadowMapping/ShadowMapHandler.cpp kryne-engine/include/kryne-engine/Rendering/ShadowMapping/ShadowMapHandler.h
        kryne-engine/src/Rendering/ShadowMapping/DirectionalShadowMapRendering.cpp kryne-engine/include/kryne-engine/Rendering/ShadowMapping/DirectionalShadowMapRendering.h
        kryne-engine/src/Core/Process.cpp kryne-engine/include/kryne-engine/Core/Process.h
        kryne-engine/src/Light/LightingRegistry.cpp kryne-engine/include/kryne-engine/Light/LightingRegistry.h
        kryne-engine/src/Rendering/Renderer.cpp kryne-engine/include/kryne-engine/Rendering/Renderer.h
        kryne-engine/src/3DObjects/Scene.cpp kryne-engine/include/kryne-engine/3DObjects/Scene.h
        kryne-engine/src/3DObjects/Object3D.cpp kryne-engine/include/kryne-engine/3DObjects/Object3D.h
        kryne-engine/src/Material/Material.cpp kryne-engine/include/kryne-engine/Material/Material.h
        kryne-engine/src/Geometry/BufferGeometry.cpp kryne-engine/include/kryne-engine/Geometry/BufferGeometry.h
        kryne-engine/src/3DObjects/Mesh.cpp kryne-engine/include/kryne-engine/3DObjects/Mesh.h
        kryne-engine/src/Geometry/BufferAttribute.cpp kryne-engine/include/kryne-engine/Geometry/BufferAttribute.h
        kryne-engine/src/Geometry/BoxBufferGeometry.cpp kryne-engine/include/kryne-engine/Geometry/BoxBufferGeometry.h
        kryne-engine/include/kryne-engine/Rendering/RenderingState.hpp
        kryne-engine/src/Utils/VectorFlattening.cpp kryne-engine/include/kryne-engine/Utils/VectorFlattening.h
        kryne-engine/include/kryne-engine/Camera/ProjectionData.hpp
        kryne-engine/src/Camera/Camera.cpp kryne-engine/include/kryne-engine/Camera/Camera.h
        kryne-engine/include/kryne-engine/Camera/PerspectiveProjectionData.hpp
        kryne-engine/src/Material/ShaderChunk.cpp kryne-engine/include/kryne-engine/Material/ShaderChunk.h
        kryne-engine/src/Material/UniformsHandler.cpp kryne-engine/include/kryne-engine/Material/UniformsHandler.h
        kryne-engine/src/Rendering/ShaderProgramCompiler.cpp kryne-engine/include/kryne-engine/Rendering/ShaderProgramCompiler.h)
target_link_libraries(kryne-engine glad glfw assimp glm ${Boost_LIBRARIES})


function(COPYASSETDIR TARGET ASSET)

    if(${UNIX})
        message(WARN todo)
    elseif(${WIN32})
        set(SYMLINK_COMMAND mklink /d ${CMAKE_BINARY_DIR}/${ASSET} ${CMAKE_SOURCE_DIR}/${ASSET}/ & exit 0)
        message("Command is : '${SYMLINK_COMMAND}'")
    endif()

    add_custom_command(
            TARGET ${TARGET} POST_BUILD
            COMMAND ${SYMLINK_COMMAND})
endfunction()

function(COPYASSETS TARGET)
    copyAssetDir(${TARGET} "Resources")
endfunction()


# Setting up executables

add_executable(SunLight Examples/SunLight/main.cpp
        Examples/SunLight/Floor.cpp Examples/SunLight/Floor.h
        Examples/SunLight/Model.cpp Examples/SunLight/Model.h
        Examples/SunLight/StainedGlass.cpp Examples/SunLight/StainedGlass.h)
target_link_libraries(SunLight kryne-engine)
copyAssets(SunLight)

add_executable(Sphere Examples/Sphere/main.cpp
        Examples/Sphere/SphereNormalMapping.cpp Examples/Sphere/SphereNormalMapping.h
        Examples/Sphere/SphereBasic.cpp Examples/Sphere/SphereBasic.h
        Examples/Sphere/SphereTexture.cpp Examples/Sphere/SphereTexture.h
        Examples/Sphere/SphereParallax.cpp Examples/Sphere/SphereParallax.h)
target_link_libraries(Sphere kryne-engine)
copyAssets(Sphere)

add_executable(Basic Examples/Basic/main.cpp)
target_link_libraries(Basic kryne-engine)
copyAssets(Basic)