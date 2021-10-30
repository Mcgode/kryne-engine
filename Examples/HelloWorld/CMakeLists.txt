add_executable(HelloWorld HelloWorld.cpp)

target_link_libraries(HelloWorld ${KryneEngineCoreLibs})
set_target_properties(HelloWorld PROPERTIES Type "Exe")
SetTargetFlags(HelloWorld)

copyAssets(HelloWorld)