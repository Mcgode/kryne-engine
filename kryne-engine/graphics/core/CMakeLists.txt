add_library(ke-graphics-core STATIC
        GraphicsApiContext.hpp Window.hpp Texture.hpp)

target_link_libraries(ke-graphics-core ${LIBS})
