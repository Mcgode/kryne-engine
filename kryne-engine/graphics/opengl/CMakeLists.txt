add_library(ke-graphics-gl STATIC
        GlApiContext.cpp GlApiContext.hpp
        GlTexture.cpp GlTexture.hpp
        EnumConversions.hpp)

target_link_libraries(ke-graphics-gl ke-graphics-core)
set_property(TARGET ke-graphics-gl PROPERTY Type "Core")
SetTargetFlags(ke-graphics-gl)