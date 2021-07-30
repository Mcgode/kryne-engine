add_library(ke-graphics-gl STATIC
        GlGraphicsApi.cpp GlGraphicsApi.hpp)

target_link_libraries(ke-graphics-gl ke-graphics-core)