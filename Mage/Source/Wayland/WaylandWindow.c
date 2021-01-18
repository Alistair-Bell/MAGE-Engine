#include "../Window.h"

static U0 MageApplicationWindowRegistryHandler(U0 *data, struct wl_registry *registry, U32 id, const char *interface, U32 version)
{
    printf("Inform: [%s:%d]\n", interface, id);
    MageApplicationWindow *window = ((MageApplicationWindow *)data);
    if (strcmp(interface, "wl_compositor") == 0)
        window->Compositor = wl_registry_bind(registry, id, &wl_compositor_interface, 1);
}
static U0 MageApplicationWindowRegistryRemover(U0 *data, struct wl_registry *registry, U32 id)
{

}

U8 MageApplicationWindowCreate(MageApplicationWindowCreateInfo *info, MageApplicationWindow *window)
{
    window->Display = wl_display_connect(NULL);
    MAGE_HANDLE_ERROR_MESSAGE(window->Display == NULL, printf("Error: Unable to open Wayland display\n"));
    struct wl_registry *registry = wl_display_get_registry(window->Display);

    struct wl_registry_listener listener;
    memset(&listener, 0, sizeof(struct wl_registry_listener));
    listener.global        = MageApplicationWindowRegistryHandler;
    listener.global_remove = MageApplicationWindowRegistryRemover;
    wl_registry_add_listener(registry, &listener, window); 

    wl_display_dispatch(window->Display);
    wl_display_terminate(window->Display);

    MAGE_HANDLE_ERROR_MESSAGE(window->Compositor == NULL, printf("Error: Unable to find Wayland compositor\n"));

    window->Surface = wl_compositor_create_surface(window->Compositor);
    MAGE_HANDLE_ERROR_MESSAGE(window->Surface == NULL, printf("Error: Unable to create Wayland surface from compositor\n"));
    return MageTrue; 
}
U8 MageApplicationWindowGetDimensions(MageApplicationWindow *window, MageApplicationWindowDimensions *dimensions)
{
    memset(dimensions, 0, sizeof(MageApplicationWindowDimensions));
    return MageTrue;
}
U8 MageApplicationWindowGetFramebufferDimensions(MageApplicationWindow *window, MageApplicationWindowDimensions *dimensions)
{

    return MageTrue;
}
U8 MageApplicationWindowSetTitle(MageApplicationWindow *window, const char *title)
{

    return MageTrue;
}
U8 MageApplicationWindowDestroy(MageApplicationWindow *window)
{
    wl_display_disconnect(window->Display);
    return MageFalse;
}

