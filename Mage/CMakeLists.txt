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
)

message(STATUS "Building engine for ${PROJECT_BUILD_PLATFORM}")

# platform specific code
if (PROJECT_BUILD_PLATFORM STREQUAL "Linux")
    set(LIBRARY_SOURCES ${LIBRARY_SOURCES}
        Source/Linux/LinuxWindow.c
        Source/Linux/LinuxInputHandler.c
    )
elseif(PROJECT_BUILD_PLATFORM STREQUAL "Windows")
	set(LIBRARY_SOURCES ${LIBRARY_SOURCES}
		Source/Windows/WindowsWindow.c
		Source/Windows/WindowsInputHandler.c
	)
endif()

if (PROJECT_BUILD_RENDERER_MODE STREQUAL "Vulkan")
    set(LIBRARY_SOURCES ${LIBRARY_SOURCES}
        Source/Vulkan/VulkanRenderer.h
        Source/Vulkan/VulkanRenderer.c
    )
endif()

add_library(${PROJECT_NAME} STATIC ${LIBRARY_SOURCES})
MageEngineAddDefinesAndFlagsToBinary(${PROJECT_NAME})
MageEngineLinkPlatformLibrariesToBinary(${PROJECT_NAME})