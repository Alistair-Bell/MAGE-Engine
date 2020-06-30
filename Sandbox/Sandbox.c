#include <mageAPI.h>

#include "Sandbox.h"

struct mageShader VertexShader;
struct mageShader FragmentShader;
struct mageShader RuntimeShaders[2];
mageEventListenerCallback Callbacks[1];

static void EventListener(void *event, mageEventType type)
{
    assert(0 == 0);
}

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
    
    Callbacks[0] = EventListener;

    props.ListenerCount = 1;
    props.Listeners = Callbacks;

    return props;   
}
struct mageRendererProps ClientRendererProps()
{
    struct mageRendererProps props;
    memset(&props, 0, sizeof(struct mageRendererProps));


    mageShaderInitialise(&VertexShader, "Mage/Resources/Shaders/vertex.sprv", "main", MAGE_VERTEX_SHADER);
    mageShaderInitialise(&FragmentShader, "Mage/Resources/Shaders/fragment.sprv", "main", MAGE_FRAGMENT_SHADER);

    RuntimeShaders[0] = VertexShader;
    RuntimeShaders[1] = FragmentShader;

    props.RuntimeShaders = RuntimeShaders;
    props.ShaderCount    = 2;

    return props;
}