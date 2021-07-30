add_library(ke-graphics-gl STATIC
        GlApiContext.cpp GlApiContext.hpp)

target_link_libraries(ke-graphics-gl ke-graphics-core)