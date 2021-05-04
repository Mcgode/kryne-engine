add_executable(VulkanTest
        VulkanTest.cpp HelloTriangleApp.cpp HelloTriangleApp.hpp DebuggingUtils.hpp Queue.hpp SwapQueue.cpp SwapQueue.hpp Utils.hpp)

target_link_libraries(VulkanTest ${LIBS})

copyAssets(VulkanTest)