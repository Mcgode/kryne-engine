
set(KryneEngineCoreSourceDir "${CMAKE_CURRENT_SOURCE_DIR}")
message("${KryneEngineCoreSourceDir}")

add_subdirectory(ke)

#add_library(kryne-engine-core STATIC)
#target_include_directories(kryne-engine-core PUBLIC .)
#set_property(TARGET kryne-engine-core PROPERTY Type "Core")
#SetTargetFlags(kryne-engine-core)
