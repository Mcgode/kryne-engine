add_executable(Basic
        Basic.cpp PerspectiveHelper.cpp PerspectiveHelper.hpp)

target_link_libraries(Basic PRIVATE kryne-engine)
set_target_properties(Basic PROPERTIES Type "Exe")
SetTargetFlags(Basic)

copyAssets(Basic)