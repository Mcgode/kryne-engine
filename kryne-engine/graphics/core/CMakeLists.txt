add_library(ke-graphics-core STATIC
        GraphicsApiContext.hpp Texture.hpp)

target_link_libraries(ke-graphics-core ${LIBS})
set_property(TARGET ke-graphics-core PROPERTY Type "Core")
SetTargetFlags(ke-graphics-core)
