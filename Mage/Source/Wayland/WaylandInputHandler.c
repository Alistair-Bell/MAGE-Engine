#include "../InputHandler.h"

U8 MageInputHandlerCreate(MageInputHandlerCreateInfo *info, MageInputHandler *handler)
{
    memset(handler, 0, sizeof(MageInputHandler));
    return MageTrue;
}
U8 MageInputHandlerPollEvents(MageInputHandler *handler, MageApplicationWindow *window)
{

    return MageTrue; 
}
U8 MageInputHandlerDestroy(MageInputHandler *handler)
{

    return MageTrue;
}
