project("MageSDK")


set(LIBRARY_SOURCES 
    Mage.h
    Source/Includes.h
    Source/CreateStructures.h
    Source/Window.h
    Source/Renderer.h

    Source/UnitTests.h
    Source/UnitTests.c

    Source/InputHandler.h
    Source/InputHandler.c

    Source/EngineApplication.h
    Source/EngineApplication.c
   
    Source/FileSystem.h
    Source/FileSystem.c

    Source/Assets/AssetsLoader.h
    Source/Assets/AssetsLoader.c

    Source/Maths.h
    Source/Maths/MathsGeneric.c

    Source/Threading.h

    Source/Vulkan/VulkanPhysicalDevice.c
    Source/Vulkan/VulkanSurface.c
    Source/Vulkan/VulkanSwapChain.c
	Source/Vulkan/VulkanLoader.c
    Source/Vulkan/VulkanRenderer.h
    Source/Vulkan/VulkanRenderer.c
    Source/Vulkan/VulkanShaders.c
    Source/Vulkan/VulkanGraphicsPipeline.c
    Source/Vulkan/VulkanRenderPass.c
    Source/Vulkan/VulkanCommandBuffers.c
    Source/Vulkan/VulkanFrameBuffer.c
    Source/Vulkan/VulkanSynchronisation.c
)

message(STATUS "Building engine for ${PROJECT_BUILD_PLATFORM}")

# platform specific code
if (PROJECT_BUILD_PLATFORM STREQUAL "Linux")
    set(LIBRARY_SOURCES ${LIBRARY_SOURCES}
        Source/Linux/LinuxWindow.c
        Source/Linux/LinuxInputHandler.c
        Source/Linux/LinuxThreading.c
    )
elseif(PROJECT_BUILD_PLATFORM STREQUAL "Windows")
	set(LIBRARY_SOURCES ${LIBRARY_SOURCES}
		Source/Windows/WindowsWindow.c
		Source/Windows/WindowsInputHandler.c
	)
endif()

add_library(${PROJECT_NAME} STATIC ${LIBRARY_SOURCES})
MageEngineAddDefinesAndFlagsToBinary(${PROJECT_NAME})
MageEngineLinkPlatformLibrariesToBinary(${PROJECT_NAME})
