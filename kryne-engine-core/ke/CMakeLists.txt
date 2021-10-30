add_subdirectory(graphics/opengl)

add_library(kryne-engine-core STATIC
        Application.cpp Application.hpp
        Window.hpp Window.cpp
        Logger.cpp Logger.hpp
        additional_impl.cpp)

target_include_directories(kryne-engine-core PUBLIC "${KryneEngineCoreSourceDir}")

if(GraphicsApi STREQUAL "GL")
    set(ApiLib ke-graphics-gl)
elseif(GraphicsApi STREQUAL "VK")
    set(ApiLib ke-graphics-vk)
endif()
target_link_libraries(kryne-engine-core ${ApiLib})

set_property(TARGET kryne-engine-core PROPERTY Type "Core")
SetTargetFlags(kryne-engine-core)

set(KryneEngineCoreLibs "-Wl,--start-group" kryne-engine-core ${ApiLib} "-Wl,--end-group" CACHE STRING "kryne-engine-core")