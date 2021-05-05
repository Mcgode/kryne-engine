add_executable(VulkanTest
        VulkanTest.cpp
        HelloTriangleApp.cpp HelloTriangleApp.hpp
        DebuggingUtils.hpp
        Queue.cpp Queue.hpp
        SwapChain.cpp SwapChain.hpp
        Utils.hpp VertexBuffer.cpp VertexBuffer.hpp)

target_link_libraries(VulkanTest ${LIBS})

copyAssets(VulkanTest)