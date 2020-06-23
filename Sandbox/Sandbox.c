#include <mageAPI.h>

#include "Sandbox.h"

struct mageShader VertexShader;
struct mageShader FragmentShader;
struct mageShader RuntimeShaders[2];


static mageResult ClientStart()
{
    return MAGE_SUCCESS;
}
static void ClientUpdate()
{

}
static mageResult ClientEnd()
{
    return MAGE_SUCCESS;
}

struct mageApplicationProps ClientApplicationProps()
{
    struct mageApplicationProps props;
    memset(&props, 0, sizeof(struct mageApplicationProps)); 
    props.Name = "Hello World";
    props.Width = 1920;
    props.Height = 1080;
    props.Version = 1.0;

    props.StartMethod = ClientStart;
    props.UpdateMethod = ClientUpdate;
    props.DestroyMethod = ClientEnd;

    return props;   
}
struct mageRendererProps ClientRendererProps()
{
    struct mageRendererProps props;
    memset(&props, 0, sizeof(struct mageRendererProps));

    props.RuntimeShaders = NULL;
    props.ShaderCount    = 0;

    return props;
}