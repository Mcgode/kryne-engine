add_library(ke-graphics-gl STATIC
        GlApiContext.cpp GlApiContext.hpp
        GlTexture.cpp GlTexture.hpp
        EnumConversions.hpp GlContextParameters.hpp)

target_include_directories(ke-graphics-gl PRIVATE "${KryneEngineCoreSourceDir}")
target_link_libraries(ke-graphics-gl ${LIBS})
set_property(TARGET ke-graphics-gl PROPERTY Type "Core")
SetTargetFlags(ke-graphics-gl)