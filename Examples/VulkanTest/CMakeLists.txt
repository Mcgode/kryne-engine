add_executable(VulkanTest
        VulkanTest.cpp HelloTriangleApp.cpp HelloTriangleApp.hpp DebuggingUtils.hpp)

target_link_libraries(VulkanTest ${LIBS})