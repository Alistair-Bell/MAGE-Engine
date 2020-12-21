include_directories(../Mage)

MageEngineAddUnitTests("TestUnitTests"    "TestUnitTests.c")
MageEngineAddUnitTests("TestNativeWindow" "TestNativeWindow.c")
MageEngineAddUnitTests("TestVulkanLayers" "TestVulkanLayers.c")