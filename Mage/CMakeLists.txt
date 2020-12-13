project("MageSDK")


set(LIBRARY_SOURCES 
    Mage.h
    Source/Includes.h
    Source/CreateStructures.h
    Source/Window.h

    Source/UnitTests.h
    Source/UnitTests.c

    Source/InputHandler.h

    Source/EngineApplication.h
    Source/EngineApplication.c
)

message(STATUS "Building engine for ${PROJECT_BUILD_PLATFORM}")

# platform specific code
if (PROJECT_BUILD_PLATFORM STREQUAL "Linux")
    set(LIBRARY_SOURCES ${LIBRARY_SOURCES}
        Source/Linux/Window.c
        Source/Linux/InputHandler.c
    )
else()
    message(FATAL_ERROR "Platform ${PROJECT_BUILD_PLATFORM} has not been implimented or invalid!!")
endif()

add_library(${PROJECT_NAME} STATIC ${LIBRARY_SOURCES})
MageEngineAddDefinesAndFlagsToBinary(${PROJECT_NAME})
set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE C)