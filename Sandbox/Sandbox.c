#include "Sandbox.h"


void ClientLoadProps(struct mageApplicationProps *applicationProps, struct mageRendererProps *rendererProps)
{
    applicationProps->Width = 1920;
    applicationProps->Height = 1080;
    applicationProps->Name = "Hello World";
    applicationProps->Version = 1.0;   
    applicationProps->WindowIcon = "Mage/Resources/Textures/MTEC/Logo.png";
    
    struct mageShader *shaders = calloc(2, sizeof(struct mageShader));
    mageShaderInitialise(&shaders[0], "Mage/Resources/Shaders/vertex.sprv", "main", MAGE_VERTEX_SHADER);
    mageShaderInitialise(&shaders[1], "Mage/Resources/Shaders/fragment.sprv", "main", MAGE_FRAGMENT_SHADER);
    rendererProps->RuntimeShaders = shaders;
    rendererProps->ShaderCount = 2;


}