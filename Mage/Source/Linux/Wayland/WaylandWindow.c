#include "../../Window.h"

U8 MageApplicationWindowCreate(MageApplicationWindowCreateInfo *info, MageApplicationWindow *window)
{
    window->Display = wl_display_create();

    return MageTrue;
}
U8 MageApplicationWindowDestroy(MageApplicationWindow *window)
{

    return MageFalse;
}

