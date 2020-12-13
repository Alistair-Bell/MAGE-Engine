project(Sandbox)

set(SANDBOX_SOURCES
    Source/Main.c
)

add_executable(${PROJECT_NAME} ${SANDBOX_SOURCES})
include_directories(../Mage)
target_link_libraries(${PROJECT_NAME} MageSDK)

MageEngineAddDefinesAndFlagsToBinary(${PROJECT_NAME})
MageEngineLinkPlatformLibrariesToBinary(${PROJECT_NAME})