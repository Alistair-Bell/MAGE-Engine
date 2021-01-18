#include <Mage.h>

#define STR_ARRAY_COUNT(arr) (sizeof(arr) / sizeof(const char *))

static U8 TestValidationLayers(U0 *data)
{
    const char *layers[] = { "VK_LAYER_KHRONOS_validation"  };
    return MageVulkanRendererValidateLayersPresent(layers, STR_ARRAY_COUNT(layers));
}
static U8 TestInstanceExtensions(U0 *data)
{
    const char *extensions[] = { VK_KHR_SURFACE_EXTENSION_NAME };
    return MageVulkanRendererValidateExtensionsPresent(extensions, STR_ARRAY_COUNT(extensions));
}

I32 main()
{
    MageUnitTestCreateInfo tests[2];
    U32 testCount = (sizeof(tests) / sizeof(MageUnitTestCreateInfo));
    memset(&tests, 0, sizeof(tests));
    
    tests[0].TestName               = "`Unit Tests` - vulkan validation layers test";
    tests[0].FailMessage            = "`Vulkan Validation Layers` module has failed";
    tests[0].ProgramData            = NULL;
    tests[0].ExpectedResult         = MageTrue;
    tests[0].Assertions             = MageFalse;
    tests[0].Callback               = TestValidationLayers;

    tests[1].TestName               = "`Unit Tests` - vulkan instance extensions test";
    tests[1].FailMessage            = "`Vulkan Instance Extensions` module has failed";
    tests[1].ProgramData            = NULL;
    tests[1].ExpectedResult         = MageTrue;
    tests[1].Assertions             = MageFalse;
    tests[1].Callback               = TestInstanceExtensions;

    MageUnitTestRuntimeInfo runtime;
    return MageUnitTestRunTests(tests, testCount, &runtime);
}
