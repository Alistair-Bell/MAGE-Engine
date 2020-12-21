#include <Mage.h>


static U8 TestValidationLayers(U0 *data)
{
    const char *layers[] = { VK_KHR_SURFACE_EXTENSION_NAME };
    return MageVulkanRendererValidateExtensionsPresent(layers, sizeof(layers) / sizeof(const char *));
}
static U8 TestInstanceExtensions(U0 *data)
{
    const char *extensions[] = { VK_KHR_SURFACE_EXTENSION_NAME };
    return MageVulkanRendererValidateExtensionsPresent(extensions, sizeof(extensions) / sizeof(const char *));
}

I32 main()
{
    MageUnitTestCreateInfo tests[1];
    U32 testCount = (sizeof(tests) / sizeof(MageUnitTestCreateInfo));
    memset(&tests, 0, sizeof(tests));
    
    tests[0].TestName               = "`Unit Tests` - vulkan validation layers test";
    tests[0].FailMessage            = "`Vulkan Validation Layers` module has failed";
    tests[0].ProgramData            = NULL;
    tests[0].ExpectedResult         = MageTrue;
    tests[0].Assertions             = MageFalse;
    tests[0].Callback               = TestValidationLayers;

    tests[0].TestName               = "`Unit Tests` - vulkan instance extensions test";
    tests[0].FailMessage            = "`Vulkan Instance Extensions` module has failed";
    tests[0].ProgramData            = NULL;
    tests[0].ExpectedResult         = MageTrue;
    tests[0].Assertions             = MageFalse;
    tests[0].Callback               = TestInstanceExtensions;

    MageUnitTestRuntimeInfo runtime;
    return MageUnitTestRunTests(tests, testCount, &runtime);
}