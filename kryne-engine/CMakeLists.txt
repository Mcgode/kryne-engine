
add_library(kryne-engine STATIC
        src/Material/Shader.cpp include/kryne-engine/Material/Shader.h
        src/Rendering/RenderScene.cpp include/kryne-engine/Rendering/RenderScene.h
        src/Core/VertexArray.cpp include/kryne-engine/Core/VertexArray.h
        src/Core/Window.cpp include/kryne-engine/Core/Window.h
        include/kryne-engine/Camera/OldCamera.h
        src/Camera/BasicCamera.cpp include/kryne-engine/Camera/BasicCamera.h
        src/Core/GeometricShapes.cpp include/kryne-engine/Core/GeometricShapes.h
        src/3DObjects/BaseObject.cpp include/kryne-engine/3DObjects/BaseObject.h
        src/3DObjects/HierarchicalNode.cpp include/kryne-engine/3DObjects/HierarchicalNode.h
        src/Camera/TrackballCamera.cpp include/kryne-engine/Camera/TrackballCamera.h
        src/Light/PointLight.cpp include/kryne-engine/Light/PointLight.h
        src/Light/DirectionalLight.cpp include/kryne-engine/Light/DirectionalLight.h
        src/Core/AdditionalParameters.cpp include/kryne-engine/Core/AdditionalParameters.h
        src/Textures/Texture2D.cpp include/kryne-engine/Textures/Texture2D.h
        src/RenderTargets/PostProcess.cpp include/kryne-engine/RenderTargets/PostProcess.h
        src/RenderTargets/FinalPostProcess.cpp include/kryne-engine/RenderTargets/FinalPostProcess.h
        src/Camera/FirstPersonCamera.cpp include/kryne-engine/Camera/FirstPersonCamera.h
        src/3DObjects/Model/ModelLoader.cpp include/kryne-engine/3DObjects/Model/ModelLoader.h
        src/3DObjects/Model/ModelMesh.cpp include/kryne-engine/3DObjects/Model/ModelMesh.h
        src/3DObjects/Skybox.cpp include/kryne-engine/3DObjects/Skybox.h
        src/Textures/CubeTexture.cpp include/kryne-engine/Textures/CubeTexture.h
        include/kryne-engine/Constants/CubeVertexData.h
        include/kryne-engine/enums/SkyboxDrawOrder.h
        include/kryne-engine/enums/BaseObject.h
        src/Rendering/MainRenderer.cpp include/kryne-engine/Rendering/MainRenderer.h
        include/kryne-engine/Rendering/RenderPass.h
        src/Rendering/ShadowMapping/ShadowMapHandler.cpp include/kryne-engine/Rendering/ShadowMapping/ShadowMapHandler.h
        src/Rendering/ShadowMapping/DirectionalShadowMapRendering.cpp include/kryne-engine/Rendering/ShadowMapping/DirectionalShadowMapRendering.h
        src/Core/OldProcess.cpp include/kryne-engine/Core/OldProcess.h
        src/Light/LightingRegistry.cpp include/kryne-engine/Light/LightingRegistry.h
        src/Rendering/Renderer.cpp include/kryne-engine/Rendering/Renderer.h
        src/3DObjects/Scene.cpp include/kryne-engine/3DObjects/Scene.h
        src/3DObjects/Transform.cpp include/kryne-engine/3DObjects/Transform.h
        src/Material/Material.cpp include/kryne-engine/Material/Material.h
        src/Geometry/BufferGeometry.cpp include/kryne-engine/Geometry/BufferGeometry.h
        src/3DObjects/Mesh.cpp include/kryne-engine/3DObjects/Mesh.h
        src/Geometry/BufferAttribute.cpp include/kryne-engine/Geometry/BufferAttribute.h
        src/Geometry/BoxBufferGeometry.cpp include/kryne-engine/Geometry/BoxBufferGeometry.h
        include/kryne-engine/Rendering/RenderingState.hpp
        include/kryne-engine/Utils/VectorMapping.hpp
        include/kryne-engine/Camera/ProjectionData.hpp
        src/Camera/Camera.cpp include/kryne-engine/Camera/Camera.h
        include/kryne-engine/Camera/PerspectiveProjectionData.hpp
        include/kryne-engine/enums/MaterialSide.h
        src/Material/ShaderChunk.cpp include/kryne-engine/Material/ShaderChunk.h
        src/Material/UniformsHandler.cpp include/kryne-engine/Material/UniformsHandler.h
        src/Rendering/ShaderProgramCompiler.cpp include/kryne-engine/Rendering/ShaderProgramCompiler.h
        include/kryne-engine/Utils/StringUtils.hpp
        src/Input/PlayerInput.cpp include/kryne-engine/Input/PlayerInput.h
        src/Camera/OrbitControlsComponent.cpp include/kryne-engine/Camera/OrbitControlsComponent.h
        include/kryne-engine/Math/Transform.hpp
        include/kryne-engine/Utils/Utils.h src/Textures/Texture.cpp include/kryne-engine/Textures/Texture.h
        include/KryneEngine
        include/KEModules/3DObjects.h
        include/KEModules/Camera.h
        include/KEModules/Constants.h
        include/KEModules/Core.h
        include/KEModules/enums.h
        include/KEModules/Geometry.h
        include/KEModules/Input.h
        include/KEModules/Light.h
        include/KEModules/Material.h
        include/KEModules/Math.h
        include/KEModules/Rendering.h
        include/KEModules/RenderTargets.h
        include/KEModules/Textures.h
        include/KEModules/Utils.h
        src/Material/MeshLambertMaterial.cpp include/kryne-engine/Material/MeshLambertMaterial.h
        src/Material/MeshStandardMaterial.cpp include/kryne-engine/Material/MeshStandardMaterial.h
        src/Material/MeshMaterialCommon.cpp include/kryne-engine/Material/MeshMaterialCommon.h
        src/Core/Entity.cpp include/kryne-engine/Core/Entity.h
        src/Core/Process.cpp include/kryne-engine/Core/Process.h
        src/Core/Component.cpp include/kryne-engine/Core/Component.h
        src/Core/System.cpp include/kryne-engine/Core/System.h
        include/kryne-engine/Core/GraphicContext/GraphicContext.h
        src/Core/GraphicContext/OpenGLContext.cpp include/kryne-engine/Core/GraphicContext/OpenGLContext.h
        src/Rendering/RenderMesh.cpp include/kryne-engine/Rendering/RenderMesh.h
        src/Core/LogicComponent.cpp include/kryne-engine/Core/LogicComponent.h
        include/kryne-engine/enums/SystemTypes.h
        src/Systems/TransformUpdateSystem.cpp include/kryne-engine/Systems/TransformUpdateSystem.h
        include/kryne-engine/Rendering/LoopRenderer.h
        include/kryne-engine/Rendering/Renderers/OpenGLRenderer.h src/Rendering/Renderers/OpenGLRenderer.cpp
        src/Systems/GameLogicComponentsRunner.cpp include/kryne-engine/Systems/GameLogicComponentsRunner.h
        src/Dispatch/Dispatcher.cpp include/kryne-engine/Dispatch/Dispatcher.h
        src/Dispatch/RunnerPool.cpp include/kryne-engine/Dispatch/RunnerPool.h
        src/Dispatch/ChildPool.cpp include/kryne-engine/Dispatch/ChildPool.h
        src/Dispatch/SynchronizablePool.cpp include/kryne-engine/Dispatch/SynchronizablePool.h
        src/Dispatch/SynchronizableChildPool.cpp include/kryne-engine/Dispatch/SynchronizableChildPool.h
        src/Dispatch/BasePool.cpp include/kryne-engine/Dispatch/BasePool.h
        include/kryne-engine/Utils/Debugging.hpp)

target_include_directories(kryne-engine PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

target_link_libraries(kryne-engine ${LIBS})
