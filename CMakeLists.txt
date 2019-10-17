cmake_minimum_required(VERSION 3.14)
project(3D_demos)

set(CMAKE_CXX_STANDARD 17)


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
include_directories(external-libs/assimp/include)


#Adding GLAD library
include_directories(external-libs/glad/include)
add_library(glad external-libs/glad/src/glad.c external-libs/assimp/include/assimp/config.h)


find_package(Boost COMPONENTS system filesystem REQUIRED)
include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIR})


# Set up common library
include_directories("kryne-engine/include")
add_library(kryne-engine
        kryne-engine/src/Core/Shader.cpp kryne-engine/include/kryne-engine/Core/Shader.h
        kryne-engine/src/Rendering/Scene.cpp kryne-engine/include/kryne-engine/Rendering/Scene.h
        kryne-engine/src/Core/VertexArray.cpp kryne-engine/include/kryne-engine/Core/VertexArray.h
        kryne-engine/src/Core/Window.cpp kryne-engine/include/kryne-engine/Core/Window.h
        kryne-engine/include/kryne-engine/Camera/Camera.h
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
        kryne-engine/src/Core/Process.cpp kryne-engine/include/kryne-engine/Core/Process.h)
target_link_libraries(kryne-engine glad glfw assimp glm ${Boost_LIBRARIES})


# Setting up executables

add_executable(SunLight Examples/SunLight/main.cpp
        Examples/SunLight/Floor.cpp Examples/SunLight/Floor.h
        Examples/SunLight/Model.cpp Examples/SunLight/Model.h
        Examples/SunLight/StainedGlass.cpp Examples/SunLight/StainedGlass.h)
target_link_libraries(SunLight kryne-engine)

add_executable(Sphere Examples/Sphere/main.cpp
        Examples/Sphere/SphereNormalMapping.cpp Examples/Sphere/SphereNormalMapping.h
        Examples/Sphere/SphereBasic.cpp Examples/Sphere/SphereBasic.h
        Examples/Sphere/SphereTexture.cpp Examples/Sphere/SphereTexture.h
        Examples/Sphere/SphereParallax.cpp Examples/Sphere/SphereParallax.h)
target_link_libraries(Sphere kryne-engine)