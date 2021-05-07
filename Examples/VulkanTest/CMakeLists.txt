add_executable(VulkanTest
        VulkanTest.cpp HelloTriangleApp.cpp HelloTriangleApp.hpp DebuggingUtils.hpp Queue.hpp)

target_link_libraries(VulkanTest ${LIBS})