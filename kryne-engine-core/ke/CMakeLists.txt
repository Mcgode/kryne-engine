add_subdirectory(graphics/opengl)

add_library(kryne-engine-core STATIC
        Application.cpp Application.hpp
        Window.hpp Window.cpp
        Logger.cpp Logger.hpp
        )

target_include_directories(kryne-engine-core PUBLIC "${KryneEngineCoreSourceDir}")

if(GraphicsApi STREQUAL "GL")
    target_link_libraries(kryne-engine-core ke-graphics-gl)
elseif(GraphicsApi STREQUAL "VK")
    target_link_libraries(kryne-engine-core ke-graphics-vk)
endif()

set_property(TARGET kryne-engine-core PROPERTY Type "Core")
SetTargetFlags(kryne-engine-core)
