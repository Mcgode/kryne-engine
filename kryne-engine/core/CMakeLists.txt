add_library(ke-core STATIC
        Application.cpp Application.hpp
        Window.hpp Window.cpp)

if(GraphicsApi STREQUAL "GL")
    target_link_libraries(ke-core ke-graphics-gl)
elseif(GraphicsApi STREQUAL "VK")
    target_link_libraries(ke-core ke-graphics-vk)
endif()

set_property(TARGET ke-core PROPERTY Type "Core")
SetTargetFlags(ke-core)
