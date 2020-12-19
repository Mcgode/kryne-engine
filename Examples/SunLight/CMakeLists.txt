add_executable(SunLight main.cpp
        Floor.cpp Floor.h
        Model.cpp Model.h
        StainedGlass.cpp StainedGlass.h)

target_link_libraries(SunLight PRIVATE kryne-engine)

copyAssets(SunLight)