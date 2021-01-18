include_directories(../Mage)

set(TEST_UTILITY_SOURCES
    CommonResources.h
    CommonResources.c
)
add_library(TestUtilities STATIC ${TEST_UTILITY_SOURCES})
MageEngineAddFlagsAndLink(TestUtilities)

message(STATUS "${PROJECT_BUILD_PLATFORM}")

function(MageEngineAddUnitTests binary source)
    add_executable(${binary} ${source})
	target_link_libraries(${binary} MageSDK)
    target_link_libraries(${binary} TestUtilities)
    add_test(NAME ${binary} COMMAND ${binary})
    MageEngineAddFlagsAndLink(${binary})
endfunction(MageEngineAddUnitTests binary source)



MageEngineAddUnitTests("TestUnitTests"     "TestUnitTests.c")
MageEngineAddUnitTests("TestNativeWindow"  "TestNativeWindow.c")
MageEngineAddUnitTests("TestVulkanLayers"  "TestVulkanLayers.c")
MageEngineAddUnitTests("TestFileSystem"    "TestFileSystem.c")
MageEngineAddUnitTests("TestThreading"     "TestThreading.c")
MageEngineAddUnitTests("TestRenderer"      "TestRenderer.c")
