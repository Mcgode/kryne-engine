add_executable(Sphere 
        main.cpp
        SphereNormalMapping.cpp SphereNormalMapping.h
        SphereBasic.cpp SphereBasic.h
        SphereTexture.cpp SphereTexture.h
        SphereParallax.cpp SphereParallax.h)

target_link_libraries(Sphere PRIVATE kryne-engine)

copyAssets(Sphere)